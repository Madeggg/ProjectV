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
    p->setSprite(new QPixmap("img/laser.png"));
    p->setRotation(degrees);

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

        // Ajouter des points au score du joueur lorsque l'ennemi meurt
        MyScene* myScene = dynamic_cast<MyScene*>(scene());
        if (myScene) {
            myScene->addScore(10);
           
        }
        scene()->removeItem(this);
        deleteLater();
        
        if(targetPlayer){
            targetPlayer->incrementKillCount();     // Incrémente le compteur de kills du joueur
        }
    }
}

bool Enemy::canMoveInDirection(const QPointF& direction){
     // Calcul de la nouvelle position après le déplacement
    QPointF newPos = pos() + direction * speed;

    // Créer un rectangle de collision pour la position suivante
    QRectF nextBoundingRect(newPos, QSizeF(boundingRect().width(), boundingRect().height()));

    // Vérifier les collisions dans la direction souhaitée
    QList<QGraphicsItem*> collisions = collidingItems();
    for (QGraphicsItem* item : collisions) {
        // Si un élément de type "collision" est trouvé, l'ennemi ne peut pas se déplacer dans cette direction
        QVariant typeData = item->data(0);
        if (typeData.toString() == "collision") {
            return false;  // L'ennemi rencontre un obstacle
        }
    }

    // Vérifier si la nouvelle position est hors de la scène
    if (!scene()->sceneRect().contains(newPos)) {
        return false;  // L'ennemi serait hors de la scène
    }

    return true;  // L'ennemi peut se déplacer dans cette direction
}


void Enemy::moveTowardsPlayer(const QPointF& playerPos) {
    // Utiliser la position absolue du joueur dans la scène
    QPointF direction = playerPos - pos(); // position actuelle de l'ennemi
    qreal distance = std::sqrt(direction.x() * direction.x() + direction.y() * direction.y());  // Distance

    // Si l'ennemi est trop proche du joueur, on inflige des dégâts
    if(getType() == "Physique" && distance < 40) {
        doDamage(targetPlayer);  // Infliger des dégâts au joueur
        return;
    }

    if(getType() == "Distance" && distance < 100) {
        doDamage(targetPlayer);  // Infliger des dégâts au joueur
    }
    // Si l'ennemi n'est pas trop proche, il doit éviter les obstacles et se déplacer
    if (distance > 0) {
        direction /= distance;  // Normalisation de la direction

        // On crée une variable pour savoir si l'ennemi a trouvé une direction libre
        bool canMove = false;
        
        // Essayer différentes directions (haut, bas, gauche, droite)
        if (canMoveInDirection(direction)) {
            setPos(x() + direction.x() * speed, y() + direction.y() * speed);  // Déplacer l'ennemi
            canMove = true;
        } else {
            // Si la direction vers le joueur est bloquée, l'ennemi va tenter de se déplacer latéralement ou verticalement
            // Tester une direction alternative (haut, bas, gauche, droite)
            if (canMoveInDirection(QPointF(1, 0))) {  // Droite
                setPos(x() + speed, y());
                canMove = true;
            } else if (canMoveInDirection(QPointF(-1, 0))) {  // Gauche
                setPos(x() - speed, y());
                canMove = true;
            } else if (canMoveInDirection(QPointF(0, 1))) {  // Bas
                setPos(x(), y() + speed);
                canMove = true;
            } else if (canMoveInDirection(QPointF(0, -1))) {  // Haut
                setPos(x(), y() - speed);
                canMove = true;
            }
        }

        // Si aucune direction n'est libre, l'ennemi ne bouge pas
        // if (!canMove) {
        //     qDebug() << "Aucune direction libre trouvée pour l'ennemi.";
        // }
    }
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

Weapon::Weapon(int damage, QString type, QGraphicsItem* parent) : QGraphicsPixmapItem(parent){
    setDamage(damage);
    setType(type);
}

int Weapon::getDamage() const{
    return damage;
}


QString Weapon::getType() const{
    return type;
}

void Weapon::setDamage(int newDamage){
    damage = newDamage;
}


void Weapon::setType(QString newType){
    type = newType;
}

void Weapon::setSprite(QPixmap* newSprite){
    sprite = newSprite;
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

int Projectile::getDamage() const {
    return damage;
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

