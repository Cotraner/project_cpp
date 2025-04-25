#ifndef PROJECT_CPP_PLAYER_H
#define PROJECT_CPP_PLAYER_H

#include <QObject>
#include <QGraphicsObject>
#include <QGraphicsPixmapItem>
#include <QMovie>

class player : public QGraphicsObject {
    Q_OBJECT
    
private:
    int life;
    int attack;

public:
    player(int life, int attack) : life(life), attack(attack) {
        setPos(200,200);
    }
    int getLife();

//redefine paint and boundingRect


    QRectF boundingRect() const override {
        return QRectF(0, 0, 32, 32);
    }


    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override {

};
#endif //PROJECT_CPP_PLAYER_H
