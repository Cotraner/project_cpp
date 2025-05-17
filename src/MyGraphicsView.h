#ifndef PROJECT_CPP_MYGRAPHICSVIEW_H
#define PROJECT_CPP_MYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>

class MyGraphicsView : public QGraphicsView {
Q_OBJECT
public:
    explicit MyGraphicsView(QWidget* parent = nullptr)
            : QGraphicsView(parent)
    {
        setRenderHint(QPainter::Antialiasing);
        setRenderHint(QPainter::SmoothPixmapTransform);
        setTransformationAnchor(AnchorViewCenter);
        setResizeAnchor(AnchorViewCenter);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }

protected:
    void wheelEvent(QWheelEvent* event) override {
        return; // desactive le scroll
    }
};

#endif //PROJECT_CPP_MYGRAPHICSVIEW_H
