#ifndef PROJECT_CPP_OVERLAY_H
#define PROJECT_CPP_OVERLAY_H
#include <QWidget>
#include <QObject>
#include <QBoxLayout>
#include <QTimer>
#include <QGraphicsTextItem>
#include <QLabel>

class LifeCircle : public QWidget {
Q_OBJECT

public:
    LifeCircle(QWidget* parent = nullptr);

    void setMaxHP(int value);
    int getMaxHP() const;
    QMovie* hearth;
    bool showDamageFlash = false;
    public slots:
    void setHP(int value);


protected:
    void paintEvent(QPaintEvent* event) override;

private:
    int hp = 100;
    int maxHp = 100;
};

class Score : public QGraphicsTextItem {
Q_OBJECT

public:
    Score(QGraphicsTextItem* parent = nullptr);
    void setScore(int value);
    int getScore() const { return score; }

private:
    int score;
};





#endif //PROJECT_CPP_OVERLAY_H
