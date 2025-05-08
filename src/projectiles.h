#ifndef PROJECT_CPP_PROJECTILES_H
#define PROJECT_CPP_PROJECTILES_H

#include <QObject>
#include <QGraphicsObject>
#include <QMovie>

class Molotov : public QGraphicsObject {
Q_OBJECT

private:
    int damage;
    QMovie* movie;

public:
    Molotov(int damage, const QString& gifPath);
    ~Molotov();

    void setDamage(int damage) { this->damage = damage; }
    int getDamage() const { return damage; }

    void launchTowards(const QPointF& startPos, const QPointF& targetPos);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private slots:
    void updateFrame();
};

#endif //PROJECT_CPP_PROJECTILES_H

