#ifndef PROJECT_CPP_OVERLAY_H
#define PROJECT_CPP_OVERLAY_H
#include <QWidget>
#include <QObject>
#include <QBoxLayout>

class LifeCircle : public QWidget {
Q_OBJECT

public:
    LifeCircle(QWidget* parent = nullptr);
    void setHP(int value);
    void setMaxHP(int value);
    int getMaxHP() const;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    int hp = 100;
    int maxHp = 100;
};
//class Overlay : public QWidget {
//    Q_OBJECT
//public:
//    Overlay(QWidget* parent = nullptr) : QWidget(parent) {
//        setAttribute(Qt::WA_TransparentForMouseEvents);
//      setAttribute(Qt::WA_NoSystemBackground);


#endif //PROJECT_CPP_OVERLAY_H
