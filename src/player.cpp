#include "player.h"
#include <iostream>
#include <QPainter>
#include <QMovie>

player::player(int life,char type): life(life), currentMovie(nullptr){
    setPos(500, 200); // position de base
    if(type == 'p') {
        // Charger toutes les directions
        movies["down"] = new QMovie("../anim/personage_down.gif");
        movies["up"] = new QMovie("../anim/personage_up.gif");
        movies["left"] = new QMovie("../anim/personage_left.gif");
        movies["right"] = new QMovie("../anim/personage_right.gif");
        movies["p"] = new QMovie("../anim/personage_p.gif");
        movies["die"] = new QMovie("../anim/personage_die.gif");
        //movies["wait"] = new QMovie("../anim/personage_wait.gif");

        // Démarrer une animation par défaut
        setAnimation("down");
        setScale(1.4);
    }
    if(type == 'e') {
        // Charger toutes les directions
        movies["down"] = new QMovie("../anim/sbire_down.gif");
        movies["up"] = new QMovie("../anim/sbire_up.gif");
        movies["left"] = new QMovie("../anim/sbire_left.gif");
        movies["right"] = new QMovie("../anim/sbire_right.gif");
        movies["base"] = new QMovie("../anim/sbire_base.gif");

        // Démarrer une animation par défaut
        setAnimation("base");
        setScale(2);
    }

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
        painter->drawPixmap(0, 0, pixmap);
    } else {
        painter->setBrush(Qt::red);
        painter->drawRect(0, 0, 32, 32);  // Debug visible
    }
}

int player::getLife() {
    return life;
}

void player::setLife(int newLife) {
    if(newLife>0) { // Assure que la vie ne soit pas négative
        life = newLife;
        emit lifeChanged(newLife);
    }
    else{
        emit lifeChanged(newLife);
        emit died();
    }
}

void player::damaged(int newLife){
    setLife(newLife);
}
