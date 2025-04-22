// Cette page est la afin de gérer le Personnage principal, et les controles par le joueur

#include "Player.h"
#include <QPixmap>

Player::Player(QGraphicsItem* parent) : QObject(), QGraphicsPixmapItem(parent), health(100), speed(5) { //appel des constructeur de QObject et QGraphicsPixmapItem
    setPixmap(QPixmap("img/Sprite_billy_static.png").scaled(40, 40));
}
// Méthodes de déplacement 
void Player::moveLeft() {
    setPos(x() - speed, y());
    qDebug() << "Signal playerMoved émis. Nouvelle position : " << pos();
    emit playerMoved(pos());
}

void Player::moveRight() {
    setPos(x() + speed, y());
    qDebug() << "Signal playerMoved émis. Nouvelle position : " << pos();
    emit playerMoved(pos());
}

void Player::moveUp() {
    setPos(x(), y() - speed);
    qDebug() << "Signal playerMoved émis. Nouvelle position : " << pos();
    emit playerMoved(pos());
}

void Player::moveDown() {
    setPos(x(), y() + speed);
     qDebug() << "Signal playerMoved émis. Nouvelle position : " << pos();
    emit playerMoved(pos());
}

bool Player::canMoveTo(const QPointF& newPosition, const QRectF& sceneRect) const {
    QRectF playerRect(newPosition, QSizeF(boundingRect().width(), boundingRect().height()));
    //Renvoie true si la scène contient le joueur
    return sceneRect.contains(playerRect);
}

int Player::getHealth() const {
    return health;
}

qreal Player::getSpeed() const {
    return speed; // Retourne la vitesse actuelle du joueur
}

void Player::setHealth(int newHealth) {
    health = newHealth;
}

void Player::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
}