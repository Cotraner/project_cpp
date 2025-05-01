#include "MainWindow.h"

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
    connect(mainScene->getPlayer(), SIGNAL(positionChanged()), this, SLOT(updatePlayerFocus()));
    this->focusOnPlayer(mainScene->getPlayer(), 2.0);
    this->setCursor(Qt::BlankCursor);


    this->setCentralWidget(mainView);
    this->setWindowTitle("My game");
    this->resize(1920/2, 1080/2);

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

void MainWindow::updatePlayerFocus()
{
    // Récupérer le joueur depuis l'émetteur du signal
    player* playerCharacter = qobject_cast<player*>(sender());
    if (playerCharacter) {
        // Juste centrer, sans modifier le niveau de zoom
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