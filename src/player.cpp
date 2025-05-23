#include "player.h"
#include <iostream>
#include <QPainter>
#include <QMovie>

player::player(int life,char type): life(life), currentMovie(nullptr){
     // position de base
    setType(type); //initialise le type
    if(type == 'p') {
        // Charger toutes les animations
        setPos(798, 837); // position du spawn
        movies["down"] = new QMovie("../anim/personage_down.gif",QByteArray(),this);
        movies["up"] = new QMovie("../anim/personage_up.gif",QByteArray(),this);
        movies["left"] = new QMovie("../anim/personage_left.gif",QByteArray(),this);
        movies["right"] = new QMovie("../anim/personage_right.gif",QByteArray(),this);
        movies["p"] = new QMovie("../anim/personage_p.gif",QByteArray(),this);
        movies["die"] = new QMovie("../anim/personage_die.gif",QByteArray(),this);
        //movies["wait"] = new QMovie("../anim/personage_wait.gif",QByteArray(),this);

        // Démarrer une animation par défaut

        setAnimation("down");
        setScore(0);
        setScale(1.4);
        setFlag(QGraphicsItem::ItemIsFocusable);
        setFocus();
    }
    if(type == 'e') {
        // Charger toutes les animations
        movies["down"] = new QMovie("../anim/sbire_down.gif",QByteArray(),this);
        movies["up"] = new QMovie("../anim/sbire_up.gif",QByteArray(),this);
        movies["left"] = new QMovie("../anim/sbire_left.gif",QByteArray(),this);
        movies["right"] = new QMovie("../anim/sbire_right.gif",QByteArray(),this);
        movies["base"] = new QMovie("../anim/sbire_base.gif",QByteArray(),this);
        movies["attackRight"] = new QMovie("../anim/sbire_attack_right.gif",QByteArray(),this);
        movies["attackLeft"] = new QMovie("../anim/sbire_attack_left.gif",QByteArray(),this);
        movies["attackUp"] = new QMovie("../anim/sbire_attack_up.gif",QByteArray(),this);
        movies["attackDown"] = new QMovie("../anim/sbire_attack_down.gif",QByteArray(),this);

        // Démarrer une animation par défaut
        setAnimation("base");
        setScale(1.4);
    }
    setTransformOriginPoint(16, 16);
    //BUG ennemies non détectable
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemHasNoContents, false); // assure que le contenu est visible
}

player::~player() {
    qDebug() << "Player deleted";
    movies.clear();
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
        // Ajoute ce rectangle de debug autour de chaque entité, même avec image
        //painter->setPen(Qt::green);
        //painter->drawRect(boundingRect());
    } else {
        painter->setBrush(Qt::red);
        painter->drawRect(0, 0, 32, 32);  // Debug visible
    }
}

int player::getLife() {
    return life;
}

void player::setLife(int newLife) {
    if (isDying) {
        return; // Si le joueur est déjà en train de mourir, ne pas changer la vie
    }
    if(newLife>0) { // Assure que la vie ne soit pas négative
        life = newLife;
        qDebug() << "Life updated : " << getPlayer();
        emit lifeChanged(newLife);
    } else{
        life = 0;
        emit lifeChanged(0);
        if(getType() == 'p'){
            playDeathAnimationForMainPlayer();
        }
        else if(getType() == 'b'){
            emit enemyKilled(50);
        }
        else {
            emit enemyKilled(10);
            this->deleteLater(); // Suppression de l'ennemi avec un delai pour evité le crash de colliding
            isDying = true;
        }
    }
}

void player::damaged(int newLife){
    setLife(newLife);
}

void player::playDeathAnimationForMainPlayer() {
    QMovie* deathMovie = movies.value("die", nullptr);
    if (!deathMovie) {
        qWarning() << "Animation de mort introuvable!";
        emit died();
        return;
    }
    currentMovie = deathMovie;
    currentMovie->stop();
    currentMovie->start();
    currentMovie->setCacheMode(QMovie::CacheAll);
    connect(currentMovie, &QMovie::frameChanged, this, [this](int){
        this->pixmap = currentMovie->currentPixmap();
        this->update();
    });
    connect(currentMovie, &QMovie::finished, this, [this](){
        emit died();  // Signal que la mort est terminée
        this->hide();
    });
}




