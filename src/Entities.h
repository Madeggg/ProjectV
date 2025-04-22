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

class Enemy : public QObject{
    Q_OBJECT
protected:
    int health;
    int damage;
    int speed;
    bool distance; //Booléen pour savoir si l'ennemi attaque le joueur à distance ou pas
    QString* type;
    QPixmap* sprite_up;
    QPixmap* sprite_down;
    QPixmap* sprite_right;
    QPixmap* sprite_left;

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
    void attack(Player* player); // Méthode d'attaque de l'ennemi
    void followPlayer(Player* player); // Méthode de suivi du joueur


};

//Classe soldier qui hérite de Enemy

Class Soldier : public Enemy {
    private:

    public:
        // Constructeur
        Soldier(QGraphicsItem* parent = nullptr); // Constructeur



};

Class Hero : public Enemy {
    private:

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
};
#endif 