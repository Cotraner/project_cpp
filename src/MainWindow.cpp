#include "MainWindow.h"
#include "MyScene.h"
#include "QGraphicsProxyWidget"
#include <QPushButton>
#include <QTimer>
#include <QWheelEvent>
#include <qfontdatabase.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), mainScene(new MyScene(this)), mainView(new MyGraphicsView(this)){

    mainView->setScene(mainScene);
    setCentralWidget(mainView);

    auto player = mainScene->getPlayer();
    if (player) {
        connect(mainScene->getPlayer(), SIGNAL(positionChanged(player*)), this, SLOT(updatePlayerFocus(player*)));
        this->focusOnPlayer(player, 2.0);
    } else {
        qDebug() << "Le joueur n'a pas été initialisé dans la scène !";
    }

    // Configurer la fenêtre principale
    this->setCentralWidget(this->mainView);

    // Le viewport est le widget où la scène est effectivement dessinée
    LifeCircle* life = new LifeCircle(this->mainView);
    life->setHP(this->mainScene->getPlayer()->getLife());
    life->setMaxHP(life->getMaxHP());
    life->show();
    // Connecter la mise à jour de vie du joueur au cercle
    connect(mainScene->getPlayer(), SIGNAL(lifeChanged(int)), life, SLOT(setHP(int)));

    // Appliquer un délai pour que la vue comprenne la taille de la scène
    QTimer::singleShot(0, this, [=]() {
        this->focusOnPlayer(player, 2.0);
    });
    this->setWindowTitle("My game");
    this->resize(800, 600);

//show menu
    helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction* actionHelp = new QAction(tr("&About"), this);
    connect(actionHelp, SIGNAL(triggered()), this, SLOT(slot_aboutMenu()));
    helpMenu->addAction(actionHelp);

    //Game Over
    connect(mainScene, &MyScene::gameOver, this, &MainWindow::onGameOver);

}

void MainWindow::focusOnPlayer(player* playerCharacter, double zoomLevel)
{
    if (!playerCharacter || !mainView)
        return;
    // Réinitialiser toute transformation précédente
    mainView->resetTransform();
    // Appliquer le niveau de zoom
    mainView->scale(zoomLevel, zoomLevel);
    // Centrer la vue sur le joueur
    mainView->centerOn(playerCharacter);
}

void MainWindow::updatePlayerFocus(player* p){
    mainView->centerOn(p);
}

void MainWindow::slot_aboutMenu(){
    QMessageBox msgBox;
    msgBox.setText("A small QT/C++ projet...");
    msgBox.setModal(true); // on souhaite que la fenetre soit modale i.e qu'on ne puisse plus cliquer ailleurs
    msgBox.exec();
}

void MainWindow::onGameOver() {
    QWidget* overlay = new QWidget(mainView);
    overlay->setStyleSheet("background-color: rgba(0, 0, 0, 150);");
    overlay->setAttribute(Qt::WA_TransparentForMouseEvents);
    overlay->setGeometry(mainView->viewport()->geometry());

    overlay->show();

    QGraphicsTextItem* dieMsg = new QGraphicsTextItem("Game Over");
    dieMsg->setDefaultTextColor(Qt::red);
    // Charger la police "Game Over"
    int fontId = QFontDatabase::addApplicationFont("../fonts/game_over.ttf");
    if (fontId != -1) {
        QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
        QFont gameOverFont(family, 90); // Taille 30
        dieMsg->setFont(gameOverFont);
    } else {
        qWarning() << "Échec du chargement de la police Game Over.";
        dieMsg->setFont(QFont("Arial", 30)); // Police de secours
    }
    dieMsg->setZValue(1000);

    // Positionner le texte au centre de la vue
    QSize viewSize = mainView->viewport()->size();
    QPointF center = mainView->mapToScene(viewSize.width() / 2, viewSize.height() / 2);
    dieMsg->setPos(center - QPointF(dieMsg->boundingRect().width() / 2,dieMsg->boundingRect().height() / 3));

    mainScene->addItem(dieMsg);
}

MainWindow::~MainWindow(){

}