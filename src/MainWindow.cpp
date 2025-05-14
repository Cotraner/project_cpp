#include "MainWindow.h"
#include "MyScene.h"
#include "QGraphicsProxyWidget"
#include <QPushButton>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->mainScene = new MyScene;
    this->mainView = new QGraphicsView;
    this->mainView->setScene(mainScene);

    // Configurer la scène
    this->mainView->setRenderHint(QPainter::Antialiasing);
    this->mainView->setRenderHint(QPainter::SmoothPixmapTransform);

    // zoom
    this->mainView->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    this->mainView->setResizeAnchor(QGraphicsView::AnchorViewCenter);

    // Désactiver le scrolling
    this->mainView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->mainView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    auto player = mainScene->getPlayer();
    if (player) {
        connect(mainScene->getPlayer(), SIGNAL(positionChanged(player*)), this, SLOT(updatePlayerFocus(player*)));
        this->focusOnPlayer(player, 2.0);
    } else {
        qDebug() << "Le joueur n'a pas été initialisé dans la scène !";
    }
    //this->setCursor(Qt::BlankCursor);

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


MainWindow::~MainWindow(){

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
    dieMsg->setFont(QFont("Arial", 30));

    // Positionner le texte au centre de la vue
    QSize viewSize = mainView->viewport()->size();
    QPointF center = mainView->mapToScene(viewSize.width() / 2, viewSize.height() / 2);
    dieMsg->setPos(center - QPointF(dieMsg->boundingRect().width() / 2,dieMsg->boundingRect().height() / 3));

    mainScene->addItem(dieMsg);

}