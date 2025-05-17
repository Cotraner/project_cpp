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

private slots:
    void updatePlayerFocus(player* p);
    void slot_aboutMenu();
    void onGameOver();

private:
    void focusOnPlayer(player* playerCharacter, double zoomLevel = 1.0);

    MyScene* mainScene;
    MyGraphicsView* mainView;
    QMenu* helpMenu;
};


#endif //CPP_QT_TPMINIPROJET_MAINWINDOW_H
