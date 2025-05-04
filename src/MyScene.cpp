#include <QJsonObject>
#include "MyScene.h"
#include "QGraphicsRectItem"
#include "QGraphicsPixmapItem"
#include "QTimer"
#include "QDebug"
#include "QKeyEvent"
#include "QJsonDocument"
#include "QFile"
#include "QJsonArray"
#include "QMovie"
#include "player.h"
#include <QGraphicsPixmapItem>
#include "mainWindow.h"

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

    //connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(30); //toutes les 30 millisecondes



}

player* MyScene::getPlayer() {
    if(this->personage != nullptr) {
        return this->personage;
    }
    qDebug() << "Error: player not found";
    return nullptr;
}


void MyScene::keyReleaseEvent(QKeyEvent *event) {
    
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
    this->personage = new player(3, 1);
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
                qDebug() << "Collision détectée avec l'objet à" << itemRect;
                return true; // Collision détectée
            }
        }
    return false; // Pas de collision
}

void MyScene::keyPressEvent(QKeyEvent *event) {
    QPointF currentPos = personage->pos();
    QPointF newPos = currentPos;

    if(event->key() == Qt::Key_Escape) {
        qDebug() << "Le jeu à été quitté";
        timer->stop();
    }
    if(event->key()== Qt::Key_S && event->key()== Qt::Key_D){
        newPos.setX(currentPos.x() + 5);
        newPos.setY(currentPos.y() + 5);
    }
    if(event->key()== Qt::Key_Down && event->key()== Qt::Key_Right){
        newPos.setX(currentPos.x() + 5);
        newPos.setY(currentPos.y() + 5);
    }
    if(event->key()== Qt::Key_Z && event->key()== Qt::Key_D){
        newPos.setX(currentPos.x() + 5);
        newPos.setY(currentPos.y() - 5);
    }
    if(event->key()== Qt::Key_Up && event->key()== Qt::Key_Right){
        newPos.setX(currentPos.x() + 5);
        newPos.setY(currentPos.y() - 5);
    }
    if(event->key()== Qt::Key_Z && event->key()== Qt::Key_Q){
        newPos.setX(currentPos.x() - 5);
        newPos.setY(currentPos.y() - 5);
    }
    if(event->key()== Qt::Key_Up && event->key()== Qt::Key_Left){
        newPos.setX(currentPos.x() - 5);
        newPos.setY(currentPos.y() - 5);
    }
    if(event->key()== Qt::Key_S && event->key()== Qt::Key_Q){
        newPos.setX(currentPos.x() - 5);
        newPos.setY(currentPos.y() + 5);
    }
    if(event->key()== Qt::Key_Down && event->key()== Qt::Key_Left){
        newPos.setX(currentPos.x() - 5);
        newPos.setY(currentPos.y() + 5);
    }
    else if(event->key() == Qt::Key_S || event->key() == Qt::Key_Down) {
        this->personage->setAnimation("down");
        newPos.setY(currentPos.y() + 5);
    }
    else if(event->key() == Qt::Key_Z || event->key() == Qt::Key_Up) {
        this->personage->setAnimation("up");
        newPos.setY(currentPos.y() - 5);
    }
    else if(event->key() == Qt::Key_Q || event->key() == Qt::Key_Left) {
        this->personage->setAnimation("left");
        newPos.setX(currentPos.x() - 5);
    }
    else if(event->key() == Qt::Key_D || event->key() == Qt::Key_Right) {
        this->personage->setAnimation("right");
        newPos.setX(currentPos.x() + 5);
    }
    // Vérifiez s'il y a une collision avant de déplacer le personnage
    if (!checkCollision(newPos)) {
        personage->setPos(newPos);

    }
}


MyScene::~MyScene() {

}
