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
    void setLife(int newLife);
    void setAnimation(const QString& direction);

    player* getPlayer() {
        return this;
    }
    void setPos(qreal x, qreal y) {
        QGraphicsItem::setPos(x, y);
        emit positionChanged(this);  // Émet le signal avec le joueur en paramètre
    }

    void setPos(const QPointF &pos) {
        QGraphicsItem::setPos(pos);
        emit positionChanged(this);  // Émet le signal avec le joueur en paramètre
    }

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;

    signals:
    void positionChanged(player* playerCharacter);
    void lifeChanged(int newLife);

};

#endif //PROJECT_CPP_PLAYER_H
