// Cette page sert à tout ce qui est hostile (enemies, bombes etc..) + les projectiles tirés par joueur ou ennemis + les armes

#include "Entities.h"
#include <QGraphicsScene>
#include <QPixmap>
#include <QRandomGenerator>
#include <QStringList>

// Méthode de Enemy

Enemy::Enemy(EnemyType type, Player* player, QGraphicsItem* parent) : QGraphicsPixmapItem(parent), type(type), weapon(), health(100) {
    setAppearance(); // On définit le sprite + comportement visuel dès la création 
    followTimer = new QTimer(this); 
    connect(followTimer, &QTimer::timeout, this, [this, player]() {
        if (player) {
            followPlayer(player); // Suit dynamiquement le joueur
        }
        else{
            qDebug() << "Le joueur n'est pas défini ici. L'ennemi ne peut pas le suivre.";
        }
    });
    followTimer->start(30); // On fait en sorte que le timer se déclenche toutes les 100ms
    qDebug() << "Timer de suivi initialisé pour l'ennemi.";
}

void Enemy::setAppearance() {
    QString imagePath; // Déclaration du lien vers l'image comme un objet QString

    if (type == EnemyType::Soldier) {
        imagePath = "img/atrain.png";
        weapon = Weapon("Rifle", 10, "", WeaponType::Rifle); // Sprite unique pour les soldats
    } else if (type == EnemyType::Hero) {
        QStringList heroSprites = {
            "homelander",
            "a_train",
            "black_noir", // Exemple d'une liste QStringList contenant les sprites de super héros (des strings vers les liens des sprites)
            "starlight",
            "the_deep"
        };

        // Choix aléatoire d’un héros dans la liste de sprite 
        int index = QRandomGenerator::global()->bounded(heroSprites.size());
        QString chosenHero = heroSprites[index];
        imagePath = ":/img/super_" + chosenHero + ".png";
        weapon = Weapon("Laser", 20, ":/img/laser.png", WeaponType::Laser); // Exemple d'arme pour un héros
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

int Enemy::getHealth() const {
    return health;
}

void Enemy::setHealth(int newHealth) {
    health = newHealth;
}

void Enemy::setWeapon(Weapon* newWeapon) {
    weapon = *newWeapon; 
}



void Enemy::followPlayer(Player* player) {
    if (player) {
        QPointF playerPos = player->pos();
        QPointF direction = playerPos - pos();
        qreal length = std::sqrt(direction.x() * direction.x() + direction.y() * direction.y());

         // Si l'ennemi est suffisamment proche du joueur, il arrête de bouger
         if (length < 2.0) { // Seuil de proximité (2 pixels)

            return;
        }

        // Normalisation du vecteur direction
        if (length != 0) {
            direction /= length;
        }

        // Déplacement de l'ennemi vers le joueur
        qreal speed = 2; // Vitesse de l'ennemi
        setPos(pos() + direction * speed);
        
       
    }
}

void Enemy::attack(Player* player) {
    if (player) {
        // Logique d'attaque ici, par exemple, infliger des dégâts au joueur
        player->takeDamage(weapon.getDamage()); // Utilise l'arme de l'ennemi pour infliger des dégâts au joueur
        qDebug() << "L'ennemi attaque le joueur ! Dégâts infligés : " << weapon.getDamage();
    } else {
        qDebug() << "Aucun joueur à attaquer.";
    }

}
// Méthode de Weapon

//Constructeur par défaut
Weapon::Weapon() : name(""), damage(0), projectileSpritePath(""), type(WeaponType::Rifle) {}

//Constructeur avec paramètres
Weapon::Weapon(QString name, int damage, QString projectileSprite, WeaponType type)
    : name(name), damage(damage), projectileSpritePath(projectileSprite), type(type) {}

// Accesseurs
QString Weapon::getName() const { return name; }
int Weapon::getDamage() const { return damage; }
QString Weapon::getProjectileSprite() const { return projectileSpritePath; }
WeaponType Weapon::getType() const { return type; }

// Méthode de Projectile

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
    // On vérifie si la position actuelle du projectile est contenue dans le rectangle de la scène
    // Si le projectile sort de la scène, on le supprime
    if (!scene()->sceneRect().contains(pos())) {
        scene()->removeItem(this);
        delete this;
    }
}

