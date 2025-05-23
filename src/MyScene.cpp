#include <QJsonObject>
#include <QtGui>
#include <QMessageBox>
#include "MyScene.h"
#include "QGraphicsRectItem"
#include "QTimer"
#include "QDebug"
#include "QKeyEvent"
#include "QJsonDocument"
#include "QFile"
#include "QJsonArray"
#include "player.h"
#include "projectiles.h"
#include "QGraphicsSceneMouseEvent"

MyScene::MyScene(QObject* parent) : QGraphicsScene(parent) {
    /*qgri = new QGraphicsRectItem(1, 1, 1920, 1080); //ajout d'un carré
    this->addItem(qgri);

    qgti = new QGraphicsTextItem("CIR2 Nantes");     //ajout d'un texte
    this->addItem(qgti);*/

    //initialisation du jeu
    createMap();
    createPersonage();
    createEnnemies(100, 100, 200, 200, 300, 300, 400, 400, 450, 450);

    addToList(entities, personage, enemy1, enemy2, enemy3, enemy4, enemy5);



    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MyScene::Movement);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    connect(personage, &player::died, this, &MyScene::handlePlayerDeath);
    enemyTimer = new QTimer(this);
    connect(enemyTimer, &QTimer::timeout, this, &MyScene::moveEnemies);


}

void MyScene::addToList(QList<player*>& entities, player* p,player* p1, player* p2, player* p3, player* p4, player* p5){
    entities.append(p);
    entities.append(p1);
    entities.append(p2);
    entities.append(p3);
    entities.append(p4);
    entities.append(p5);
}

player* MyScene::getPlayer() {
    if(this->personage != nullptr) {
        return this->personage;
    }
    qDebug() << "Error: player not found";
    return nullptr;
}

void MyScene::createMap(){
    QFile file("../map/bonmap.json");
    file.open(QIODevice::ReadOnly);
    QJsonDocument document = QJsonDocument::fromJson(file.readAll());
    QJsonObject mapObject = document.object();

    //initialise une liste avec des tuiles
    QMap<int, QPixmap> listPixmap;

    int tileWidth = mapObject["tilewidth"].toInt();
    int tileHeight = mapObject["tileheight"].toInt();
    int numberTileWidth = mapObject["width"].toInt();
    int numberTileHeight = mapObject["height"].toInt();
    this->backgroundWidth = numberTileWidth * tileWidth;
    this->backgroundHeight = numberTileHeight * tileHeight;

    QJsonArray tilesets = mapObject["tilesets"].toArray();
    for(QJsonValue tilesetValue : tilesets){
        QJsonObject tileset = tilesetValue.toObject();
        int firstGid = tileset["firstgid"].toInt();
        QString source = tileset["image"].toString();
        QPixmap tilesetImage(source); //load image
        if(tilesetImage.isNull()){
            qDebug() << "Error loading tileset image: " << source;
            continue;
        }
        int numColumns = tilesetImage.width() / tileWidth;
        int numRows = tilesetImage.height() / tileHeight;
        for (int row = 0; row < numRows; ++row) {
            for (int column = 0; column < numColumns; ++column) {
                int tileID = firstGid + (row * numColumns) + column;

                //We add one by one tiles so we 'cut' the image everytime
                QRect tileRect(column * tileWidth, row * tileHeight, tileWidth, tileHeight);
                QPixmap tilePixmap = tilesetImage.copy(tileRect);
                listPixmap[tileID] = tilePixmap;
            }
        }
    }
    QJsonArray layers = mapObject["layers"].toArray();
    for(QJsonValue layerValue : layers) {
        QJsonObject layer = layerValue.toObject();
        if (layer["type"] == "tilelayer") {
            int width = layer["width"].toInt();
            int height = layer["height"].toInt();
            QJsonArray data = layer["data"].toArray();
            for (int y = 0; y < height; y++) { //line
                for (int x = 0; x < width; x++) { //column
                    int tileID = data[width * y + x].toInt();
                    if (tileID != 0) {
                        QGraphicsPixmapItem *tile = new QGraphicsPixmapItem(listPixmap[tileID]);
                        tile->setPos(x * 32, y * 32);
                        tile->setOpacity(layer["opacity"].toDouble());
                        this->addItem(tile);//draw the tile at the right position
                    }
                }
            }

        }
        //place les arbres au dessus du joueur
        if (layer["type"] == "tilelayer") {
            QString layerName = layer["name"].toString();
            bool isTreeLayer = (layerName == "arbres");

            int width = layer["width"].toInt();
            int height = layer["height"].toInt();
            QJsonArray data = layer["data"].toArray();
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int tileID = data[width * y + x].toInt();
                    if (tileID != 0) {
                        QGraphicsPixmapItem *tile = new QGraphicsPixmapItem(listPixmap[tileID]);
                        tile->setPos(x * 32, y * 32);
                        tile->setOpacity(layer["opacity"].toDouble());
                        if (isTreeLayer) {
                            tile->setZValue(10);
                        }
                        this->addItem(tile);
                    }
                }
            }
        }
            //Ajouts des collisions
        else if (layer["type"] == "objectgroup" && layer["name"] == "collision") {
            QJsonArray objects = layer["objects"].toArray();
            for (QJsonValue objectValue: objects) {
                QJsonObject object = objectValue.toObject();
                int x = object["x"].toInt();
                int y = object["y"].toInt();
                int width = object["width"].toInt();
                int height = object["height"].toInt();
                bool isEllipse = object.contains("ellipse") && object["ellipse"].toBool();

                if (isEllipse) {
                    QGraphicsEllipseItem *ellipse = new QGraphicsEllipseItem(x, y, width, height);
                    ellipse->setBrush(Qt::transparent); // Transparent pour ne pas les voir en jeu
                    ellipse->setPen(Qt::NoPen);
                    ellipse->setData(0, "collision");
                    ellipse->setZValue(100);
                    this->addItem(ellipse);
                    collisionItems.append(ellipse); // Ajout à la liste de collision
                } else {
                    QGraphicsRectItem *rect = new QGraphicsRectItem(x, y, width, height);
                    rect->setBrush(Qt::transparent); // Transparent pour ne pas les voir en jeu
                    rect->setPen(Qt::NoPen);
                    rect->setData(0, "collision");
                    rect->setZValue(11);
                    this->addItem(rect);
                    collisionItems.append(rect); // Ajout à la liste de collision
                }
            }
        }
    }
    file.close();
}

void MyScene::createPersonage() {
    this->personage = new player(100,'p');
    this->addItem(personage);
    personage->setZValue(5);
}

void MyScene::createEnnemies(int x1, int y1,int x2,int y2,int x3,int y3,int x4,int y4,int x5,int y5) {
    //1
    this->enemy1 = new player(50,'e');
    this->addItem(enemy1);
    enemy1->setZValue(5);
    enemy1->setPos(x1, y1);
    connect(enemy1, &player::enemyKilled, this->personage, &player::addPoints);
    //2
    this->enemy2 = new player(50,'e');
    this->addItem(enemy2);
    enemy2->setZValue(5);
    enemy2->setPos(x2, y2);
    connect(enemy2, &player::enemyKilled, this->personage, &player::addPoints);
    //3
    this->enemy3 = new player(50,'e');
    this->addItem(enemy3);
    enemy3->setZValue(5);
    enemy3->setPos(x3, y3);
    connect(enemy3, &player::enemyKilled, this->personage, &player::addPoints);
    //4
    this->enemy4 = new player(50,'e');
    this->addItem(enemy4);
    enemy4->setZValue(5);
    enemy4->setPos(x4, y4);
    connect(enemy4, &player::enemyKilled, this->personage, &player::addPoints);
    //5
    this->enemy5 = new player(50,'e');
    this->addItem(enemy5);
    enemy5->setZValue(5);
    enemy5->setPos(x5, y5);
    connect(enemy5, &player::enemyKilled, this->personage, &player::addPoints);
}

void MyScene::update(){
}

bool MyScene::checkCollision(QPointF newPos) {
    // On crée un rectangle correspondant à la hitbox du personnage à la nouvelle position
    QRectF playerRect = personage->boundingRect();
    playerRect.moveTopLeft(newPos);
    // Vérifier si ce rectangle entre en collision avec un des objets de collision
            foreach(QGraphicsItem* item, collisionItems) {
            QRectF itemRect = item->sceneBoundingRect();
            if (playerRect.intersects(itemRect)) {
                return true; // Collision détectée
            }
        }
    return false; // Pas de collision
}

void MyScene::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        qDebug() << "Le jeu a été quitté";
        movementTimer->stop();
        return;
    }
    if (!isGameActive || event->isAutoRepeat()) {
        return;
    }
    pressedKeys.insert(event->key());
}

void MyScene::Movement() {
    if (!isGameActive || !personage){
        return;
    }
    QPointF currentPos = personage->pos();
    QPointF newPos = currentPos;
    if(pressedKeys.contains(Qt::Key_B)){
        qDebug() << currentPos;
    }

    //mouvement diagonal
    if ((pressedKeys.contains(Qt::Key_Q) && pressedKeys.contains(Qt::Key_Z)) || (pressedKeys.contains(Qt::Key_Up) && pressedKeys.contains(Qt::Key_Left))) {
        personage->setAnimation("up");
        newPos.setY(newPos.y() - sqrt(2));
        newPos.setX(newPos.x() - sqrt(2));
    }
    else if ((pressedKeys.contains(Qt::Key_D) && pressedKeys.contains(Qt::Key_S)) || (pressedKeys.contains(Qt::Key_Down) && pressedKeys.contains(Qt::Key_Right))) {
        personage->setAnimation("down");
        newPos.setY(newPos.y() + sqrt(2));
        newPos.setX(newPos.x() + sqrt(2));
    }
    else if ((pressedKeys.contains(Qt::Key_D) && pressedKeys.contains(Qt::Key_Z)) || (pressedKeys.contains(Qt::Key_Up) && pressedKeys.contains(Qt::Key_Right))) {
        personage->setAnimation("up");
        newPos.setY(newPos.y() - sqrt(2));
        newPos.setX(newPos.x() + sqrt(2));
    }
    else if ((pressedKeys.contains(Qt::Key_Q) && pressedKeys.contains(Qt::Key_S)) || (pressedKeys.contains(Qt::Key_Down) && pressedKeys.contains(Qt::Key_Left))) {
        personage->setAnimation("down");
        newPos.setY(newPos.y() + sqrt(2));
        newPos.setX(newPos.x() - sqrt(2));
    }

    else{
    //mouvement ligne droite
        if (pressedKeys.contains(Qt::Key_S) || pressedKeys.contains(Qt::Key_Down)) {
            personage->setAnimation("down");
            newPos.setY(newPos.y() + 2);
            setLookingDirection('b');
        }
        if (pressedKeys.contains(Qt::Key_Z) || pressedKeys.contains(Qt::Key_Up)) {
            personage->setAnimation("up");
            newPos.setY(newPos.y() - 2);
            setLookingDirection('h');
        }
        if (pressedKeys.contains(Qt::Key_Q) || pressedKeys.contains(Qt::Key_Left)) {
            personage->setAnimation("left");
            newPos.setX(newPos.x() - 2);
            setLookingDirection('g');
        }
        if (pressedKeys.contains(Qt::Key_D) || pressedKeys.contains(Qt::Key_Right)) {
            personage->setAnimation("right");
            setLookingDirection('d');
            newPos.setX(newPos.x() + 2);
        }
    }
    if (pressedKeys.contains(Qt::Key_P)) {
        personage->setAnimation("p");
    }
    if (newPos != currentPos && !checkCollision(newPos)) {
        personage->setPos(newPos);
    }
}

void MyScene::setLookingDirection(char direction){
    lookingDirection = direction;
}

void MyScene::keyReleaseEvent(QKeyEvent* event) {
    if (!isGameActive || event->isAutoRepeat()) {
        return;
    }
    pressedKeys.remove(event->key());
}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    qDebug() << "Mouse pressed at: ";
    if (!isGameActive) {
        return;
    }
    if (event->button() == Qt::RightButton) {
        QPointF playerPos = getPlayer()->pos();  // Position du joueur
        QPointF mouseScenePos = event->scenePos();  // Position de la souris dans la scène

        // Appliquer la réduction de distance
        QPointF reducedTarget = reduceLengthAttack(playerPos, mouseScenePos);

        // Créer et lancer le Molotov vers la position ajustée
        Molotov* molotov = new Molotov(10, "../anim/molotov.gif");
        molotov->launchTowards(playerPos, reducedTarget);
        molotov->setZValue(5);
        this->addItem(molotov);

    }
    if(event->button() == Qt::LeftButton) {
        QPointF playerPos = getPlayer()->pos();

        QString swordGifPath;
        switch(getLookingDirection()) {
            case 'd':
                swordGifPath = "../anim/sword_right.gif";
                break;
            case 'g':
                swordGifPath = "../anim/sword_left.gif";
                break;
            case 'h':
                swordGifPath = "../anim/sword_up.gif";
                break;
            case 'b':
                swordGifPath = "../anim/sword_down.gif";
                break;
        }

        Sword* sword = new Sword(50, swordGifPath);
        sword->setZValue(5);
        // Positionnement du sprite
        switch(getLookingDirection()) {
            case 'd':
                sword->setPos(playerPos.x() + 25, playerPos.y());
                break;
            case 'g':
                sword->setPos(playerPos.x() - 25, playerPos.y());
                break;
            case 'h':
                sword->setPos(playerPos.x(), playerPos.y() - 25);
                break;
            case 'b':
                sword->setPos(playerPos.x(), playerPos.y() + 25);
                break;
        }

        this->addItem(sword);
    }

    QGraphicsScene::mousePressEvent(event);  // Appel à la méthode parente
}


QPointF MyScene::reduceLengthAttack(const QPointF& playerPos, const QPointF& targetPos) {
    QPointF direction = targetPos - playerPos;
    double dx = direction.x();
    double dy = direction.y();
    double distance = sqrt(dx * dx + dy * dy);

    // Limite de distance (par exemple, 250 pixels)
    if (distance > 100.0) {
        double scale = 100.0 / distance; // Proportionner la direction
        direction.setX(dx * scale);
        direction.setY(dy * scale);
    }
    return playerPos + direction; // Retourne la position ajustée
}

void MyScene::handlePlayerDeath() {
    isGameActive = false; // bloque les mouvements et les attaques
    personage->setAnimation("die");
    QTimer::singleShot(1000, this, [this]() {
        emit gameOver();
    });
}

void MyScene::showGameOverMessage() {
    QGraphicsTextItem* dieMsg = new QGraphicsTextItem("Game Over");
    dieMsg->setDefaultTextColor(Qt::red);
    dieMsg->setFont(QFont("Arial", 30));
    dieMsg->setPos(backgroundWidth / 2 - 100, backgroundHeight / 2 - 50);
    dieMsg->setZValue(1000);
    this->addItem(dieMsg);
}
QList<player*> MyScene::getEnemies() const {
    // Retourne uniquement les ennemis vivants (pas en cours de suppression)
    QList<player*> aliveEnemies;
    for (player* enemy : entities) {
        if (enemy && !enemy->isDying && enemy->getLife() > 0) {
            aliveEnemies.append(enemy);
        }
    }
    return aliveEnemies;
}

void MyScene::removeEnemy(player* enemy) {
    entities.removeAll(enemy);
    if (enemy) {
        enemy->deleteLater();
    }
}

void MyScene::moveEnemies() {
    player* playerCharacter = nullptr;

    for (player* p : getEnemies()) {
        if (p->getType() == 'p') {
            playerCharacter = p;
            break;
        }
    }
    if (!playerCharacter) return;

    QPointF playerPos = playerCharacter->pos();

    for (player* enemy : getEnemies()) {
        if (!enemy || enemy->isDying || enemy->getType() == 'p') continue;

        QPointF enemyPos = enemy->pos();
        QPointF newPos = enemyPos;

        qreal distance = QLineF(enemyPos, playerPos).length();

        if (distance < 50.0) {
            // Direction vers joueur
            QPointF dir = playerPos - enemyPos;

            if (std::abs(dir.x()) > std::abs(dir.y())) {
                if (dir.x() > 0)
                    enemy->setAnimation("attackRight");
                else
                    enemy->setAnimation("attackLeft");
            } else {
                if (dir.y() > 0)
                    enemy->setAnimation("attackDown");
                else
                    enemy->setAnimation("attackUp");
            }
            playerCharacter->damaged(playerCharacter->getLife() - 5);

        } else if (distance < 150.0) {
            // Poursuite
            QPointF direction = playerPos - enemyPos;
            qreal length = std::sqrt(direction.x()*direction.x() + direction.y()*direction.y());
            if (length != 0) {
                direction /= length;
                newPos += direction * 10.0; // vitesse d’approche
            }

            if (std::abs(direction.x()) > std::abs(direction.y())) {
                if (direction.x() > 0)
                    enemy->setAnimation("right");
                else
                    enemy->setAnimation("left");
            } else {
                if (direction.y() > 0)
                    enemy->setAnimation("down");
                else
                    enemy->setAnimation("up");
            }

        } else {
            // Errance aléatoire
            int dir = QRandomGenerator::global()->bounded(4);
            switch (dir) {
                case 0:
                    enemy->setAnimation("up");
                    newPos.setY(enemyPos.y() - 2);
                    break;
                case 1:
                    enemy->setAnimation("down");
                    newPos.setY(enemyPos.y() + 2);
                    break;
                case 2:
                    enemy->setAnimation("left");
                    newPos.setX(enemyPos.x() - 2);
                    break;
                case 3:
                    enemy->setAnimation("right");
                    newPos.setX(enemyPos.x() + 2);
                    break;
            }
        }

        if (!checkCollision(newPos)) {
            enemy->setPos(newPos);
        }
    }
}

void MyScene::start() {
    isGameActive = true;
    timer->start(30);
    enemyTimer->start(300);
}







MyScene::~MyScene() {

}
