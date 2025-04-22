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

MyScene::MyScene(QObject* parent) : QGraphicsScene(parent) {
    map = new QGraphicsPixmapItem();
    map->setPixmap(QPixmap("../map/WIN_20250227_10_22_20_Pro.jpg")); //ajout d'une image
    this->addItem(map);

    qgri = new QGraphicsRectItem(10, 100, 300, 200); //ajout d'un carré
    this->addItem(qgri);


    qgti = new QGraphicsTextItem("CIR2 Nantes");     //ajout d'un texte
    this->addItem(qgti);


    QTimer* timer;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(30); //toutes les 30 millisecondes

    createMap();
}

void MyScene::update(){
    QPointF pos = qgti->pos();
    qgti->setPos(pos.rx(), pos.ry()+1);
    if (qgti->collidesWithItem(qgri)) {
        qDebug() << "Collision !";
    }
}

void MyScene::keyPressEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key_S) {
        //code pour descendre
    }
    if(event->key() == Qt::Key_Z) {
        //code pour monter
    }
    if(event->key() == Qt::Key_Escape) {
        timer->stop();
    }
    //...
}

void MyScene::keyReleaseEvent(QKeyEvent *event) {
    
}

void MyScene::createMap(){
    //Load and parse json file
    QFile file("../map/bonmap.json");
    file.open(QIODevice::ReadOnly);

    QJsonDocument document = QJsonDocument::fromJson(file.readAll());
    QJsonObject mapObject = document.object();

    //listPixmap will contains every tiles
    QMap<int, QPixmap> listPixmap;

    int tileWidth = mapObject["tilewidth"].toInt();
    int tileHeight = mapObject["tileheight"].toInt();
    int numberTileWidth = mapObject["width"].toInt();
    int numberTileHeight = mapObject["height"].toInt();

    this->backgroundWidth = numberTileWidth * tileWidth;
    this->backgroundHeight = numberTileHeight * tileHeight;

    //First we get every tiles to add it into listPixmap
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
    for(QJsonValue layerValue : layers){
        QJsonObject layer = layerValue.toObject();

        if(layer["type"] == "tilelayer"){
            int width = layer["width"].toInt();
            int height = layer["height"].toInt();

            QJsonArray data = layer["data"].toArray();
            for(int y = 0; y < height; y++){ //line
                for(int x = 0; x < width; x++){ //column
                    int tileID = data[width * y + x].toInt();
                    if(tileID != 0){
                        QGraphicsPixmapItem* tile = new QGraphicsPixmapItem(listPixmap[tileID]);
                        tile->setPos(x * 32, y * 32);
                        tile->setOpacity(layer["opacity"].toDouble());
                        this->addItem(tile);//draw the tile at the right position
                    }
                }
            }

            //Add collisions objects
        } else if(layer["type"] == "objectgroup" && layer["name"] == "collisions"){
            QJsonArray objects = layer["objects"].toArray();

            for(QJsonValue objectValue : objects){
                QJsonObject object = objectValue.toObject();

                int x = object["x"].toInt();
                int y = object["y"].toInt();
                int width = object["width"].toInt();
                int height = object["height"].toInt();
                bool isEllipse = object.contains("ellipse") && object["ellipse"].toBool();

                if (isEllipse) {
                    QGraphicsEllipseItem* ellipse = new QGraphicsEllipseItem(x, y, width, height);
                    ellipse->setBrush(Qt::red);
                    ellipse->setPen(Qt::NoPen);
                    ellipse->setData(0, "collision");
                    ellipse->setZValue(100);
                    this->addItem(ellipse);
                } else {
                    QGraphicsRectItem* rect = new QGraphicsRectItem(x, y, width, height);
                    rect->setBrush(Qt::red);
                    rect->setPen(Qt::NoPen);
                    rect->setData(0, "collision");
                    rect->setZValue(100);
                    this->addItem(rect);
                }
            }
        }
    }

    file.close();
}

MyScene::~MyScene() {

}
