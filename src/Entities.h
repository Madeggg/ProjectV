// Cette page sert à tout ce qui est hostile (enemies, bombes etc..) + les projectiles tirés par joueur ou ennemis + les armes

#ifndef ENTITIES_H
#define ENTITIES_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>
#include <QString>

enum class EnemyType{           //On crée une classe à part pour les deux types d'ennemis 
    Soldier,
    Hero
};

class Enemy: public QGraphicsPixmapItem{        
    Q_OBJECT

public:
    // Constructeur : crée un ennemi du type spécifié
    Enemy(EnemyType type,QGraphicsItem* parent = nullptr);
    //Mutateur
    void setWeapon(Weapon* weapon);
    //Accesseur
    Weapon* getWeapon() const;

public slot : 
    void move();            //Méthode qui permettra à l'ennemi de se déplacer de manière aléatoire

private :
    EnemyType* type;
    QTimer* moveTimer;
    
    void setAppearance();       //Cette méthode permettra de donner l'apparence à l'ennemi en fonction de son type 


};


class Weapon{
    public:
        Weapon(QString name, int damage, QString projectileSprite); //Constucteur de l'arme

        //Accesseurs
        QString getName() const;
        int getDamage() const;
        QString getProjectileSprite() const;
    
    private:
        QString name;
        int damage;
        QString projectileSpritePath; // pour les visuels de tirs
};

#endif 