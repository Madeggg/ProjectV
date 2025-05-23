#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QPainter>
#include <QRectF>
#include <QTimer>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QSoundEffect>
#include <QRandomGenerator>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include "GameManager.h"
#include "Player.h"
#include "Entities.h"
#include "HUD.h"

// Déclaration anticipée
class GameManager;
class Player;
class HUD;
class Enemy;


class MyScene : public QGraphicsScene {
    Q_OBJECT

public:
    MyScene(QObject* parent = nullptr);
    virtual ~MyScene();

    Player* getPlayer() const;
    void loadMap();
    void spawnAmmoBox(); // Fait apparaître une boîte de munitions
    void spawnWeapon(); // Fait apparaître une arme
    void addProjectile(QPointF targetPos);

    //Pour les armes qui tirent en direction fixe (shotgun)
    void addProjectileDir(QPointF direction, int speed, int damage, int maxDistance);

    //Sound effects
    QSoundEffect* knifeSounds[4];
    QSoundEffect* moveSound;

    //pause

    void showPauseOverlay();
    void hidePauseOverlay();
    void setHUD(HUD* hudWidget);

   
    QTimer* timer;




    GameManager* getGameManager() const { return gameManager; }

protected:
    void keyPressEvent(QKeyEvent* event);
   
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private slots:
    void update();

private:
    GameManager* gameManager;
    Player* player;
    Enemy* enemy;
    QPixmap pixBackground;
    bool pistolSpawned = false;
    bool shotgunSpawned = false;
    bool ammoBoxSpawned = false;

     //Pause 
    QGraphicsRectItem* pauseOverlay = nullptr;
    QGraphicsProxyWidget* pauseProxy = nullptr;
    QPushButton* resumeButton = nullptr;
    HUD* hud = nullptr;

    int elapsedSeconds;     



    

    int backgroundWidth = 0;
    int backgroundHeight = 0;


};


#endif //CPP_QT_TPMINIPROJET_MYSCENE_H
