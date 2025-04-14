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

enum class EnemyType{           //On crée une classe à part pour les deux types d'ennemis 
    Soldier,
    Hero
    //+ pour plus tard
};

class Weapon{
    public:
        Weapon(QString name, int damage, QString projectileSprite,WeaponType type); //Constructeur de l'arme

        //Accesseurs
        QString getName() const;
        int getDamage() const;
        WeaponType getType() const;
        QString getProjectileSprite() const;            
    
    private:
        QString name;
        int damage;
        WeaponType type; // pour savoir si c'est une arme de type Gun ou Rifle
        QString projectileSpritePath; // pour les visuels de tirs
};



class Enemy: public QGraphicsPixmapItem{        
    Q_OBJECT

public:
    // Constructeur : crée un ennemi du type spécifié
    Enemy(EnemyType type,QGraphicsItem* parent = nullptr);
    //Mutateur
    void setWeapon(Weapon* weapon);
    //Accesseur
    Weapon getWeapon() const;

public slots : 
    void move();            //Méthode qui permettra à l'ennemi de se déplacer de manière aléatoire
    void shoot();          //Méthode qui permettra à l'ennemi de tirer un projectile

private :
    EnemyType type;
    QTimer* moveTimer;
    Weapon weapon;
    
    void setAppearance();       //Cette méthode permettra de donner l'apparence à l'ennemi en fonction de son type 


};

enum class WeaponType{           //On crée une classe à part pour les deux types d'armes 
    Gun,
    Rifle
    //+ pour plus tard
};





//Classe projectile pour gérer les tirs ennemis et du joueur, gérer leur déplacement et leur collision avec les ennemis

class Projectile : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Projectile(QPointF startPosition, QPointF direction, int speed, QGraphicsItem* parent = nullptr);

public slots:
    void move();

private:
    QPointF direction;
    int speed;
};

#endif 