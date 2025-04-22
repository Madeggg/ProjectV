// Cette page sert à tout ce qui est hostile (enemies, bombes etc..) + les projectiles tirés par joueur ou ennemis + les armes

#include "Entities.h"
#include <QGraphicsScene>
#include <QPixmap>
#include <QRandomGenerator>
#include <QStringList>

// Méthodes de Enemy

Enemy::Enemy(QGraphicsItem* parent) :  QGraphicsPixmapItem(parent), health(100), damage(10), speed(5) {
    // Initialisation des sprites
    sprite_up = new QPixmap("img/enemy_up.png");
    sprite_down = new QPixmap("img/enemy_down.png");
    sprite_right = new QPixmap("img/enemy_right.png");
    sprite_left = new QPixmap("img/enemy_left.png");

    //Par défaut, l'ennemi n'attaque pas à distance
    distance = false;

    type = nullptr; // Initialisation du type d'ennemi à nullptr

    weapon = nullptr; 

    damageTimer = new QTimer(this);
    connect(damageTimer, &QTimer::timeout, this, &Enemy::doDamage);
    shootTimer->start(3000); // Toutes les 3 secondes
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

QString Enemy::getType() const{
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

void Enemy::punch(Player* player){
    player->takeDamage(damage); 
}

void Enemy::shoot(Player* player){
    player->takeDamage(weapon->getDamage()); 
}

void Ennemy::doDamage(Player* player){
    if(type == "Physique"){
        punch(player);
    }
    else if(type == "Distance"){
        shoot(player);
    }
}

void Enemy::followPlayerAndAttack(Player* player){
    if(player){
        //QLineF crée une ligne entre la position de l'ennemi et celle du joueur
        QLineF lineToPlayer(pos(), player->pos());

        if(getType() == "Physique"){
            // Si l'ennemi est suffisamment proche du joueur, il attaque
            if (lineToPlayer.length() < 2.0) { // Seuil de proximité (2 pixels)
                qDebug() << "L'ennemi de type physique attaque le joueur !";
                doDamage(player);
                return;
            }

             // Réduit la longueur de la ligne à la vitesse de l'ennemi
             lineToPlayer.setLength(speed);

             // Déplace l'ennemi en utilisant les décalages calculés par QLineF
             setPos(pos().x() + lineToPlayer.dx(), pos().y() + lineToPlayer.dy());

        }

        else if (getType() == "Distance"){
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

Soldier::Soldier(QGraphicsItem* parent,Weapon* w) :  QGraphicsPixmapItem(parent){
    setHealth(50);
    setDamage(10);
    setSpeed(2);
    setWeapon(w);
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

QString Weapon::getType const{
    return type;
}

void Weapon::setDamage(int newDamage){
    damage = newDamage;
}

void Weapon::setRange(int newRange){
    range = newRange;
}

void Weapon setType(QString newType){
    type = newType;
}



//Méthodes de Projectile

Projectile::Projectile(QPointF startPos, QPointF direction, int speed, QGraphicsItem* parent) : QGraphicsPixmapItem(parent), speed(speed) {
    setPixmap(QPixmap("img/projectile.png").scaled(10, 10));
    setPos(startPos);
    setDirection(direction)
    setSpeed(speed);
    timer = new QTimer(this);
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

void Projectile::move(Player* player) {
    if (player) {
        // Récupère la position actuelle du joueur
        QPointF playerPos = player->pos();

        
        if (direction.isNull()) {
            QLineF lineToPlayer(pos(), playerPos);
            qreal length = lineToPlayer.length();
            if (length != 0) {
                direction = QPointF(lineToPlayer.dx() / length, lineToPlayer.dy() / length); // Normalisation
                setDirection(direction); // Définit la direction
            }
        }

        // Déplace le projectile en fonction de la direction et de la vitesse
        setPos(pos().x() + direction.x() * speed, pos().y() + direction.y() * speed);

    }
}