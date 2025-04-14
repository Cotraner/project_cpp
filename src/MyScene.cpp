#include "MyScene.h"
#include "QGraphicsRectItem"
#include "QGraphicsPixmapItem"
#include "QTimer"
#include "QDebug"
#include "QKeyEvent"

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

MyScene::~MyScene() {

}
