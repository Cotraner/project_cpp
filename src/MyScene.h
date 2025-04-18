#ifndef CPP_QT_TPMINIPROJET_MYSCENE_H
#define CPP_QT_TPMINIPROJET_MYSCENE_H

#include <QGraphicsScene>


class MyScene : public QGraphicsScene {
    Q_OBJECT

public:
    MyScene(QObject* parent = nullptr);
    virtual ~MyScene();
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

    public slots:
    void update();

private:
    QGraphicsTextItem* qgti;
    QGraphicsPixmapItem* map;
    QGraphicsRectItem* qgri;
    QTimer* timer;
};


#endif //CPP_QT_TPMINIPROJET_MYSCENE_H
