// Cette page est la afin de gérer le Personnage principal, et les controles par le joueur

#include "Player.h"
#include <QPixmap>

Player::Player(QGraphicsItem* parent) : QObject(), QGraphicsPixmapItem(parent), health(100), speed(5) {
    setPixmap(QPixmap("img/Sprite_billy_static.png").scaled(40, 40));
    setShapeMode(QGraphicsPixmapItem::BoundingRectShape); 
    previousPosition = pos();

    hasWeapon = false; // Par défaut, le joueur n'a pas d'arme
    weapon = nullptr; 

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

void Player::checkKillCount() {
    killCount++;
    qDebug() << "Kill count = " << killCount;

    emit scoreChanged(killCount * 10); 

    if (killCount % 5 == 0) {
        emit ammoBoxNeeded();
    }
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

bool Player::selectSlot(Slot s){
    if (inventory[s] == nullptr){
        return false;   // rien dans ce slot
    }
    else{
        currentSlot = s;
        emit weaponChanged(inventory[s]);            // pour le HUD
        return true;
    }
    
}

void Player::setHasWeapon(bool newHasWeapon) {
    hasWeapon = newHasWeapon;
}


Player::Slot Player::pickWeapon(Weapon* w){
    if (!w) return Melee; // pas d'arme à ramasser

    // Détermine le slot cible
    Slot pickedSlot = Melee;           // valeur par défaut (au cas où)

    if (w->getType() == "Pistol") {
        pickedSlot          = Pistol;
        inventory[Pistol]   = w;
    }
    else if (w->getType() == "Shotgun") {
        pickedSlot          = Shotgun;
        inventory[Shotgun]  = w;
    }
    else {
        // autres types éventuels à ajouter
    }

    // --- debug ------------------------------------------------------------
    qDebug() << ">> Weapon picked =" << w->getType()
             << "| placed in slot" << pickedSlot;

    for (int i = 0; i < 3; ++i) {
        qDebug() << "slot" << i << ":"
                 << (inventory[i] ? inventory[i]->getType() : "null");
    }

    // Éventuel signal pour le HUD
    emit weaponChanged(inventory[pickedSlot]);

    return pickedSlot;  // renvoie le slot occupé par cette arme
}

void Player::switchTo(Slot s)
{
    currentSlot = s;
    emit weaponChanged(inventory[s]);    // signal pour le HUD
}


void Player::punch() {
    for (QGraphicsItem* item : scene()->items()) {
        Enemy* enemy = dynamic_cast<Enemy*>(item);
        if (enemy) {
            qreal distance = QLineF(pos(), enemy->pos()).length();
            if (distance < 40) {
                enemy->takeDamage(10);
                enemy->showHitEffect(); 
                qDebug() << "L'ennemi a été frappé à la mano ! PV restants :" << enemy->getHealth();
                return;
            }
        }
    }
}

void Player::shoot(QPointF mouseScenePos){
    
    Weapon* weapon = getCurrentWeapon();
    MyScene* sc = qobject_cast<MyScene*>(scene());

    if (!weapon || !sc) return;

    // --- PISTOL -------------------------------------------------------------
    if (weapon->getType() == "Pistol") {

        if (weapon->getAmmo() < 1 || !sc) return;               // pas de balle ou pas de scène
        
        else{
            sc->addProjectile(mouseScenePos);
            weapon->setAmmo(weapon->getAmmo() - 1);
            emit weapon->ammoChanged(weapon->getAmmo());
            qDebug() << "Tir pistolet - munitions restantes:" << weapon->getAmmo();
        }
        return;
    }

    // --- SHOTGUN ------------------------------------------------------------
    if (weapon->getType() == "Shotgun") {
        if (weapon->getAmmo() < 3 || !sc) return;               // il faut au moins 3 balles pour tirer au shotgun

        // directions : 0°, +30°, -30° (facile à ajuster)
        QList<QPointF> dirs;
        if (direction == "right") {
            dirs << QPointF(1, 0) << QPointF(1, -0.5) << QPointF(1, 0.5);
        } else if (direction == "left") {
            dirs << QPointF(-1, 0) << QPointF(-1, -0.5) << QPointF(-1, 0.5);
        } else if (direction == "up") {
            dirs << QPointF(0, -1) << QPointF(-0.5, -1) << QPointF(0.5, -1);
        } else { // down
            dirs << QPointF(0, 1) << QPointF(-0.5, 1) << QPointF(0.5, 1);
        }

        // Crée les 3 projectiles
        for (const QPointF& d : dirs) {
            QPointF norm = d / std::hypot(d.x(), d.y());
            sc->addProjectileDir(norm, 6, 30, 100); 

        }
        weapon->setAmmo(weapon->getAmmo() - 3);
        emit weapon->ammoChanged(weapon->getAmmo());
        qDebug() << "Tir shotgun - munitions restantes:" << weapon->getAmmo();
    }
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

int Player::getKillCount() const {
    return killCount; // Retourne le nombre de kills du joueur
}

bool Player::getHasWeapon() const {
    return hasWeapon; // Retourne si le joueur a une arme
}

Weapon* Player::getCurrentWeapon() const{
    return inventory[currentSlot];
}

int Player::getAmmo(Slot s) const{

    return inventory[s] ? inventory[s]->getAmmo() : -1;

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

    emit healthChanged(health);  
}


QPainterPath Player::shape() const {
    QPainterPath path;
    path.addRect(boundingRect()); // Utilise la même forme que le rectangle de collision
    return path;
}