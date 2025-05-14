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
#include "GameManager.h"
#include "Player.h"
#include "Entities.h"

// Déclaration anticipée
class GameManager;
class Player;
class Enemy;


class MyScene : public QGraphicsScene {
    Q_OBJECT

public:
    MyScene(QObject* parent = nullptr);
    virtual ~MyScene();

    Player* getPlayer() const;
    void loadMap();
    void updateHUD(); // Met à jour l'interface utilisateur (score, timer, barre de vie)
    void addScore(int points); // Ajoute des points au score

    GameManager* getGameManager() const { return gameManager; }


protected:
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private slots:
    void update();

private:
    QTimer* timer;
    GameManager* gameManager;
    Player* player;
    Enemy* enemy;
    QPixmap pixBackground;


    //TEST HUD 
    QGraphicsTextItem* scoreText;
    QGraphicsTextItem* timerText;
    QGraphicsRectItem* healthBarBack;
    QGraphicsRectItem* healthBarFront;
    QTimer* gameTimer;
    int elapsedTime = 0;
    int score = 0;
    

    int backgroundWidth = 0;
    int backgroundHeight = 0;

};


#endif //CPP_QT_TPMINIPROJET_MYSCENE_H
