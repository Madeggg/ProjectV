#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
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

protected:
    void keyPressEvent(QKeyEvent* event);
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private slots:
    void update();

private:
    QTimer* timer;
    GameManager* gameManager;
    Player* player;
    QPixmap pixBackground;
    

    int backgroundWidth = 0;
    int backgroundHeight = 0;

};


#endif //CPP_QT_TPMINIPROJET_MYSCENE_H
