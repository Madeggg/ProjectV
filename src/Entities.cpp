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