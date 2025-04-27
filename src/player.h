#ifndef PROJECT_CPP_PLAYER_H
#define PROJECT_CPP_PLAYER_H

#include <QObject>
#include <QGraphicsObject>
#include <QGraphicsPixmapItem>
#include <QMovie>
#include "QPainter"

class player : public QGraphicsObject {
Q_OBJECT

private:
    int life;
    int attack;
    QPixmap pixmap;
    QMovie* currentMovie;
    QMap<QString, QMovie*> movies; // <--- Map des animations par direction

public:
    player(int life, int attack);
    ~player();

    int getLife();
    void setAnimation(const QString& direction);

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;
};

#endif //PROJECT_CPP_PLAYER_H
