#ifndef PROJECT_CPP_MAINMENU_H
#define PROJECT_CPP_MAINMENU_H

#include <QGraphicsScene>
#include <QPushButton>
#include <QGraphicsProxyWidget>

class MainMenuScene : public QGraphicsScene {
Q_OBJECT
public:
    explicit MainMenuScene(QObject* parent = nullptr);

signals:
    void startGame();  // signal pour démarrer le jeu
};

#endif //PROJECT_CPP_MAINMENU_H
