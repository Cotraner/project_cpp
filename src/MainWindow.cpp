#include "MainWindow.h"
#include "MyScene.h"
#include "QGraphicsProxyWidget"
#include <QPushButton>
#include <QTimer>
#include <QWheelEvent>
#include <qfontdatabase.h>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), mainScene(new MyScene(this)), mainView(new MyGraphicsView(this)){
    // Configurer la fenêtre principale
    this->setCentralWidget(this->mainView);
    this->setWindowTitle("Albert's Adventure");
    this->resize(1000, 800);

    showStartMenu(); // Afficher le menu d'accueil au lancement
    mainView->setScene(mainScene);
    setCentralWidget(mainView);



    //show menu
    helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction* actionHelp = new QAction(tr("&About"), this);
    connect(actionHelp, SIGNAL(triggered()), this, SLOT(slot_aboutMenu()));
    helpMenu->addAction(actionHelp);

    auto player = mainScene->getPlayer();
    if (player) {
        connect(mainScene->getPlayer(), SIGNAL(positionChanged(player*)), this, SLOT(updatePlayerFocus(player*)));
        this->focusOnPlayer(player, 2.0);
    } else {
        qDebug() << "Le joueur n'a pas été initialisé dans la scène !";
    }

    // Le viewport est le widget où la scène est effectivement dessinée
    LifeCircle* life = new LifeCircle(this->mainView);
    life->setHP(this->mainScene->getPlayer()->getLife());
    life->setMaxHP(life->getMaxHP());
    life->show();
    // Connecter la mise à jour de vie du joueur au cercle
    connect(mainScene->getPlayer(), SIGNAL(lifeChanged(int)), life, SLOT(setHP(int)));

    // Appliquer un délai pour que la vue comprenne la taille de la scène
    QTimer::singleShot(0, this, [=]() {
        this->focusOnPlayer(player, 3.5);
    });




    //Game Over
    connect(mainScene, &MyScene::gameOver, this, &MainWindow::onGameOver);
    mainScene->start();



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
        QFont gameOverFont(family, 90);
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

void MainWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
    if (startMenu)
        startMenu->resize(this->size());
}

void MainWindow::showStartMenu() {
    // Création du menu
    startMenu = new QWidget(this);
    startMenu->setGeometry(this->rect()); // assure que ça prend toute la fenêtre

// Utilise la taille de la fenêtre principale (MainWindow)
    QSize windowSize = this->size();

    QLabel* backgroundLabel = new QLabel(startMenu);
    QPixmap bgPixmap("../map/background_menu.png");
    backgroundLabel->setPixmap(bgPixmap.scaled(
            windowSize,
            Qt::IgnoreAspectRatio,  // ou Qt::KeepAspectRatioByExpanding selon ce que tu veux
            Qt::SmoothTransformation
    ));
    backgroundLabel->setGeometry(0, 0, windowSize.width(), windowSize.height());
    backgroundLabel->lower(); // fond derrière les boutons
    backgroundLabel->show();

    // Titre
    titleLabel = new QLabel("Albert's adventure", startMenu);
    int fontId = QFontDatabase::addApplicationFont("../fonts/game_over.ttf");
    QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont gameOverFont(family, 90);
    titleLabel->setFont(gameOverFont);
    titleLabel->setStyleSheet("color: white;");
    titleLabel->setAlignment(Qt::AlignCenter);

    // Bouton "Jouer"
    startButton = new QPushButton("JOUER", startMenu);
    startButton->setFixedSize(200, 60);
    startButton->setFont(gameOverFont);
    startButton->setStyleSheet("font-size: 60px; background-color: #2ecc71; color: white; border-radius: 10px;");
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startGame);

    // bouton règles
    rulesButton = new QPushButton("RULES", startMenu);
    rulesButton->setFixedSize(200, 60);
    rulesButton->setFont(gameOverFont);
    rulesButton->setStyleSheet("font-size: 60px; background-color: #3498db; color: white; border-radius: 10px;");

    // ScoreBoard
    scoreButton = new QPushButton("SCOREBOARD", startMenu);
    scoreButton->setFixedSize(200, 60);
    scoreButton->setFont(gameOverFont);
    scoreButton->setStyleSheet("font-size: 60px; background-color: #e74c3c; color: white; border-radius: 10px;");


    // Layout vertical
    QVBoxLayout* layout = new QVBoxLayout(startMenu);
    layout->addStretch();
    layout->addWidget(titleLabel, 0, Qt::AlignCenter);
    layout->addSpacing(30);
    layout->addWidget(startButton, 0, Qt::AlignCenter);
    layout->addSpacing(20); // petit espace entre les deux boutons
    layout->addWidget(rulesButton, 0, Qt::AlignCenter);
    layout->addSpacing(20);
    layout->addWidget(scoreButton, 0, Qt::AlignCenter);
    layout->addStretch();

    startMenu->setLayout(layout);

    startMenu->setLayout(layout);
    startMenu->show();

    // Masquer la vue tant que le menu est affiché
    mainView->hide();
}

void MainWindow::startGame() {
    // Démarrage du jeu
    if (startMenu) {
        startMenu->hide();
        startMenu->deleteLater();
        startMenu = nullptr;
    }
    mainView->setScene(mainScene);

    mainView->show();
    //focus le clavier a la fermeture du menu
    mainView->setFocus();
    mainScene->getPlayer()->setFocus();
    this->focusOnPlayer(mainScene->getPlayer(), 2.0); // recentrer sur joueur
}


MainWindow::~MainWindow(){

}