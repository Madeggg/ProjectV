// Cette page sert à tout ce qui est hostile (enemies, bombes etc..) + les projectiles tirés par joueur ou ennemis + les armes

#include "Entities.h"
#include <QGraphicsScene>
#include <QPixmap>
#include <QRandomGenerator>
#include <QStringList>

// Méthodes de Enemy

Enemy::Enemy(QString type,QGraphicsItem* parent, Player* player) : QGraphicsPixmapItem(parent), targetPlayer(player), health(100), damage(10), speed(6) {
    setApperance(type);     // Définit l'apparence de l'ennemi en fonction de son type
    setShapeMode(QGraphicsPixmapItem::BoundingRectShape); 
    setDistance(false); // Par défaut, l'ennemi n'attaque pas à distance
    setType(type); 
    setWeapon(nullptr); // L'ennemi n'a pas d'arme par défaut
    setData(0,"enemy"); //Pour les collisions
  
    attackTimer = new QTimer(this);
    attackTimer->setInterval(1000); // 1 seconde entre deux attaques
    attackTimer->setSingleShot(true);
    connect(attackTimer, &QTimer::timeout, this, [this]() {
        canAttack = true;
    });

    // Initialiser le timer de mise à jour toutes les 100 ms
    movementTimer = new QTimer(this);
    connect(movementTimer, &QTimer::timeout, this, [this]() {
        if (targetPlayer) {
            moveTowardsPlayer(targetPlayer->pos());  // Met à jour la direction
        }
    });
    movementTimer->start(100);  // Toutes les 100ms

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

bool Enemy::getIsDead() const {
    return isDead;
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

void Enemy::setType(QString newType) {
    type = newType;
}

void Enemy::setApperance(QString type) {
    if(type == "Physique"){
        setPixmap(QPixmap("img/sprite_ennemi_static.png").scaled(40, 40));
    }
}


void Enemy::setDistance(bool newDistance) {
    distance = newDistance;
}

void Enemy::punch(Player* player){
    player->takeDamage(damage); 
    qDebug()<<"Le joueur a pris des dégats ! PV restants : "<<player->getHealth();  
    player->setPixmap(QPixmap("img/billyTriste.jpg").scaled(40, 40));

    // Rétablir le sprite normal après 100 ms
    QTimer::singleShot(100, player, [player]() {
        QString dir = player->getDirection();
        QString path = QString("img/Sprite_billy_%1.png").arg(dir);  // par ex: Sprite_billy_left.png
        player->setPixmap(QPixmap(path).scaled(40, 40));
    });
                    
}

void Enemy::shoot(Player* player){
    player->takeDamage(weapon->getDamage()); 
}

void Enemy::doDamage(Player* player) {
    if (!canAttack) return;

    canAttack = false;
    attackTimer->start();

    if (getType() == "Physique") {
        punch(player);
    } else if (getType() == "Distance") {
        shoot(player);
    }
}

void Enemy::takeDamage(int amount) {
    if(isDead) return;

    health -= amount;
    if (health <= 0) {
        isDead = true; 
        qDebug() << "L'ennemi est mort. Supression de la scène";
        scene()->removeItem(this);
        deleteLater();
    }
}

void Enemy::moveTowardsPlayer(const QPointF& playerPos) {
    
    // Utiliser la position absolue du joueur dans la scène
    QPointF direction = playerPos - pos(); // position actuelle de l'ennemi
    qreal distance = std::sqrt(direction.x() * direction.x() + direction.y() * direction.y());  // Distance

    // Ne pas se déplacer si on est déjà assez proche
    if(getType() == "Physique"){
        if (distance < 40){
            doDamage(targetPlayer);     // Infliger des dégâts au joueur
            return;
        }
    }
    

    if (distance > 0) {
        direction /= distance;  // Normalisation de la direction
        setPos(x() + direction.x() * speed, y() + direction.y() * speed);  // Déplacer l'ennemi
    }
    

}




// //Méthodes de Soldier

// Soldier::Soldier(QString type,QGraphicsItem* parent, Weapon* w) :  QGraphicsPixmapItem(parent), type(type){
//     setHealth(50);
//     setDamage(10);
//     setSpeed(2);
//     setWeapon(w); // L'ennemi n'a pas d'arme par défaut
    
// }




// //Méthodes de Hero

// Hero::Hero(QString type,QGraphicsItem* parent,Weapon* w) : QGraphicsPixmapItem(parent), type(type){
//     setHealth(100);
//     setDamage(30);
//     setSpeed(5);
//     setWeapon(w);
// }


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

    if (!scene()) return; 

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
            deleteLater();  // Supprimer le projectile
            return;  // Quitter la méthode après la suppression
        }

    }
   
    if (!scene()->sceneRect().contains(pos())) {
        qDebug() << "Projectile hors de la scène. Suppression.";
        scene()->removeItem(this);
        deleteLater(); 
    }
}

