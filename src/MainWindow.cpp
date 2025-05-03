#include <QStackedLayout>
#include "MainWindow.h"
#include "overlay.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{

    this->mainScene = new MyScene;
    this->mainView = new QGraphicsView;
    this->mainView->setScene(mainScene);


    // Activer les transformations de vue
    this->mainView->setRenderHint(QPainter::Antialiasing);
    this->mainView->setRenderHint(QPainter::SmoothPixmapTransform);

    //config de zoom
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

    this->setCursor(Qt::BlankCursor);

    // afficher l'overlay


    this->setCentralWidget(mainView);
    this->setWindowTitle("My game");
    this->resize(800, 600);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction* actionHelp = new QAction(tr("&About"), this);
    connect(actionHelp, SIGNAL(triggered()), this, SLOT(slot_aboutMenu()));
    helpMenu->addAction(actionHelp);

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

void MainWindow::updatePlayerFocus(player* playerCharacter)
{
    if (playerCharacter) {
        mainView->centerOn(playerCharacter);

    }
}


MainWindow::~MainWindow(){

}

void MainWindow::slot_aboutMenu(){
    QMessageBox msgBox;
    msgBox.setText("A small QT/C++ projet...");
    msgBox.setModal(true); // on souhaite que la fenetre soit modale i.e qu'on ne puisse plus cliquer ailleurs
    msgBox.exec();
}