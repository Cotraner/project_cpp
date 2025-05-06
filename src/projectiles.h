#ifndef PROJECT_CPP_PROJECTILES_H
#define PROJECT_CPP_PROJECTILES_H
#include <QObject>
#include <QGraphicsObject>

class Molotov : public QGraphicsObject {
    Q_OBJECT
private:
    int damage;
    int speed;
    QPixmap pixmap;

};
#endif //PROJECT_CPP_PROJECTILES_H
