#ifndef CPP_QT_TPMINIPROJET_MYSCENE_H
#define CPP_QT_TPMINIPROJET_MYSCENE_H

#include <QGraphicsScene>
#include "player.h"


class MyScene : public QGraphicsScene {
    Q_OBJECT

public:
    MyScene(QObject* parent = nullptr);
    virtual ~MyScene();
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void createMap();
    void createPersonage();
    player* getPlayer();
    QTimer* timer;


    public slots:
    void update();

private:
    QGraphicsPixmapItem* map;

    int backgroundWidth;
    int backgroundHeight;
    player* personage;
    QList<QGraphicsItem*> collisionItems;
    bool checkCollision(QPointF newPos);
};


#endif //CPP_QT_TPMINIPROJET_MYSCENE_H
