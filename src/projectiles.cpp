#include "projectiles.h"
#include <QPainter>
#include <QGraphicsScene>
#include <QPropertyAnimation>
#include <QTimer>
#include "MyScene.h"

Molotov::Molotov(int damage, const QString& gifPath)
        : damage(damage)
{
    movie = new QMovie(gifPath);
    movie->start();

    // Mise à jour du visuel quand le frame change
    connect(movie, &QMovie::frameChanged, this, &Molotov::updateFrame);
}

QRectF Molotov::boundingRect() const {
    if (movie && movie->currentPixmap().isNull()){
        return QRectF();
    }
    return QRectF(0, 0, movie->currentPixmap().width(), movie->currentPixmap().height());
}

void Molotov::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    if (movie) {
        painter->drawPixmap(0, 0, movie->currentPixmap());
        //painter->setPen(QPen(Qt::red));  // Couleur du contour
        //painter->setBrush(QBrush(QColor(255, 0, 0, 50)));  // Couleur semi-transparente pour l'intérieur
        //painter->drawRect(boundingRect());  // Dessiner le rectangle de détection
    }
}

void Molotov::updateFrame() {
    update(); // Redessine l'objet avec la nouvelle frame du GIF
}

void Molotov::launchTowards(const QPointF& startPos, const QPointF& targetPos) {
    setPos(startPos);
    auto* animation = new QPropertyAnimation(this, "pos");
    animation->setDuration(1000); // 1 seconde
    animation->setStartValue(startPos);
    animation->setEndValue(targetPos);
    animation->setEasingCurve(QEasingCurve::Linear);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
    connect(movie, &QMovie::frameChanged, this, [this](int frameNumber) {
        if (frameNumber == movie->frameCount() - 1) {
            startExplosion();  // Appelle l’explosion quand le GIF est fini
        }
    });
}

QList<player*> Molotov::getEnemiesFromScene() const {
    QList<player*> enemies;
    // On caste la scène en MyScene pour accéder à son vecteur d'ennemis
    if (auto* myScene = dynamic_cast<MyScene*>(scene())) {
        enemies = myScene->getEnemies();  // Méthode à ajouter en public dans MyScene
    }
    return enemies;
}


void Molotov::startExplosion() {
    if (!scene()) return;

    auto* myScene = dynamic_cast<MyScene*>(scene());
    if (!myScene) return;

    QList<QGraphicsItem*> colliding = collidingItems();
    qDebug() << "Molotov colliding with" << colliding.size() << "items";

    for (QGraphicsItem* item : colliding) {
        player* entity = dynamic_cast<player*>(item);
        if (!entity || entity->isDying) continue;

        int newLife = entity->getLife() - getDamage();
        entity->damaged(newLife);

        // Si c’est un ennemi basique, on le retire
        if (newLife <= 0 && entity->getType() == 'e') {
            myScene->removeEnemy(entity);
        }
    }

    if (scene()) scene()->removeItem(this);
    deleteLater();
}



Molotov::~Molotov(){
    delete movie;
    movie = nullptr;
}

Sword::Sword(int damage, const QString& gifPath): damage(damage){
    movie = new QMovie(gifPath);
    movie->setCacheMode(QMovie::CacheAll);
    movie->start();

    connect(movie, &QMovie::frameChanged, this, &Sword::updateFrame);

    QTimer::singleShot(0, this, [this]() {
        QList<player*> enemies = getEnemiesFromScene();
        checkCollisionWithPlayer(enemies);
    });

    int totalDuration = 0;
    for (int i = 0; i < movie->frameCount(); ++i) {
        totalDuration += movie->nextFrameDelay();
    }

// Supprimer l’objet après la lecture complète
    QTimer::singleShot(totalDuration, this, [this]() {
        if (scene()) {
            scene()->removeItem(this);
        }
        QTimer::singleShot(0, this, [this]() {
            this->deleteLater();
        });
    });

}

Sword::~Sword(){
    delete movie;
    movie = nullptr;
}

void Sword::updateFrame() {
    update(); // Redessine l'objet avec la nouvelle frame du GIF
}

QRectF Sword::boundingRect() const {
    if (movie && movie->currentPixmap().isNull()) {
        return QRectF();
    }
    return QRectF{0, 0, static_cast<qreal>(movie->currentPixmap().width()), static_cast<qreal>(movie->currentPixmap().height())};
}

void Sword::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    if (movie) {
        painter->drawPixmap(0, 0, movie->currentPixmap());
        //painter->setPen(QPen(Qt::red));  // Couleur du contour
        //painter->setBrush(QBrush(QColor(255, 0, 0, 50)));  // Couleur semi-transparente pour l'intérieur
        //painter->drawRect(boundingRect());  // Dessiner le rectangle de détection
    }
}

QList<player*> Sword::getEnemiesFromScene() const {
    QList<player*> enemies;
    if (auto* myScene = dynamic_cast<MyScene*>(scene())) {
        enemies = myScene->getEnemies();  // Liste mise à jour à chaque appel
    }
    return enemies;
}


void Sword::checkCollisionWithPlayer(const QList<player*>& enemies) {
    auto* myScene = dynamic_cast<MyScene*>(scene());
    for (player* enemy : enemies) {
        if (!enemy->isDying && collidesWithItem(enemy) && enemy->getType() != 'p') {
            qDebug() << "→ Collision détectée avec :" << enemy;
            int newLife = enemy->getLife() - getDamage();
            enemy->damaged(newLife);
            // Supprime uniquement si la vie tombe à 0 ET ce n’est pas un boss
            if (newLife <= 0 && enemy->getType() != 'b') {
                myScene->removeEnemy(enemy);  // supprime l'ennemi de la liste
            }
        }
    }
}

QRectF BossProjectile::boundingRect() const {
    if (!movie || movie->currentPixmap().isNull())
        return QRectF();

    return QRectF(0, 0, movie->currentPixmap().width(), movie->currentPixmap().height());
}

void BossProjectile::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    if (movie) {
        painter->drawPixmap(0, 0, movie->currentPixmap());
    }
}

BossProjectile::BossProjectile(int damage, const QString& gifPath)
        : damage(damage) {
    movie = new QMovie(gifPath);
    movie->setCacheMode(QMovie::CacheAll);
    movie->start();
    connect(movie, &QMovie::frameChanged, this, [this]() {
        this->update();
    });
}

void BossProjectile::launch(const QPointF& start, const QPointF& end, player* p) {
    this->targetPlayer = p;

    QSize spriteSize = movie->currentPixmap().size();
    QPointF centerOffset(spriteSize.width() / 2.0, spriteSize.height() / 2.0);

    setPos(start - centerOffset);

    auto* anim = new QPropertyAnimation(this, "pos");
    anim->setStartValue(start - centerOffset);
    anim->setEndValue(end - centerOffset);
    anim->setDuration(3000);
    anim->setEasingCurve(QEasingCurve::Linear);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
    connect(anim, &QPropertyAnimation::finished, this, [this]() {
        if (scene()) scene()->removeItem(this);
        deleteLater();
    });
}

bool BossProjectile::isWall(QGraphicsItem* item) {
    return item && item->data(0).isValid() && item->data(0).toString() == "collision";
}

int BossProjectile::getDamage(){
    return damage;
}

void BossProjectile::advance(int phase) {
    if (phase == 0 || hashit || !scene()) return;

    // Détection de collision avec un mur
    for (QGraphicsItem* item : collidingItems()) {
        if (isWall(item)) {
            hashit = true;
            scene()->removeItem(this);
            deleteLater();
            return;
        }
    }

    // Détection de collision avec le joueur
    if (targetPlayer && !targetPlayer->isDying && collidesWithItem(targetPlayer)) {
        targetPlayer->damaged(targetPlayer->getLife() - damage);
        hashit = true;
        scene()->removeItem(this);
        deleteLater();
    }
}

BossProjectile::~BossProjectile() {
    delete movie;
}




