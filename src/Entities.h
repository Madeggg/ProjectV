// Cette page sert à tout ce qui est hostile (enemies, bombes etc..) + les projectiles tirés par joueur ou ennemis + les armes

#ifndef ENTITIES_H
#define ENTITIES_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>   
#include <QString>
#include <QGraphicsItem>

#include <QGraphicsScene>
#include <QPixmap>
#include <QRandomGenerator>
#include <QStringList>

#include "GameManager.h"
#include "Player.h"
#include "MyScene.h"

class Player;
class Weapon;


//Enemy est une classe abstraite, elle ne peut être instanciée que par les classe filles
class Enemy : public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
    protected:
        Player* targetPlayer;
        int health;
        int damage;
        int speed;
        bool distance;  //Booléen pour savoir si l'ennemi attaque le joueur à distance ou pas
        bool isDead = false; // Booléen pour savoir si l'ennemi est mort
        Weapon* weapon;
        QTimer* movementTimer; // Timer pour le mouvement de l'ennemi
        QTimer* attackTimer;    // Timer pour l'attaque de l'ennemi
        bool canAttack = true;
        bool canShoot = true;
        int currentFrame = 0;
        int deathDuration;
        QString facingDirection; // "up", "down", "left", "right"
       


        //Déplacements classiques
        QVector<QPixmap*> walkFront;
        QVector<QPixmap*> walkBack;
        QVector<QPixmap*> walkRight;
        QVector<QPixmap*> walkLeft;

        //Quand l'ennemi attaque 
        QVector<QPixmap*> attackFront;
        QVector<QPixmap*> attackBack;
        QVector<QPixmap*> attackRight;
        QVector<QPixmap*> attackLeft;

        // Quand l'ennemi meurt
        QVector<QPixmap*> deathFront;
        QVector<QPixmap*> deathBack;
        QVector<QPixmap*> deathRight;
        QVector<QPixmap*> deathLeft;

    signals:
        void damagePlayer(int damage); // Signal pour infliger des dégâts au joueur
    
    public:
        //Constructeur
        Enemy(QGraphicsItem* parent = nullptr, Player* player = nullptr, int health = 0, int damage = 0, int speed = 0); 
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
        void showHitEffect();               // Méthode pour changer le sprite de l'ennemi lorsqu'il est touché
        
        QPainterPath shape() const override;

        virtual void loadAnimations() = 0;
        virtual void playWalkAnimation() = 0; // Méthode pour jouer l'animation de marche
        virtual void playAttackAnimation() = 0;     // Méthode pour jouer l'animation d'attaque
        virtual void playDeathAnimation() = 0;      // Méthode pour jouer l'animation de mort

        virtual void attack(Player* player) = 0;

        void updateFacingDirection();       // Met à jour la direction de l'ennemi en fonction de sa position par rapport au joueur

        bool hasLineOfSightTo(Player* player); // Vérifie si l'ennemi a une ligne de vue sur le joueur
        virtual void wander() = 0; // Méthode pour faire errer l'ennemi
        virtual void moveTowardsPlayerOrWander(Player* player); // Méthode pour déplacer l'ennemi vers le joueur, ou errer
        virtual void moveTowards(QPointF target) = 0; // Méthode pour déplacer l'ennemi vers une cible

        virtual void takeDamage(int amount) = 0;    // Méthode pour infliger des dégâts à l'ennemi

        bool canMoveInDirection(const QPointF& direction);
};

class Zombie : public Enemy{

    public:
        Zombie(QGraphicsItem* parent = nullptr, Player* player = nullptr); // Constructeur

        void loadAnimations() override; // Charge les animations spécifiques au zombie
        void playWalkAnimation() override; // Joue l'animation de marche du zombie
        void playAttackAnimation() override; // Joue l'animation d'attaque du zombie
        void playDeathAnimation() override; // Joue l'animation de mort du zombie

        void wander() override; 
        void moveTowards(QPointF target) override;
        void moveTowardsPlayerOrWander(Player* player) override; 

        void takeDamage(int amount) override; 

        void attack(Player* player) override; 
};

class Vampire : public Enemy{

    public:
        Vampire(QGraphicsItem* parent = nullptr, Player* player = nullptr); // Constructeur

        void loadAnimations() override; // Charge les animations spécifiques au vampire
        void playWalkAnimation() override; // Joue l'animation de marche du vampire
        void playAttackAnimation() override; // Joue l'animation d'attaque du vampire
        void playDeathAnimation() override; // Joue l'animation de mort du vampire

        
        void wander() override; 
        void moveTowardsPlayerOrWander(Player* player) override; 
        void moveTowards(QPointF target) override;

        void takeDamage(int amount) override;

        void attack(Player* player) override; // Attaque le joueur avec une attaque à distance
};


class Reaper : public Enemy{
    private:
        bool canPlayWalkAnimation = true; // Pour éviter de jouer l'animation de marche quand il attaque
    public:
        Reaper(QGraphicsItem* parent = nullptr, Player* player = nullptr); // Constructeur

        void loadAnimations() override; // Charge les animations spécifiques au reaper
        void playWalkAnimation() override; 
        void playAttackAnimation() override; 
        void playDeathAnimation() override; 
        
        void wander() override; 
        void moveTowardsPlayerOrWander(Player* player) override; 
        void moveTowards(QPointF target) override;

        void takeDamage(int amount) override;

        void attack(Player* player) override; // Attaque le joueur 
};

class Weapon : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
    private:
        QString type; // Type d'arme 
        int ammo;
    public:
        // Constructeur
        Weapon(QString type, QGraphicsItem* parent = nullptr); // Constructeur
        // Accesseurs
        QString getType() const; 
        int getAmmo() const;
        // Mutateurs
        void setType(QString newType);
        void setSprite(QPixmap* newSprite);     
        void setAmmo(int newAmmo);

        
signals:
    void ammoChanged(int newAmmo);
};


class Projectile : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
    private:
        QPointF direction;
        int speed;
        int damage;
        QString* sprite;
        QTimer* timer;
        QString source; // Source du projectile (ennemi ou joueur)
        qreal distanceTraveled = 0; // Distance parcourue par le projectile
        qreal maxDistance; // Distance maximale que le projectile peut parcourir
    public:
        Projectile(QPointF startPosition, QPointF direction, int speed, int damage, QGraphicsItem* parent = nullptr);
        void setDirection(QPointF newDirection);
        void setSpeed(int newSpeed);
        void setSprite(QPixmap* newSprite);
        void setDamage(int newDamage);
        void setSource(QString newSource);
        void setMaxDistance(qreal newMaxDistance);

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