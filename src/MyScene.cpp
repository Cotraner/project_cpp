#include <QJsonObject>
#include <QtGui>
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
    createMap();
    createPersonage();
    this->addItem(personage);
    QTimer* timer;
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MyScene::Movement);

   connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(30); //toutes les 30 millisecondes

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

            //Ajouts des collision
        } else if (layer["type"] == "objectgroup" && layer["name"] == "collision") {
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
                    rect->setZValue(100);
                    this->addItem(rect);
                    collisionItems.append(rect); // Ajout à la liste de collision
                }
            }
        }
    }
    file.close();
}

void MyScene::createPersonage() {
    this->personage = new player(100);
    this->addItem(personage);
    personage->setZValue(100);
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
                personage->setLife(personage->getLife() - 5);
                //qDebug() << "Collision détectée avec l'objet à" << itemRect;
                return true; // Collision détectée
            }
        }
    return false; // Pas de collision
}

void MyScene::keyPressEvent(QKeyEvent* event) {
    if (event->isAutoRepeat())
        return;

    if (event->key() == Qt::Key_Escape) {
        qDebug() << "Le jeu a été quitté";
        movementTimer->stop();
        return;
    }

    pressedKeys.insert(event->key());
}

void MyScene::Movement() {
    if (!personage) return;

    QPointF currentPos = personage->pos();
    QPointF newPos = currentPos;

    if (pressedKeys.contains(Qt::Key_S) || pressedKeys.contains(Qt::Key_Down)) {
        personage->setAnimation("down");
        newPos.setY(newPos.y() + 2);
    }
    if (pressedKeys.contains(Qt::Key_Z) || pressedKeys.contains(Qt::Key_Up)) {
        personage->setAnimation("up");
        newPos.setY(newPos.y() - 2);
    }
    if (pressedKeys.contains(Qt::Key_Q) || pressedKeys.contains(Qt::Key_Left)) {
        personage->setAnimation("left");
        newPos.setX(newPos.x() - 2);
    }
    if (pressedKeys.contains(Qt::Key_D) || pressedKeys.contains(Qt::Key_Right)) {
        personage->setAnimation("right");
        newPos.setX(newPos.x() + 2);
    }
    if (pressedKeys.contains(Qt::Key_P)) {
        personage->setAnimation("p");
    }
    if (newPos != currentPos && !checkCollision(newPos)) {
        personage->setPos(newPos);
    }
}

void MyScene::keyReleaseEvent(QKeyEvent* event) {
    if (event->isAutoRepeat())
        return;

    pressedKeys.remove(event->key());
}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::RightButton) {
        QPointF playerPos = getPlayer()->pos();  // Position du joueur
        QPointF mouseScenePos = event->scenePos();  // Position de la souris dans la scène

        // Appliquer la réduction de distance
        QPointF reducedTarget = reduceLengthAttack(playerPos, mouseScenePos);

        // Créer et lancer le Molotov vers la position ajustée
        Molotov* molotov = new Molotov(50, "../anim/molotov.gif");
        molotov->launchTowards(playerPos, reducedTarget);
        this->addItem(molotov);

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





MyScene::~MyScene() {

}
