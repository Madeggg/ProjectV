// Cette page sert à tout ce qui est hostile (enemies, bombes etc..) + les projectiles tirés par joueur ou ennemis + les armes

#include "Entities.h"
#include <QGraphicsScene>
#include <QPixmap>
#include <QRandomGenerator>
#include <QStringList>

// Méthodes de Enemy

Enemy::Enemy(QGraphicsItem* parent, Player* player) : QGraphicsPixmapItem(parent), targetPlayer(player), health(100), damage(10), speed(5) {
    setDistance(false); // Par défaut, l'ennemi n'attaque pas à distance
    setType(nullptr);
    setWeapon(nullptr); // L'ennemi n'a pas d'arme par défaut

    damageTimer = new QTimer(this);
    connect(damageTimer, &QTimer::timeout, this, [this]() {
        emit damagePlayer(damage); // Émet le signal avec les dégâts
    });
    damageTimer->start(3000); // Toutes les 3 secondes
} 


int Enemy::getHealth() const {
    return health;
}

int Enemy::getDamage() const {
    return damage;
}

int Enemy::getSpeed() const {
    return speed;
}

QString* Enemy::getType() const{
    return type;
}

void Enemy::setHealth(int newHealth) {
    health = newHealth;
}

void Enemy::setDamage(int newDamage) {
    damage = newDamage;
}

void Enemy::setSpeed(int newSpeed) {
    speed = newSpeed;
}

void Enemy::setWeapon(Weapon* newWeapon) {
    weapon = newWeapon;
}

void Enemy::setType(QString* newType) {
    type = newType;
}

void Enemy::setApperance(QString* newType) {
    if (newType && newType->compare("Physique") == 0) {
        sprite_up = new QPixmap("img/sprite_enemy_up.png");
        sprite_down = new QPixmap("img/sprite_enemy_down.png");
        sprite_right = new QPixmap("img/sprite_enemy_right.png");
        sprite_left = new QPixmap("img/sprite_enemy_left.png");
    } else if (newType && newType->compare("Distance") == 0) {
        sprite_up = new QPixmap("img/enemy_distance_up.png");
    }
}


void Enemy::setDistance(bool newDistance) {
    distance = newDistance;
}

void Enemy::punch(Player* player){
    player->takeDamage(damage); 
}

void Enemy::shoot(Player* player){
    player->takeDamage(weapon->getDamage()); 
}

void Enemy::doDamage(Player* player) {
    if (type && type->compare("Physique") == 0) {
        punch(player);
    } else if (type && type->compare("Distance") == 0) {
        shoot(player);
    }
}

void Enemy::followPlayerAndAttack(Player* player) {
    if (player) {
        // QLineF crée une ligne entre la position de l'ennemi et celle du joueur
        QLineF lineToPlayer(pos(), player->pos());

        if (type && type->compare("Physique") == 0) { // Utilisation de QString::compare
            // Si l'ennemi est suffisamment proche du joueur, il attaque
            if (lineToPlayer.length() < 2.0) { // Seuil de proximité (2 pixels)
                qDebug() << "L'ennemi de type physique attaque le joueur !";
                if (player->collidesWithItem(this)) {
                    // Si l'ennemi est en collision avec le joueur, il inflige des dégâts
                    doDamage(player);
                }
                doDamage(player);
                return;
            }

            // Réduit la longueur de la ligne à la vitesse de l'ennemi
            lineToPlayer.setLength(speed);

            // Déplace l'ennemi en utilisant les décalages calculés par QLineF
            setPos(pos().x() + lineToPlayer.dx(), pos().y() + lineToPlayer.dy());
        } else if (type && type->compare("Distance") == 0) { // Utilisation de QString::compare
            // Si l'ennemi est suffisamment proche du joueur, il tire
            if (lineToPlayer.length() <= 100) { // Seuil de proximité (100 pixels)
                qDebug() << "L'ennemi de type distance tire sur le joueur !";

                doDamage(player);
                return;
            }

            // Réduit la longueur de la ligne à la vitesse de l'ennemi
            lineToPlayer.setLength(speed);

            // Déplace l'ennemi en utilisant les décalages calculés par QLineF
            setPos(pos().x() + lineToPlayer.dx(), pos().y() + lineToPlayer.dy());
        }
    }
}


//Méthodes de Soldier

Soldier::Soldier(QGraphicsItem* parent, Weapon* w) :  QGraphicsPixmapItem(parent){
    setHealth(50);
    setDamage(10);
    setSpeed(2);
    setWeapon(w); // L'ennemi n'a pas d'arme par défaut
}



//Méthodes de Hero

Hero::Hero(QGraphicsItem* parent,Weapon* w) : QGraphicsPixmapItem(parent){
    setHealth(100);
    setDamage(30);
    setSpeed(5);
    setWeapon(w);
}


//Méthodes de Weapon

Weapon::Weapon(int damage, int range, QString type, QGraphicsItem* parent) : QGraphicsPixmapItem(parent){
    setDamage(damage);
    setRange(range);
    setType(type);
}

int Weapon::getDamage() const{
    return damage;
}

int Weapon::getRange() const{
    return range;
}

QString Weapon::getType() const{
    return type;
}

void Weapon::setDamage(int newDamage){
    damage = newDamage;
}

void Weapon::setRange(int newRange){
    range = newRange;
}

void Weapon::setType(QString newType){
    type = newType;
}



//Méthodes de Projectile

Projectile::Projectile(QPointF startPos, QPointF direction, int speed, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent), speed(speed) {
    setPixmap(QPixmap("img/bullet1.png").scaled(20, 20)); // Sprite du projectile
    setPos(startPos);

    // Définit la direction normalisée
    setDirection(direction);

    // Configure un timer pour déplacer le projectile
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() {
        move(); // Appelle move sans argument
    });
    timer->start(16); // Appelle move toutes les 16 ms (60 FPS)
}

int Projectile::getSpeed() const {
    return speed;
}

QPointF Projectile::getDirection() const {
    return direction;
}   

void Projectile::setDirection(QPointF newDirection) {
    direction = newDirection;
}

void Projectile::setSpeed(int newSpeed) {
    speed = newSpeed;
}   

void Projectile::setSprite(QPixmap* newSprite) {
    sprite = newSprite;
}

void Projectile::move() {
    // Déplace le projectile en fonction de la direction et de la vitesse
    setPos(pos().x() + direction.x() * speed, pos().y() + direction.y() * speed);


    // Vérification des collisions avec les objets "collision"
    QList<QGraphicsItem*> collisions = collidingItems(); // Récupère tous les éléments en collision avec le projectile

    for (QGraphicsItem* item : collisions) {
        if (!item) continue;
        
        QVariant typeData = item->data(0);
        
        // Si le projectile entre en collision avec un objet de type "collision"
        if (typeData.toString() == "collision") {
            qDebug() << "Collision avec un objet de type 'collision' à :" << item->pos();
            scene()->removeItem(this); // Retirer le projectile de la scène
            delete this;  // Supprimer le projectile
            return;  // Quitter la méthode après la suppression
        }
    }
   
    if (!scene()->sceneRect().contains(pos())) {
        qDebug() << "Projectile hors de la scène. Suppression.";
        scene()->removeItem(this);
        delete this; 
    }
}