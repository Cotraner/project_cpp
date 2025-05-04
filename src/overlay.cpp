#include "overlay.h"
#include <QPainter>
#include <QPen>

LifeCircle::LifeCircle(QWidget* parent) : QWidget(parent) {
    this->setFixedSize(100, 100);
    this->setAttribute(Qt::WA_TransparentForMouseEvents);
}

void LifeCircle::setHP(int value) {
    hp = value;
    update();
}

void LifeCircle::setMaxHP(int value) {
    maxHp = value;
    update();
}

int LifeCircle::getMaxHP() const {
    return maxHp;
}

void LifeCircle::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRectF rect(10, 10, width() - 20, height() - 20); // espace autour

    QPen pen(Qt::red, 8);
    painter.setPen(pen);

    // angle dans Qt : 16*degrés (sens horaire depuis 3h)
    float percent = static_cast<float>(hp) / maxHp;
    int spanAngle = static_cast<int>(360.0 * percent * 16);

    painter.drawArc(rect, 90 * 16, -spanAngle); // commence à 12h, sens anti-horaire
}

