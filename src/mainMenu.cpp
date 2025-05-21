#include "mainMenu.h"
#include <QGraphicsTextItem>
#include <QFont>
#include <QGraphicsView>
#include <QFontDatabase>

MainMenuScene::MainMenuScene(QObject* parent) : QGraphicsScene(parent) {
    setSceneRect(0, 0, 1000, 800);  // taille du menu

    auto title = new QGraphicsTextItem("Mon Jeu RPG");
    title->setDefaultTextColor(Qt::white);
    int fontId = QFontDatabase::addApplicationFont("../fonts/game_over.ttf");
    if (fontId != -1) {
        QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
        QFont gameOverFont(family, 60);
    title->setFont(gameOverFont);
    } else {
        qWarning() << "Échec du chargement de la police Game Over.";
        title->setFont(QFont("Arial", 30)); // Police de secours
    }
    title->setPos(250, 100);
    addItem(title);

    // Créer un bouton "Démarrer"
    QPushButton* startButton = new QPushButton("Jouer");
    QGraphicsProxyWidget* proxyStart = addWidget(startButton);
    proxyStart->setPos(350, 300);
    connect(startButton, &QPushButton::clicked, this, &MainMenuScene::startGame);
}
