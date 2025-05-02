#include "player.h"
#include <iostream>
#include <QGraphicsObject>
#include <QPainter>
#include <QGraphicsPixmapItem>
#include "player.h"
#include <QMovie>

player::player(int life, int attack): life(life), attack(attack), currentMovie(nullptr){
    setPos(500, 200); // position de base

    // Charger toutes les directions
    movies["down"] = new QMovie("../anim/personage_down.gif");
    //movies["up"] = new QMovie("../anim/personage_up.gif");
    //movies["left"] = new QMovie("../anim/personage_left.gif");
    //movies["right"] = new QMovie("../anim/personage_right.gif");

    // Démarrer une animation par défaut
    setAnimation("down");

    setScale(1.4);
    setTransformOriginPoint(16, 16);
}

player::~player() {
    for (auto movie : movies) {
        delete movie;
    }
}

void player::setAnimation(const QString& direction) {
    if (movies.contains(direction)) {
        if (currentMovie) {
            disconnect(currentMovie, nullptr, this, nullptr);
        }

        currentMovie = movies[direction];
        connect(currentMovie, &QMovie::frameChanged, this, [this]() {
            this->pixmap = currentMovie->currentPixmap();
            update();
        });

        currentMovie->start();
    }
}

QRectF player::boundingRect() const {
    return QRectF(0, 0, 32, 32);
}

void player::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    if (!pixmap.isNull()) {
        painter->drawPixmap(boundingRect(), pixmap, pixmap.rect());
    }
}

int player::getLife() {
    return life;
}
