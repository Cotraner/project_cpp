#ifndef CPP_QT_TPMINIPROJET_MYSCENE_H
#define CPP_QT_TPMINIPROJET_MYSCENE_H
#include <QGraphicsScene>
#include "player.h"
#include <QSet>
#include <QTimer>

class MyScene : public QGraphicsScene {
    Q_OBJECT

public:
    MyScene(QObject* parent = nullptr);
    virtual ~MyScene();
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void createMap();
    void createPersonage();
    player* getPlayer();
    QTimer* timer;
    QSet<int> pressedKeys;
    QTimer* movementTimer;
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    QPointF reduceLengthAttack(const QPointF& playerPos, const QPointF& targetPos);
    void showGameOverMessage();
    void createEnnemies(int x1, int y1,int x2,int y2,int x3,int y3,int x4,int y4,int x5,int y5);
    void addToList(QList<player*>& entities, player* p,player* p1, player* p2, player* p3, player* p4, player* p5);
    void setLookingDirection(char direction);
    char getLookingDirection() const { return lookingDirection; }
    QList<player*> getEnemies()const;
    void removeEnemy(player* enemy);
    void moveEnemies();
    void start();
    void spawnBossEnemy();
    bool areAllBasicEnemiesDead() const;
    void spawnBossProjectiles(player* boss);
    void handleBossAttacks();
    void ensureBossVisible();
    void startInfiniteWaveMode();

    signals:
    void gameOver();


    public slots:
    void update();



private slots:
    void Movement();
    void handlePlayerDeath();

private:
    QGraphicsPixmapItem* map;
    QTimer* enemyTimer;
    int backgroundWidth;
    int backgroundHeight;
    QList<player*> entities;
    //all entity
    player* personage;
    player* enemy1;
    player* enemy2;
    player* enemy3;
    player* enemy4;
    player* enemy5;
    player* boss;

    QList<QGraphicsItem*> collisionItems;
    bool checkCollision(QPointF newPos);
    bool isGameActive = true;
    char lookingDirection = 'b'; // Direction par défaut
    bool bossSpawned = false;
    QTimer* bossAttackTimer = nullptr;
};



#endif //CPP_QT_TPMINIPROJET_MYSCENE_H
