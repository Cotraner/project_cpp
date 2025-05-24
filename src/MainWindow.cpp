#include "MainWindow.h"
#include "MyScene.h"
#include "QGraphicsProxyWidget"
#include <QPushButton>
#include <QTimer>
#include <QWheelEvent>
#include <qfontdatabase.h>
#include <QLabel>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), mainScene(new MyScene(this)), mainView(new MyGraphicsView(this)){
    // Configurer la fenêtre principale
    this->setCentralWidget(this->mainView);
    this->setWindowTitle("Albert's Adventure");
    this->resize(1000, 800);
    //la font de base
    int fontId = QFontDatabase::addApplicationFont("../fonts/game_over.ttf");


    showStartMenu(); // Afficher le menu d'accueil au lancement
    mainView->setScene(mainScene);
    setCentralWidget(mainView);

    // Créer un label pour le leaderboard
    scoreLabel = new QLabel(this->mainView);
    QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont gameOverFont(family, 40);
    scoreLabel->setFont(gameOverFont);
    scoreLabel->setStyleSheet("color: white;");
    scoreLabel->setGeometry(880, 10, 200, 50);



    connect(rulesButton, &QPushButton::clicked, this, [this]() {
        startButton->hide();
        rulesButton->hide();
        scoreButton->hide();
        titleLabel->hide();
        rulesText->show();
        closeRulesButton->show();
    });

    connect(closeRulesButton, &QPushButton::clicked, this, [this]() {
        rulesText->hide();
        closeRulesButton->hide();
        titleLabel->show();
        startButton->show();
        rulesButton->show();
        scoreButton->show();
    });
    connect(scoreButton, &QPushButton::clicked, this, &MainWindow::showScoreboard);

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
    life = new LifeCircle(this->mainView);
    life->setHP(this->mainScene->getPlayer()->getLife());
    life->setMaxHP(life->getMaxHP());
    life->show();
    // Connecter la mise à jour de vie du joueur au cercle
    connect(mainScene->getPlayer(), SIGNAL(lifeChanged(int)), life, SLOT(setHP(int)));

    // Connecter la mise à jour du leaderboard du joueur au label
    connect(mainScene->getPlayer(), SIGNAL(scoreChanged(int)), scoreLabel, SLOT(setScore(int)));

    // Appliquer un délai pour que la vue comprenne la taille de la scène
    QTimer::singleShot(0, this, [=]() {
        this->focusOnPlayer(player, 3.5);
    });

    this->mainScene->getPlayer()->setScoreLabel(scoreLabel);

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
    //overlay->setAttribute(Qt::WA_TransparentForMouseEvents);
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

    int newScore = mainScene->getPlayer()->getScore();
    QList<int> scores;

// Lire les scores existants
    QFile file("../leaderboard/leaderboard.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (!line.isEmpty()) {
                scores.append(line.toInt());
            }
        }
        file.close();
    }

// Ajouter le nouveau leaderboard
    scores.append(newScore);

    std::sort(scores.begin(), scores.end(), std::greater<int>());

// Réécrire dans le fichier
    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QTextStream out(&file);
        for (int score : scores) {
            out << score << "\n";
        }
        file.close();
    }

    // Bouton "Recommencer"
    QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont gameOverFont(family, 90);
    QPushButton* restartButton = new QPushButton("RESTART", overlay);
    restartButton->setFixedSize(200, 60);
    restartButton->move(viewSize.width() / 2 - 100, viewSize.height() / 2 + 100);
    restartButton->setFont(gameOverFont);
    restartButton->setStyleSheet("font-size: 30px; background-color: #27ae60; color: white; border-radius: 10px;");
    restartButton->show();

    connect(restartButton, &QPushButton::clicked, this, [=]() {
        overlay->deleteLater();
        dieMsg->scene()->removeItem(dieMsg);
        delete dieMsg;

        // 🔽 Enregistrer le score même si ce n’est pas un Game Over
        int newScore = mainScene->getPlayer()->getScore();
        QList<int> scores;

        QFile file("../leaderboard/leaderboard.txt");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            while (!in.atEnd()) {
                QString line = in.readLine().trimmed();
                if (!line.isEmpty())
                    scores.append(line.toInt());
            }
            file.close();
        }

        scores.append(newScore);
        QSet<int> uniqueScores = QSet<int>(scores.begin(), scores.end());
        scores = uniqueScores.values();
        std::sort(scores.begin(), scores.end(), std::greater<int>());

        if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
            QTextStream out(&file);
            for (int score : scores)
                out << score << "\n";
            file.close();
        }
        restartGame();
    });

}

void MainWindow::restartGame() {
    qDebug() << "Recommencer cliqué";

    // 🔽 1. Sauvegarder le score actuel
    int newScore = mainScene->getPlayer()->getScore();
    QList<int> scores;

    QFile file("../leaderboard/leaderboard.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (!line.isEmpty())
                scores.append(line.toInt());
        }
        file.close();
    }

    scores.append(newScore);  // ✅ Garde les doublons
    std::sort(scores.begin(), scores.end(), std::greater<int>());

    while (scores.size() > 10)
        scores.removeLast();

    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QTextStream out(&file);
        for (int score : scores)
            out << score << "\n";
        file.close();
    }

    // Supprimer l'ancienne scène
    mainScene->deleteLater();


    mainScene = new MyScene(this);
    mainView->setScene(mainScene);


    connect(mainScene, &MyScene::gameOver, this, &MainWindow::onGameOver);

    auto player = mainScene->getPlayer();
    if (player) {
        player->setScore(0);


        player->setScoreLabel(scoreLabel);
        if (!scoreLabel) {
            qWarning() << "ERREUR: scoreLabel est null ! Impossible de connecter.";
        } else {
            connect(player, &player::scoreChanged, this, [=](int newScore) {
                scoreLabel->setText("SCORE : " + QString::number(newScore));
            });
        }


        connect(player, SIGNAL(positionChanged(player*)), this, SLOT(updatePlayerFocus(player*)));
        connect(player, SIGNAL(lifeChanged(int)), life, SLOT(setHP(int)));

        life->setHP(player->getLife());
    }

    mainScene->start();
    mainView->setFocus();
    player->setFocus();
    focusOnPlayer(player, 2.0);
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

    // Texte des règles
    rulesText = new QLabel("Des ennemis sont répartis tout autour de la map, a vous de les débusquer pour que le boss apparaisse ! \n Commandes : \n - left Click : Sword attack\n - right Click : Molotov \n -Z/Up : Go up\n - Q/left : Go left\n - S/down : Go down\n - D/right : Go right", startMenu);
    rulesText->setStyleSheet("color: white; font-size: 60px;");
    rulesText->setFont(gameOverFont);
    rulesText->setAlignment(Qt::AlignCenter);
    rulesText->setWordWrap(true);
    rulesText->setGeometry(startMenu->width() * 0.1, startMenu->height() * 0.3,
                           startMenu->width() * 0.8, startMenu->height() * 0.4);
    rulesText->hide();

// Bouton de fermeture des règles
    closeRulesButton = new QPushButton("X", startMenu);
    closeRulesButton->setFixedSize(40, 40);
    closeRulesButton->move(startMenu->width() - 60, 20);
    closeRulesButton->setStyleSheet("background-color: transparent; color: white; font-weight: bold;");
    closeRulesButton->hide();

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

void MainWindow::showScoreboard() {
    titleLabel->hide();
    startButton->hide();
    rulesButton->hide();
    scoreButton->hide();
    int fontId = QFontDatabase::addApplicationFont("../fonts/game_over.ttf");
    QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont gameOverFont(family, 40);

    // Création du widget texte si pas déjà fait
    QLabel* scoreboardLabel = new QLabel(startMenu);
    scoreboardLabel->setStyleSheet("color: white; font-size: 40px;");
    scoreboardLabel->setFont(gameOverFont);
    scoreboardLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    scoreboardLabel->setWordWrap(true);

    // Lire et formater le fichier
    QFile file("../leaderboard/leaderboard.txt");
    QString text;
    int index = 1;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (!line.isEmpty()) {
                text += QString::number(index++) + ": " + line + "\n";
            }
        }
        file.close();
    } else {
        text = "Impossible d'ouvrir le fichier des scores.";
    }

    scoreboardLabel->setText(text);
    scoreboardLabel->setGeometry(startMenu->width() * 0.2, startMenu->height() * 0.2,
                                 startMenu->width() * 0.6, startMenu->height() * 0.5);
    scoreboardLabel->show();

    // Bouton retour
    QPushButton* backButton = new QPushButton("Retour", startMenu);
    backButton->setFixedSize(150, 50);
    backButton->move(startMenu->width() - 200, startMenu->height() - 80);
    backButton->setFont(gameOverFont);
    backButton->setStyleSheet("font-size: 30px; background-color: #f1c40f; color: black; border-radius: 10px;");
    backButton->show();

    connect(backButton, &QPushButton::clicked, this, [=]() {
        scoreboardLabel->deleteLater();
        backButton->deleteLater();
        titleLabel->show();
        startButton->show();
        rulesButton->show();
        scoreButton->show();
    });
}

void MainWindow::restartWithoutResetScore() {
    qDebug() << "Redémarrage sans reset du score ni de la vie";

    // Sauvegarder score et vie actuels
    int currentScore = mainScene->getPlayer()->getScore();
    int currentHP = mainScene->getPlayer()->getLife();

    // Supprimer l'ancienne scène
    mainScene->deleteLater();

    // Nouvelle scène
    mainScene = new MyScene(this);
    mainView->setScene(mainScene);
    connect(mainScene, &MyScene::gameOver, this, &MainWindow::onGameOver);
    mainScene->start();

    auto player = mainScene->getPlayer();
    if (player) {
        // ✅ Restaurer score et vie
        player->setScore(currentScore);
        player->setLife(currentHP);  // ⚠️ Cela ne doit pas relancer l'animation de mort

        player->setScoreLabel(scoreLabel);

        if (!scoreLabel) {
            qWarning() << "ERREUR: scoreLabel est null ! Impossible de connecter.";
        } else {
            connect(player, &player::scoreChanged, this, [=](int newScore) {
                scoreLabel->setText("SCORE : " + QString::number(newScore));
            });
        }

        connect(player, SIGNAL(positionChanged(player*)), this, SLOT(updatePlayerFocus(player*)));
        connect(player, SIGNAL(lifeChanged(int)), life, SLOT(setHP(int)));
        connect(player, &player::bossDefeated, this, &MainWindow::restartWithoutResetScore);

        life->setHP(currentHP);
    }

    mainView->setFocus();
    player->setFocus();
    focusOnPlayer(player, 2.0);
}






MainWindow::~MainWindow(){

}