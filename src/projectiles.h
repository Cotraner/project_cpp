#ifndef PROJECT_CPP_PROJECTILES_H
#define PROJECT_CPP_PROJECTILES_H

#include <QObject>
#include <QGraphicsObject>
#include <QMovie>
#include "player.h"

class Molotov : public QGraphicsObject {
Q_OBJECT

private:
    int damage;
    QList<player*> toDelete;
    QMovie* movie;

public:
    Molotov(int damage, const QString& gifPath);
    ~Molotov();

    void setDamage(int damage) { this->damage = damage; }
    int getDamage() const { return damage; }

    void launchTowards(const QPointF& startPos, const QPointF& targetPos);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void startExplosion();
    void checkCollisionWithPlayer(const QList<player*>& enemies);
    QList<player*> getEnemiesFromScene()const;

private slots:
    void updateFrame();
};


class Sword : public QGraphicsObject {
Q_OBJECT
private:
    int damage;
    QMovie* movie;
public:
    Sword(int damage, const QString& gifPath);
    ~Sword();

    void setDamage(int damage) { this->damage = damage; }
    int getDamage() const { return damage; }
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    void checkCollisionWithPlayer(const QList<player*>& enemies);
    QList<player*> getEnemiesFromScene()const;

private slots:
    void updateFrame();

};



class BossProjectile : public QGraphicsObject {
Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

private:
    QMovie* movie;
    int damage;
    bool hashit;
    QTimer* collisionTimer;
    player* targetPlayer;

public:
    BossProjectile(int damage, const QString& gifPath);
    void launch(const QPointF& start, const QPointF& end,player* p);
    void checkCollisionWithPlayer(player* p);
    int getDamage();
    bool isWall(QGraphicsItem* item);

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;
    ~BossProjectile();
protected:
    void advance(int phase) override;
};


#endif //PROJECT_CPP_PROJECTILES_H

