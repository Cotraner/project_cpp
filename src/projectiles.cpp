#include "projectiles.h"
#include <QPainter>
#include <QGraphicsScene>
#include <QPropertyAnimation>
#include <QTimer>

Molotov::Molotov(int damage, const QString& gifPath)
        : damage(damage)
{
    movie = new QMovie(gifPath);
    movie->start();

    // Mise à jour du visuel quand le frame change
    connect(movie, &QMovie::frameChanged, this, &Molotov::updateFrame);
}

Molotov::~Molotov() {
    delete movie;
}

QRectF Molotov::boundingRect() const {
    if (movie && movie->currentPixmap().isNull()) return QRectF();
    return QRectF(0, 0, movie->currentPixmap().width(), movie->currentPixmap().height());
}

void Molotov::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    if (movie) {
        painter->drawPixmap(0, 0, movie->currentPixmap());
        painter->setPen(QPen(Qt::red));  // Couleur du contour
        painter->setBrush(QBrush(QColor(255, 0, 0, 50)));  // Couleur semi-transparente pour l'intérieur
        painter->drawRect(boundingRect());  // Dessiner le rectangle de détection
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
    connect(movie, &QMovie::frameChanged, this, [=](int frameNumber) {
        if (frameNumber == movie->frameCount() - 1) {
            startExplosion();  // Appelle l’explosion quand le GIF est fini
        }
    });
}

void Molotov::startExplosion() {
    QRectF impactZone = mapToScene(boundingRect()).boundingRect();
    QList<QGraphicsItem*> itemsInZone = scene()->items(impactZone);

    for (QGraphicsItem* item : itemsInZone) {
        if (item == this) continue;

        QObject* obj = dynamic_cast<QObject*>(item);
        if (player* p = qobject_cast<player*>(obj)) {
            p->damaged(p->getLife() -getDamage());
        }
    }

    scene()->removeItem(this);
    deleteLater();
}

void Molotov::checkCollisionWithPlayer(player* player) {
    if (!player) return;

    // Vérifie si le boundingRect du Molotov intersecte avec celui du joueur
    if (this->collidesWithItem(player)) {
        player->damaged(player->getLife() - getDamage());  // Déclenche automatiquement lifeChanged
    }
}