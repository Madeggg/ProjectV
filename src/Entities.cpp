// Cette page sert à tout ce qui est hostile (enemies, bombes etc..) + les projectiles tirés par joueur ou ennemis + les armes

#include "Entities.h"
#include <QGraphicsScene>
#include <QPixmap>
#include <QRandomGenerator>
#include <QStringList>

Enemy::Enemy(EnemyType type, QGraphicsItem* parent) : QGraphicsPixmapItem(parent), type(type){
    setAppearance();            // On définit le sprite + comportement visuel dès la création 
}

void Enemy::setAppearance(){
    QString imagePath;              //Déclaration du lien vers l'image comme un objet QString

    if (type == EnemyType::Soldier) {
        imagePath = " ";                                    // Sprite unique pour les soldats
    } else if (type == EnemyType::Hero) {
        QStringList heroSprites = {                         
            "homelander",
            "a_train",
            "black_noir",                   //Exemple d'une liste QStringList contenant les sprites de super héros (des strings vers les liens des sprites)
            "starlight",
            "the_deep"
        };

        // Choix aléatoire d’un héros dans la liste de sprite 
        int index = QRandomGenerator::global()->bounded(heroSprites.size());
        QString chosenHero = heroSprites[index];
        imagePath = ":/img/super_" + chosenHero + ".png";


    }

    QPixmap sprite(imagePath);  
    if (!sprite.isNull()) {
        setPixmap(sprite.scaled(64, 64)); // La fonction scale permet d'appliquer au pixmap la taille donnée 
    } else {
        qDebug("Image introuvable : %s", qPrintable(imagePath));
    }
}


Weapon::Weapon(QString name, int damage, QString projectileSprite): 
    name(name), damage(damage), projectileSpritePath(projectileSprite) {}

//Accesseurs
QString Weapon::getName() const { return name; }
int Weapon::getDamage() const { return damage; }
QString Weapon::getProjectileSprite() const { return projectileSpritePath; }

void Entity::setWeapon(Weapon* w) {
    weapon = w;
}

Weapon   Entity::getWeapon() const {
    return weapon;
}