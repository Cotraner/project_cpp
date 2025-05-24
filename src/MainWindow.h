#ifndef CPP_QT_TPMINIPROJET_MAINWINDOW_H
#define CPP_QT_TPMINIPROJET_MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
#include "MyScene.h"
#include "overlay.h"
#include "MyGraphicsView.h"
#include "player.h"

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;
    void setupMenu();
    void startGame();
    void showStartMenu();
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void updatePlayerFocus(player* p);
    void slot_aboutMenu();
    void onGameOver();
    void showScoreboard();
    void restartGame();
    void restartWithoutResetScore();

private:
    void focusOnPlayer(player* playerCharacter, double zoomLevel = 1.0);
    MyScene* mainScene;
    MyGraphicsView* mainView;
    MyScene* menuScene;
    QMenu* helpMenu;
    QWidget* startMenu = nullptr;
    QPushButton* startButton = nullptr;
    QPushButton* rulesButton = nullptr;
    QPushButton* scoreButton = nullptr;
    QLabel* titleLabel = nullptr;
    QLabel* scoreLabel;
    QLabel* rulesText;
    QPushButton* closeRulesButton;
    LifeCircle* life;
};


#endif //CPP_QT_TPMINIPROJET_MAINWINDOW_H
