#ifndef CPP_QT_TPMINIPROJET_MYSCENE_H
#define CPP_QT_TPMINIPROJET_MYSCENE_H

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include<QGraphicsPixMapItem>
#include<QPixMap>
#include<QPainter>
#include<QRectF>
#include <QTimer>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QDebug>
#include "GameManager.h"
#include "Player.h"
#include "Entities.h"


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
    QPixMap pixBackground;
    

};


#endif //CPP_QT_TPMINIPROJET_MYSCENE_H
