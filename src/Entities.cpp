// Cette page sert à tout ce qui est hostile (enemies, bombes etc..) + les projectiles tirés par joueur ou ennemis + les armes

#include "Entities.h"
#include <QGraphicsScene>
#include <QPixmap>
#include <QRandomGenerator>
#include <QStringList>

// Méthodes de Enemy

Enemy::Enemy(QString type,QGraphicsItem* parent, Player* player) : QGraphicsPixmapItem(parent), targetPlayer(player), health(10), damage(0), speed(6) {
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
            moveTowardsPlayerOrWander(targetPlayer);  // Met à jour la direction
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
    else if(type == "Distance"){
        setPixmap(QPixmap("img/sprite_homelander_static.png").scaled(40, 40));
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

void Enemy::shoot(Player* player) {
    if (!scene() || !player) return;

    qDebug()<<"appel a shoot de enemy";

    QPointF start = pos() + QPointF(boundingRect().width()/2, boundingRect().height()/2);
    QPointF direction = player->pos() - start;

    qreal length = std::sqrt(direction.x() * direction.x() + direction.y() * direction.y());
    if (length == 0) return;
    direction /= length;

    qreal angle = std::atan2(direction.y(), direction.x());
    qreal degrees = angle * 180 / M_PI;

    Projectile* p = new Projectile(start, direction, 5,20); 
    p->setSprite(new QPixmap("img/laserBeam.png"));
    p->setRotation(degrees);
    p->setMaxDistance(500); // Distance maximale du projectile
    p->setSource("enemy");  // Définit la source du projectile

    scene()->addItem(p);
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
    if (isDead) return;

    health -= amount;
    if (health <= 0) {
        isDead = true;
        qDebug() << "L'ennemi est mort. Suppression de la scène";

        scene()->removeItem(this);
        deleteLater();

        if(targetPlayer){
            targetPlayer->checkKillCount();
        }
    }
}


bool Enemy::canMoveInDirection(const QPointF& direction){
    if (!scene()) return false;

    // Prochaine position en fonction de la direction et de la vitesse
    QPointF newPos = pos() + direction * speed;
    QRectF futureRect(newPos, boundingRect().size());

    // Vérifie s’il y aurait une collision à cette future position
    QList<QGraphicsItem*> items = scene()->items(futureRect);

    for (QGraphicsItem* item : items) {
        if (item == this) continue;
        if (item->data(0).toString() == "collision") {
            return false; // Un mur ou un obstacle est détecté
        }
    }

    // Vérifie les limites de la scène
    if (!scene()->sceneRect().contains(futureRect)) {
        return false;
    }

    return true;
}


bool Enemy::hasLineOfSightTo(Player* player) {
    if (!player || !scene()) return false;

    QLineF line(pos(), player->pos());  // Ligne entre l'ennemi et le joueur

    // Crée une bounding box englobant la ligne 
    QRectF boundingRect = QRectF(line.p1(), QSizeF(line.p2().x() - line.p1().x(),
                                                   line.p2().y() - line.p1().y())).normalized();

    QList<QGraphicsItem*> itemsOnPath = scene()->items(boundingRect);

    // Crée un chemin représentant la ligne
    QPainterPath linePath;
    linePath.moveTo(line.p1());
    linePath.lineTo(line.p2());

    for (QGraphicsItem* item : itemsOnPath) {
        if (item == this || item == player) continue;

        if (item->data(0).toString() == "collision") {
            QPainterPath shape = item->shape();
            shape.translate(item->pos()); // Adapter à la scène

            if (shape.intersects(linePath)) {
                return false; // Un obstacle bloque la ligne
            }
        }
    }

    return true; // Aucun obstacle ne bloque la ligne 
}


void Enemy::wander() {
    QList<QPointF> directions = {
        QPointF(1, 0), QPointF(-1, 0), QPointF(0, 1), QPointF(0, -1)
    };
    std::shuffle(directions.begin(), directions.end(), *QRandomGenerator::global());

    for (const QPointF& dir : directions) {
        if (canMoveInDirection(dir)) {
            setPos(x() + dir.x() * speed, y() + dir.y() * speed);
            return;
        }
    }
}

void Enemy::moveTowardsPlayerOrWander(Player* player) {
    if (hasLineOfSightTo(player)) {
        moveTowards(player->pos());
    } else {
        wander();
    }
}

void Enemy::moveTowards(QPointF target) {
    QPointF direction = target - pos();
    qreal dist = std::hypot(direction.x(), direction.y());
    if (dist > 0) {
        direction /= dist;
        if (canMoveInDirection(direction)) {
            setPos(x() + direction.x() * speed, y() + direction.y() * speed);
        }
    }
}

void Enemy::showHitEffect() {

    if(getType() == "Physique"){
         this->setPixmap(QPixmap("img/sprite_ennemi_static_hit.png").scaled(40, 40));
    }
    else if (getType() == "Distance"){
        this->setPixmap(QPixmap("img/sprite_homelander_static_hit.png").scaled(40, 40));
    }
   

    QTimer* timer = new QTimer(this);
    timer->setSingleShot(true);
    QObject::connect(timer, &QTimer::timeout, this, [this]() {
        if (!this->getIsDead()) {
            if(getType() == "Physique"){
                this->setPixmap(QPixmap("img/sprite_ennemi_static.png").scaled(40, 40));
            }
            else if(getType() == "Distance"){
                this->setPixmap(QPixmap("img/sprite_homelander_static.png").scaled(40, 40));
            }
            
        }
    });
    timer->start(100); // 100ms d'effet visuel
}


 QRectF Enemy::boundingRect() const {
    return QRectF(0, 0, 40, 40);  // Utiliser la taille réelle de l'ennemi
}

 QPainterPath Enemy::shape() const{
        QPainterPath path;
        path.addRect(boundingRect());  // Forme de collision = rectangle de l'image
        return path;
    
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

Weapon::Weapon(QString type, QGraphicsItem* parent) : QGraphicsPixmapItem(parent){
    setType(type);
}



QString Weapon::getType() const{
    return type;
}




void Weapon::setType(QString newType){
    type = newType;
}

void Weapon::setSprite(QPixmap* newSprite){
    setPixmap(newSprite->scaled(30, 30));
}

void Weapon::setAmmo(int newAmmo) {
    if (ammo != newAmmo) {
        ammo = newAmmo;
        emit ammoChanged(ammo);  // Notification
    }
}

int Weapon::getAmmo() const{
    return ammo;
}




//Méthodes de Projectile

Projectile::Projectile(QPointF startPos, QPointF direction, int speed, int damage, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent), speed(speed) {
    
    setPos(startPos);

    // Définit la direction normalisée
    setDirection(direction);

    setDamage(damage);

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
    setPixmap(newSprite->scaled(20, 20));
}

void Projectile::setDamage(int newDamage) {
    damage = newDamage;
}

void Projectile::setSource(QString newSource) {
    source = newSource;
}

void Projectile::setMaxDistance(qreal newMaxDistance) {
    maxDistance = newMaxDistance;
}

int Projectile::getDamage() const {
    return damage;
}

QString Projectile::getSource() const {
    return source;
}

void Projectile::move() {
    if (!scene()) return;

    // Calcul du déplacement
    QPointF delta = direction * speed;
    setPos(pos() + delta);

    // 🔁 Mise à jour de la distance parcourue
    distanceTraveled += std::sqrt(delta.x() * delta.x() + delta.y() * delta.y());

    // ❌ Si le projectile a dépassé sa portée maximale, on le supprime
    if (distanceTraveled >= maxDistance) {
        qDebug() << "Projectile supprimé (portée max atteinte)";
        scene()->removeItem(this);
        deleteLater();
        return;
    }

    // 🔎 Vérification des collisions
    QList<QGraphicsItem*> collisions = collidingItems();
    for (QGraphicsItem* item : collisions) {
        if (!item) continue;

        QVariant typeData = item->data(0);

        if (typeData.toString() == "collision") {
            qDebug() << "Collision avec un objet de type 'collision' à :" << item->pos();
            scene()->removeItem(this);
            deleteLater();
            return;
        }
    }

    // ❌ Si le projectile sort de la scène, on le supprime aussi
    if (!scene()->sceneRect().contains(pos())) {
        qDebug() << "Projectile hors de la scène. Suppression.";
        scene()->removeItem(this);
        deleteLater();
    }
}

