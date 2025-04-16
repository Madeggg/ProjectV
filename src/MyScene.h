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
#include <QMouseEvent>
#include <QDebug>
#include "GameManager.h"
#include "Player.h"
#include "Entities.h"

// Déclaration anticipée
class GameManager;
class Player;


class MyScene : public QGraphicsScene {
    Q_OBJECT

public:
    MyScene(QObject* parent = nullptr);
    virtual ~MyScene();

protected:
    void keyPressEvent(QKeyEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void drawBackground(QPainter* painter, const QRectF& rect);

private slots:
    void update();

private:
    QTimer* timer;
    QGraphicsTextItem* qgti;
    QGraphicsRectItem* qgri;
    GameManager* gameManager;
    Player* player;
    QPixmap pixBackground;
    

};


#endif //CPP_QT_TPMINIPROJET_MYSCENE_H
