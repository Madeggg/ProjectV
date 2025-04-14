#ifndef CPP_QT_TPMINIPROJET_MYSCENE_H
#define CPP_QT_TPMINIPROJET_MYSCENE_H

#include <QGraphicsScene
#include "GameManager.h"
#include "Player.h"
#include "Entites.h"


class MyScene : public QGraphicsScene {
    Q_OBJECT

public:
    MyScene(QObject* parent = nullptr);
    virtual ~MyScene();

};


#endif //CPP_QT_TPMINIPROJET_MYSCENE_H
