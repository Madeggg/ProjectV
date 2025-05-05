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
class Weapon;


class Enemy : public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
protected:
    Player* targetPlayer;
    int health;
    int damage;
    int speed;
    bool distance; //Booléen pour savoir si l'ennemi attaque le joueur à distance ou pas
    bool isDead = false; // Booléen pour savoir si l'ennemi est mort
    Weapon* weapon;
    QString type;
    QPixmap* sprite_up;
    QPixmap* sprite_down;
    QPixmap* sprite_right;
    QPixmap* sprite_left;
    QTimer* movementTimer; // Timer pour le mouvement de l'ennemi
    QTimer* attackTimer;    // Timer pour l'attaque de l'ennemi
    bool canAttack = true;

signals:
    void damagePlayer(int damage); // Signal pour infliger des dégâts au joueur

public:
    //Constructeur
    Enemy(QString type,QGraphicsItem* parent = nullptr, Player* player = nullptr); 
    //Accesseurs
    int getHealth() const;
    int getDamage() const;
    int getSpeed() const;
    QString getType() const; // Getter pour le type d'ennemi
    bool getIsDead() const; // Getter pour le booléen isDead
    //Mutateurs
    void setHealth(int newHealth);
    void setDamage(int newDamage);
    void setSpeed(int newSpeed);
    void setWeapon(Weapon* newWeapon); 
    void setType(QString newType);     
    virtual void setApperance(QString newType); 
    void setDistance(bool newDistance); // Setter pour le booléen distance
    

    // Méthode d'attaque de l'ennemi si son type est physique
    void punch(Player* player); 

    //Méthode d'attaque de l'ennemi si son type est distance
    void shoot(Player* player);

    // Méthode générale pour infliger des dégâts au joueur
    void doDamage(Player* player); 

    // Méthode de suivi du joueur
    void moveTowardsPlayer(const QPointF& playerPos);

    void takeDamage(int amount);    // Méthode pour infliger des dégâts à l'ennemi

    

};

// //Classe soldier qui hérite de Enemy

// class Soldier : public Enemy, public virtual QGraphicsPixmapItem {
//     public:
//         // Constructeur
//         Soldier(QString type,QGraphicsItem* parent = nullptr, Weapon* w = nullptr); // Constructeur



// };

// class Hero : public Enemy, public virtual QGraphicsPixmapItem {
//     public:
//         // Constructeur
//         Hero(QString type,QGraphicsItem* parent = nullptr, Weapon* w = nullptr); // Constructeur
//         void setApperance(); // Méthode pour définir l'apparence du héros
        
       

// };

class Weapon : public QGraphicsPixmapItem {
    private:
        int damage;
        int range;
        QString type; // Type d'arme 
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


class Projectile : public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
    private:
        QPointF direction;
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
        void move(); // Déplace le projectile 
    signals:
    void damagePlayer(int damage); // Signal pour infliger des dégâts au joueur
    void hitEnemy(Enemy* enemy);
    
};



#endif 