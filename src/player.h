#ifndef PROJECT_CPP_PLAYER_H
#define PROJECT_CPP_PLAYER_H

#include <QObject>
#include <QGraphicsObject>
#include <QGraphicsPixmapItem>
#include <QMovie>
#include <QLabel>
#include "QPainter"

class player : public QGraphicsObject {
    Q_OBJECT

private:
    int life;
    int damage;
    QPixmap pixmap;
    QMovie* currentMovie;
    QMap<QString, QMovie*> movies; // Map des animations par action
    char type;
    int score;
    QLabel* scoreLabel = nullptr;
    void updateScoreLabel() {
        if (scoreLabel) {
            scoreLabel->setText(QString("SCORE : %1").arg(score));
        }
    }



public:
    player(int life,char type);
    ~player();

    int getLife();
    void setLife(int newLife);
    void setAnimation(const QString& direction);
    bool isDying = false;

    void setDamage(int value) { damage = value; }
    int getDamage() const { return damage; }


    player* getPlayer() {
        return this;
    }
    void setScore(int score) {
        this->score = score;
        emit scoreChanged(score);
    }
    int getScore() {
        return this->score;
    }
    void setPos(qreal x, qreal y) {
        QGraphicsItem::setPos(x, y);
        emit positionChanged(this);  // Émet le signal avec le joueur en paramètre
    }

    void setPos(const QPointF &pos) {
        QGraphicsItem::setPos(pos);
        emit positionChanged(this);  // Émet le signal avec le joueur en paramètre
    }
    void setType(char type) {
        this->type = type;
    }

    char getType() {
        return this->type;
    }

    void setScoreLabel(QLabel* label) {
        scoreLabel = label;
        updateScoreLabel();  // pour afficher le leaderboard initial
    }

    void addPoints(int pts) {
        score += pts;
        updateScoreLabel();
    }

    int getScore() const {
        return score;
    }

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;
    void damaged(int newLife);
    void playDeathAnimationForMainPlayer();


    signals:
    void positionChanged(player* playerCharacter);
    void lifeChanged(int newLife);
    void died();
    void enemyKilled(int points);
    void scoreChanged(int);
    void bossDefeated();

};

#endif //PROJECT_CPP_PLAYER_H
