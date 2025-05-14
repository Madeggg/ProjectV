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
        QString type;       //Type de l'eenemi (physique ou distance)
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
        void showHitEffect();               // Méthode pour changer le sprite de l'ennemi lorsqu'il est touché
        virtual void setApperance(QString newType); 
        void setDistance(bool newDistance); // Setter pour le booléen distance
        QRectF boundingRect() const;
        QPainterPath shape() const override;
      

        // Méthode d'attaque de l'ennemi si son type est physique
        void punch(Player* player); 

        //Méthode d'attaque de l'ennemi si son type est distance
        void shoot(Player* player);

        // Méthode générale pour infliger des dégâts au joueur
        void doDamage(Player* player); 

        // Méthode de suivi du joueur
        void moveTowardsPlayer(const QPointF& playerPos);

        void takeDamage(int amount);    // Méthode pour infliger des dégâts à l'ennemi

        bool canMoveInDirection(const QPointF& direction);
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

class Weapon : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
    private:
        int damage;
        QString type; // Type d'arme 
        int ammo;
    public:
        // Constructeur
        Weapon(int damage, QString type, QGraphicsItem* parent = nullptr); // Constructeur
        // Accesseurs
        int getDamage() const;
        QString getType() const; 
        int getAmmo() const;
        // Mutateurs
        void setDamage(int newDamage);
        void setType(QString newType);
        void setSprite(QPixmap* newSprite);     
        void setAmmo(int newAmmo);
};


class Projectile : public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
    private:
        QPointF direction;
        int speed;
        int damage;
        QString* sprite;
        QTimer* timer;
        QString source; // Source du projectile (ennemi ou joueur)
    public:
        Projectile(QPointF startPosition, QPointF direction, int speed, int damage, QGraphicsItem* parent = nullptr);
        void setDirection(QPointF newDirection);
        void setSpeed(int newSpeed);
        void setSprite(QPixmap* newSprite);
        void setDamage(int newDamage);
        void setSource(QString newSource);

        int getSpeed() const;
        int getDamage() const;
        QPointF getDirection() const;
        QString getSource() const;
    public slots:
        void move(); // Déplace le projectile 
    signals:
    void damagePlayer(int damage); // Signal pour infliger des dégâts au joueur
    void hitEnemy(Enemy* enemy);
    
};



#endif 