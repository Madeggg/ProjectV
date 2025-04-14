// Cette page sert à tout ce qui est hostile (enemies, bombes etc..) + les projectiles tirés par joueur ou ennemis + les armes

#include "Entities.h"
#include <QGraphicsScene>
#include <QPixmap>
#include <QRandomGenerator>
#include <QStringList>

//Méthode de Enemy

Enemy::Enemy(EnemyType type, QGraphicsItem* parent) : QGraphicsPixmapItem(parent), type(type){
    setAppearance();            // On définit le sprite + comportement visuel dès la création 
}

void Enemy::setAppearance(){
    QString imagePath;              //Déclaration du lien vers l'image comme un objet QString

    if (type == EnemyType::Soldier) {
        imagePath = "";               
        weapon = Weapon("Rifle",10,"",WeaponType::Rifle);                     // Sprite unique pour les soldats
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
        weapon = Weapon("Laser", 20, ":/img/laser.png");    // Exemple d'arme pour un héros

    }

    QPixmap sprite(imagePath);  
    if (!sprite.isNull()) {
        setPixmap(sprite.scaled(64, 64)); // La fonction scale permet d'appliquer au pixmap la taille donnée 
    } else {
        qDebug("Image introuvable : %s", qPrintable(imagePath));
    }
}

void Enemy::shoot() {
    QPointF direction(0, 1); // Exemple : les projectiles descendent
    int speed = 5;
    Projectile* projectile = new Projectile(pos(), direction, speed);
    scene()->addItem(projectile);
}

Weapon Enemy::getWeapon() const {
    return weapon;
}


//Méthode de weapon

Weapon::Weapon(QString name, int damage, QString projectileSprite): 
    name(name), damage(damage), projectileSpritePath(projectileSprite), type(type) {}

//Accesseurs
QString Weapon::getName() const { return name; }
int Weapon::getDamage() const { return damage; }
QString Weapon::getProjectileSprite() const { return projectileSpritePath; }

void Entity::setWeapon(Weapon* w) {
    weapon = w;
}

WeaponType Weapon::getType() const {
    return type;
}


//Méthode de Projectile

Projectile::Projectile(QPointF startPosition, QPointF direction, int speed, QGraphicsItem* parent)
    : QObject(), QGraphicsPixmapItem(parent), direction(direction), speed(speed) {
    setPos(startPosition);
    setPixmap(QPixmap(":/img/projectile.png").scaled(16, 16)); // Exemple de sprite
    QTimer* moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, &Projectile::move);
    moveTimer->start(16); // 60 FPS
}

void Projectile::move() {
    setPos(pos() + direction * speed);
    //On vérifie si la position actuelle du projectile est contenue dans le rectangle de la scène
    //Si le projectile sort de la scène, on le supprime
    if (!scene()->sceneRect().contains(pos())) {
        scene()->removeItem(this);
        delete this;
    }
}