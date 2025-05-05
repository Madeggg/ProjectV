// Cette page est la afin de gérer le Personnage principal, et les controles par le joueur

#include "Player.h"
#include <QPixmap>

Player::Player(QGraphicsItem* parent) : QObject(), QGraphicsPixmapItem(parent), health(100), speed(5) {
    setPixmap(QPixmap("img/Sprite_billy_static.png").scaled(40, 40));
    setShapeMode(QGraphicsPixmapItem::BoundingRectShape); 
    previousPosition = pos();

    walkTimer = new QTimer(this);
    walkTimer->setInterval(150);  // vitesse de l'animation
    connect(walkTimer, &QTimer::timeout, this, &Player::updateWalkAnimation);
}
// Méthodes de déplacement 
void Player::moveLeft() {
    previousPosition = pos(); // Sauvegarde la position actuelle
    setPos(x() - speed, y());
    // qDebug() << "Signal playerMoved émis. Nouvelle position : " << pos();
    emit playerMoved(pos());
}

void Player::moveRight() {
    previousPosition = pos(); // Sauvegarde la position actuelle
    setPos(x() + speed, y());
    // qDebug() << "Signal playerMoved émis. Nouvelle position : " << pos();
    emit playerMoved(pos());
}

void Player::moveUp() {
    previousPosition = pos(); // Sauvegarde la position actuelle
    setPos(x(), y() - speed);
    // qDebug() << "Signal playerMoved émis. Nouvelle position : " << pos();
    emit playerMoved(pos());
}

void Player::moveDown() {
    previousPosition = pos(); // Sauvegarde la position actuelle
    setPos(x(), y() + speed);
    //  qDebug() << "Signal playerMoved émis. Nouvelle position : " << pos();
    emit playerMoved(pos());
}

void Player::revertToPreviousPosition() {
    setPos(previousPosition);
    qDebug() << "Position restaurée à :" << previousPosition;
}

bool Player::canMoveTo(const QPointF& newPosition, const QRectF& sceneRect) const {
    QRectF playerRect(newPosition, QSizeF(boundingRect().width(), boundingRect().height()));

    // Vérifie si la nouvelle position est à l'intérieur de la scène
    if (!sceneRect.contains(playerRect)) {
        return false;
    }

    // Vérifie les collisions avec les objets marqués comme "collision"
    QList<QGraphicsItem*> collisions = collidingItems();
    for (QGraphicsItem* item : collisions) {
        QVariant typeData = item->data(0);  // Récupère le type de l'objet
        if (typeData.toString() == "collision") {
            return false;  // Empêche le mouvement s'il y a une collision
        }
    }

    return true;  // Le déplacement est possible si aucune collision n'est détectée
}

void Player::updateWalkAnimation() {
    walkframe = (walkframe + 1) % 2;        //Pour alterner entre 0 et 1
    QString spritePath = QString("img/Sprite_billy_%1_%2.png").arg(direction).arg(walkframe + 1);
    setPixmap(QPixmap(spritePath).scaled(40, 40));
}


int Player::getHealth() const {
    return health;
}

qreal Player::getSpeed() const {
    return speed; // Retourne la vitesse actuelle du joueur
}

QString Player::getDirection() const {
    return direction;   // Retourne la direction actuelle du joueur
}

void Player::setHealth(int newHealth) {
    health = newHealth;
}

void Player::setDirection(const QString& dir){
    direction = dir;
}
void Player::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
    qDebug() << "Le joueur a subi des dégâts. Santé restante :" << health;
}


QPainterPath Player::shape() const {
    QPainterPath path;
    path.addRect(boundingRect()); // Utilise la même forme que le rectangle de collision
    return path;
}