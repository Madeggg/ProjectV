// Cette page sert à tout ce qui est hostile (enemies, bombes etc..) + les projectiles tirés par joueur ou ennemis + les armes

#ifndef ENTITIES_H
#define ENTITIES_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>   
#include <QString>

#include "GameManager.h"
#include "Player.h"
#include "MyScene.h"

class Player;
Class Weapon;


class Enemy : public QObject{
    Q_OBJECT
protected:
    int health;
    int damage;
    int speed;
    bool distance; //Booléen pour savoir si l'ennemi attaque le joueur à distance ou pas
    Weapon* weapon;
    QString* type;
    QPixmap* sprite_up;
    QPixmap* sprite_down;
    QPixmap* sprite_right;
    QPixmap* sprite_left;
    QTimer* damageTimer; // Timer pour gérer l'intervalle de temps entre les dégâts infligés au joueur

public:
    //Constructeur
    Enemy(QGraphicsItem* parent = nullptr); 
    //Accesseurs
    int getHealth() const;
    int getDamage() const;
    int getSpeed() const;
    QString* getType() const; // Getter pour le type d'ennemi
    //Mutateurs
    void setHealth(int newHealth);
    void setDamage(int newDamage);
    void setSpeed(int newSpeed);
    void setWeapon(Weapon* newWeapon); // Setter pour l'arme de l'ennemi

    // Méthode d'attaque de l'ennemi si son type est physique
    void punch(Player* player); 

    //Méthode d'attaque de l'ennemi si son type est distance
    void shoot(Player* player);

    // Méthode générale pour infliger des dégâts au joueur
    void doDamage(Player* player); 

    // Méthode de suivi du joueur
    void followPlayer(Player* player); 


};

//Classe soldier qui hérite de Enemy

Class Soldier : public Enemy {
    

    public:
        // Constructeur
        Soldier(QGraphicsItem* parent = nullptr); // Constructeur



};

Class Hero : public Enemy {
   

    public:
        // Constructeur
        Hero(QGraphicsItem* parent = nullptr); // Constructeur
        // Méthode d'attaque de l'ennemi
       

};

Class Weapon : public QGraphicsPixmapItem {
    Q_OBJECT
    private:
        int damage;
        int range;
        QString type; // Type d'arme (ex: "gun", "sword", etc.)
        QPixmap* sprite; // Sprite de l'arme
    public:
        // Constructeur
        Weapon(int damage, int range, QString type, QGraphicsItem* parent = nullptr); // Constructeur
        // Accesseurs
        int getDamage() const;
        int getRange() const;
        QString getType() const; 
        // Mutateurs
        void setDamage(int newDamage);
        void setRange(int newRange);
        void setType(QString newType);
};


class Projectile : public QGraphicsPixmapItem{
    Q_OBJECT
    private:
        QPointF direction
        int speed;
        QPixmap* sprite;
        QTimer* timer;
    public:
        Projectile(QPointF startPosition, QPointF direction, int speed, QGraphicsItem* parent = nullptr);
        void setDirection(QPointF newDirection);
        void setSpeed(int newSpeed);
        void setSprite(QPixmap* newSprite);
        int getSpeed() const;
        QPointF getDirection() const;
    public slots:
        void move(Player* player); // Déplace le projectile vers la position cible
    
}



#endif 