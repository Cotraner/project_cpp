#include "overlay.h"
#include <QPainter>
#include <QPen>
#include <QMovie>
#include <QGraphicsTextItem>
#include <QFontDatabase>


LifeCircle::LifeCircle(QWidget* parent) : QWidget(parent) {
    this->setFixedSize(100, 100);
    this->setAttribute(Qt::WA_TransparentForMouseEvents);
    hearth = new QMovie("../anim/hearth.gif");
    hearth->start();
}

void LifeCircle::setHP(int value) {
    hp = value;
    showDamageFlash = true;
    update(); //dessiner le flash

    QTimer::singleShot(100, this, [this]() {
        showDamageFlash = false;
        update(); //enleve le flash
    });
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

    QRectF rect(10, 10, width() - 20, height() - 20);

    if (showDamageFlash) { //dessine la prise de dégats si true
        QBrush redFlash(QColor(255, 0, 0, 100)); // Rouge translucide
        painter.setBrush(redFlash);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(rect); // Ou drawRect(rect) si tu veux tout le widget en rouge
    }

    QPen pen(Qt::red, 8);
    painter.setPen(pen);

    float percent = static_cast<float>(hp) / maxHp;
    int spanAngle = static_cast<int>(360.0 * percent * 16);

    painter.drawArc(rect, 90 * 16, -spanAngle);

    // Centre du rectangle
    QPixmap frame = hearth->currentPixmap();
    QPoint center = rect.center().toPoint();
    QPoint topLeft(center.x() - frame.width() / 2, center.y() - frame.height() / 2);
    painter.drawPixmap(topLeft, frame);
}

Score::Score(QGraphicsTextItem* parent) :  score(0) {
    QGraphicsTextItem* score = new QGraphicsTextItem("SCORE :");     //ajout d'un texte
    int fontId = QFontDatabase::addApplicationFont("../fonts/game_over.ttf");
    QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont gameOverFont(family, 40);
    setFont(gameOverFont);
    setDefaultTextColor(Qt::white);
}

void Score::setScore(int value) {
    score = value;

}
