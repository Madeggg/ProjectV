// Cette page est la afin de gérer le Personnage principal, et les controles par le joueur

#include "Player.h"
#include <QPixmap>

Player::Player(QGraphicsItem* parent) : QObject(), QGraphicsPixmapItem(parent), health(100), speed(5) { //appel des constructeur de QObject et QGraphicsPixmapItem
    setPixmap(QPixmap(":/images/player.png").scaled(40, 40));
}
// Méthodes de déplacement 
void Player::moveLeft() {
    setPos(x() - speed, y());
}

void Player::moveRight() {
    setPos(x() + speed, y());
}

void Player::moveUp() {
    setPos(x(), y() - speed);
}

void Player::moveDown() {
    setPos(x(), y() + speed);
}

int Player::getHealth() const {
    return health;
}

void Player::setHealth(int newHealth) {
    health = newHealth;
}

void Player::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
}