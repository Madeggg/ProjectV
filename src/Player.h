// Cette page est la afin de gérer le Personnage principal, et les controles par le joueur
#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QKeyEvent>
#include <QTimer>

#include "GameManager.h"
#include "Entities.h"

// Déclaration anticipée
class MyScene;



class Player : public QObject, public QGraphicsPixmapItem { //QGraphicsPixmapItem, sous-classe de QGraphicsItem, affiche une image (pixmap) dans une scène graphique
    Q_OBJECT

public:
    Player(QGraphicsItem* parent = nullptr); //constructeur de la classe, cet objet devient le parent de notre Player

    //Méthodes de déplacement 
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    // Méthodes de gestion de PV
    int getHealth() const;
    void setHealth(int newHealth);
    void takeDamage(int amount);

private:
    int health;
    int speed;
};

#endif 