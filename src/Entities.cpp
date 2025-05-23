// Cette page sert à tout ce qui est hostile (enemies, bombes etc..) + les projectiles tirés par joueur ou ennemis + les armes

#include "Entities.h"
#include <QGraphicsScene>
#include <QPixmap>
#include <QRandomGenerator>
#include <QStringList>

// Méthodes de Enemy

Enemy::Enemy(QString type,QGraphicsItem* parent, Player* player) : QGraphicsPixmapItem(parent), targetPlayer(player), health(40), damage(0), speed(6) {
    setShapeMode(QGraphicsPixmapItem::BoundingRectShape); 
    setDistance(false); // Par défaut, l'ennemi n'attaque pas à distance
    setType(type); 
    setWeapon(nullptr); // L'ennemi n'a pas d'arme par défaut
    setData(0,"enemy"); //Pour les collisions
  
    attackTimer = new QTimer(this);
    attackTimer->setInterval(2000); // 2 seconde entre deux attaques
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

    loadAnimations(); // Charger les animations

} 

void Enemy::loadAnimations() {

    //Zombie
    QPixmap sheet("img/WalkZombie.png");
    QPixmap meleeSheet("img/AttackZombie.png");
    QPixmap deathSheet("img/DeathZombie.png");
    QPixmap idleSheet("img/IdleZombie.png");

    //Vampire
    QPixmap sheetVampire("img/Vampires3_Walk_full.png");
    QPixmap meleeSheetVampire("img/Vampires3_Attack_full.png");
    QPixmap deathSheetVampire("img/Vampires3_Death_full.png");

    //Tous nos sprites sont en taille 32*32
    int frameWidth = 32;
    int frameHeight = 32;

    for (int i = 0; i < 10; ++i) {
        walkFront.append(new QPixmap(sheet.copy(i * frameWidth, 0 * frameHeight, frameWidth, frameHeight))); // down
        walkBack.append(new QPixmap(sheet.copy(i * frameWidth, 1 * frameHeight, frameWidth, frameHeight))); // right
        walkRight.append(new QPixmap(sheet.copy(i * frameWidth, 2 * frameHeight, frameWidth, frameHeight)));  // left
        walkLeft.append(new QPixmap(sheet.copy(i * frameWidth, 3 * frameHeight, frameWidth, frameHeight)));  // up
    }

    for (int i = 0; i < 8; ++i) {
        meleeFront.append(new QPixmap(meleeSheet.copy(i * frameWidth, 0 * frameHeight, frameWidth, frameHeight))); // down
        meleeBack.append(new QPixmap(meleeSheet.copy(i * frameWidth, 1 * frameHeight, frameWidth, frameHeight))); // right
        meleeRight.append(new QPixmap(meleeSheet.copy(i * frameWidth, 2 * frameHeight, frameWidth, frameHeight)));  // left
        meleeLeft.append(new QPixmap(meleeSheet.copy(i * frameWidth, 3 * frameHeight, frameWidth, frameHeight)));  // up
    }
        

    for (int i = 0; i < 7; ++i) {
        deathFront.append(new QPixmap(deathSheet.copy(i * frameWidth, 0 * frameHeight, frameWidth, frameHeight))); // down
        deathBack.append(new QPixmap(deathSheet.copy(i * frameWidth, 1 * frameHeight, frameWidth, frameHeight))); // right
        deathRight.append(new QPixmap(deathSheet.copy(i * frameWidth, 2 * frameHeight, frameWidth, frameHeight)));  // left
        deathLeft.append(new QPixmap(deathSheet.copy(i * frameWidth, 3 * frameHeight, frameWidth, frameHeight)));  // up
    }

    int frameWidthWalk = sheetVampire.width() / 6;  // 6 colonnes pour le vampire
    int frameHeightWalk = sheetVampire.height() / 4; // 4 lignes pour le vampire

    for(int i = 0; i < 6; ++i) {
        walkFrontVampire.append(new QPixmap(sheetVampire.copy(i * frameWidthWalk, 0 * frameHeightWalk, frameWidthWalk, frameHeightWalk))); // down
        walkBackVampire.append(new QPixmap(sheetVampire.copy(i * frameWidthWalk, 1 * frameHeightWalk, frameWidthWalk, frameHeightWalk))); // left
        walkLeftVampire.append(new QPixmap(sheetVampire.copy(i * frameWidthWalk, 2 * frameHeightWalk, frameWidthWalk, frameHeightWalk))); // right
        walkRightVampire.append(new QPixmap(sheetVampire.copy(i * frameWidthWalk, 3 * frameHeightWalk, frameWidthWalk, frameHeightWalk))); // up
    }

    int frameWidthAttack = meleeSheetVampire.width() / 12;  // 12 colonnes pour le vampire
    int frameHeightAttack = meleeSheetVampire.height() / 4; // 4 lignes pour le vampire


    for(int i = 0; i < 12; ++i) {
        shootFront.append(new QPixmap(meleeSheetVampire.copy(i * frameWidthAttack, 0 * frameHeightAttack, frameWidthAttack, frameHeightAttack))); // down
        shootBack.append(new QPixmap(meleeSheetVampire.copy(i * frameWidthAttack, 1 * frameHeightAttack, frameWidthAttack, frameHeightAttack))); // left
        shootLeft.append(new QPixmap(meleeSheetVampire.copy(i * frameWidthAttack, 2 * frameHeightAttack, frameWidthAttack, frameHeightAttack))); // right
        shootRight.append(new QPixmap(meleeSheetVampire.copy(i * frameWidthAttack, 3 * frameHeightAttack, frameWidthAttack, frameHeightAttack))); // up
    }

    int frameWidthDeath = deathSheetVampire.width() / 11;  
    int frameHeightDeath = deathSheetVampire.height() / 4; // 4 lignes pour le vampire

    for(int i = 0; i < 11; ++i) {
        deathFrontVampire.append(new QPixmap(deathSheetVampire.copy(i * frameWidthDeath, 0 * frameHeightDeath, frameWidthDeath, frameHeightDeath))); // down
        deathBackVampire.append(new QPixmap(deathSheetVampire.copy(i * frameWidthDeath, 1 * frameHeightDeath, frameWidthDeath, frameHeightDeath))); // left
        deathLeftVampire.append(new QPixmap(deathSheetVampire.copy(i * frameWidthDeath, 2 * frameHeightDeath, frameWidthDeath, frameHeightDeath))); // right
        deathRightVampire.append(new QPixmap(deathSheetVampire.copy(i * frameWidthDeath, 3 * frameHeightDeath, frameWidthDeath, frameHeightDeath))); // up
    }



}

void Enemy::playWalkAnimation() {
    QVector<QPixmap*>* currentAnim = nullptr;
    QString direction = facingDirection;


    if(canPlayAnimation){
        if(type == "Physique"){
        

        if (direction == "down") {
            currentAnim = &walkFront;
        } else if (direction == "up") {
            currentAnim = &walkBack;
        } else if (direction == "left") {
            currentAnim = &walkLeft;
        } else if (direction == "right") {
            currentAnim = &walkRight;
        }

        
    }
    else if(type == "Distance"){
       
        if (direction == "down") {
            currentAnim = &walkFrontVampire;
        } else if (direction == "up") {
            currentAnim = &walkBackVampire;
        } else if (direction == "left") {
            currentAnim = &walkLeftVampire;
        } else if (direction == "right") {
            currentAnim = &walkRightVampire;
        }
    }

    if (currentAnim && !currentAnim->isEmpty()) {
            currentFrame = (currentFrame + 1) % currentAnim->size();
            setPixmap(*(*currentAnim)[currentFrame]);
        }
    }
}

    

void Enemy::playAttackAnimation() {
    QVector<QPixmap*>* currentAnim = nullptr;
    QString direction = facingDirection;


    if(canPlayAnimation){
         if(type == "Physique"){
        if (direction == "down") {
            currentAnim = &meleeFront;
        } else if (direction == "up") {
            currentAnim = &meleeBack;
        } else if (direction == "left") {
            currentAnim = &meleeLeft;
        } else if (direction == "right") {
            currentAnim = &meleeRight;
        }

        
    }
    else if(type=="Distance"){
        if (direction == "down") {
            currentAnim = &shootFront;
        } else if (direction == "up") {
            currentAnim = &shootBack;
        } else if (direction == "left") {
            currentAnim = &shootLeft;
        } else if (direction == "right") {
            currentAnim = &shootRight;
        }

       
    }

    if (currentAnim && !currentAnim->isEmpty()) {
            for (int i = 0; i < currentAnim->size(); ++i) {
                QTimer::singleShot(i * 100, this, [this, currentAnim, i]() {
                    setPixmap(*(*currentAnim)[i]);
                });
            }
        }
    }
    
   
}


void Enemy::playDeathAnimation() {
    QVector<QPixmap*>* currentAnim = nullptr;
    QString direction = facingDirection;

     
    canPlayAnimation = false;


    if(type == "Physique"){
        if (direction == "down") {
            currentAnim = &deathFront;
        } else if (direction == "up") {
            currentAnim = &deathBack;
        } else if (direction == "left") {
            currentAnim = &deathLeft;
        } else if (direction == "right") {
            currentAnim = &deathRight;
        }
    }

    else if(type == "Distance"){
        if (direction == "down") {
            currentAnim = &deathFrontVampire;
        } else if (direction == "up") {
            currentAnim = &deathBackVampire;
        } else if (direction == "left") {
            currentAnim = &deathLeftVampire;
        } else if (direction == "right") {
            currentAnim = &deathRightVampire;
        }
    }

    
    if (currentAnim && !currentAnim->isEmpty()) {
        for (int i = 0; i < currentAnim->size(); ++i) {
            QTimer::singleShot(i * 100, this, [this, currentAnim, i]() {
                setPixmap(*(*currentAnim)[i]);
            });
        }
    }
}

void Enemy::updateFacingDirection() {
    if (!targetPlayer) return;

    QPointF delta = targetPlayer->pos() - pos();
    if (std::abs(delta.x()) > std::abs(delta.y())) {
        facingDirection = delta.x() > 0 ? "right" : "left";
    } else {
        facingDirection = delta.y() > 0 ? "down" : "up";
    }
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




void Enemy::setDistance(bool newDistance) {
    distance = newDistance;
}

void Enemy::punch(Player* player){
    if (!canAttack) return;  // Empêche de frapper trop souvent
    canAttack = false;

    updateFacingDirection(); 
    playAttackAnimation();  
    player->takeDamage(damage); 
    player->showHitEffect(); 
    qDebug() << "Le joueur a pris des dégats ! PV restants : " << player->getHealth();  

    attackTimer->start();  // Redémarre le timer pour autoriser la prochaine attaque dans 2 sec
}


void Enemy::shoot(Player* player) {
    if (!scene() || !player || !canShoot) return;
    canShoot = false;

    updateFacingDirection(); 
    // Joue l'animation de tir
    playAttackAnimation();

    // Calcul du délai basé sur le nombre de frames
    int frameCount = 12;     // ← nombre de frames dans l'animatiojn shoot
    int frameDuration = 100; // ← durée d'une frame en ms (comme dans playAttackAnimation)
    int totalDuration = frameCount * frameDuration;

    QTimer::singleShot(totalDuration, this, [this, player]() {
        if (!scene() || !player) return;
        
        //Pour faire partir le projectile du centre du sprite de l'ennemi (MARCHE PAS)
        QPointF start = pos() + QPointF(pixmap().width() / 2, pixmap().height() / 2);
       

        QPointF targetCenter = player->pos() + QPointF(player->boundingRect().width()/2, player->boundingRect().height()/2);
        QPointF direction = targetCenter - start;

        qreal length = std::sqrt(direction.x() * direction.x() + direction.y() * direction.y());
        if (length == 0) return;
        direction /= length;

        qreal angle = std::atan2(direction.y(), direction.x());
        qreal degrees = angle * 180 / M_PI;

        Projectile* p = new Projectile(start, direction, 5, 20); 
        p->setSprite(new QPixmap("img/laserBeam.png"));
        p->setRotation(degrees);
        p->setMaxDistance(500);
        p->setSource("enemy");

        scene()->addItem(p);

        // Autorise le prochain tir après un délai (si nécessaire)
        QTimer::singleShot(1500, this, [this]() {
            canShoot = true;
        });
    });
}







void Enemy::takeDamage(int amount) {
    if (isDead) return;

    health -= amount;
    if (health <= 0 && !isDead) {
        isDead = true;
        playDeathAnimation();
        qDebug() << "L'ennemi est mort. Suppression de la scène";

        if(type == "Physique"){
            deathDuration = 7*100;  // 7 frames à 100ms pour le zombie
        }
        else if (type == "Distance"){
            deathDuration = 11*100;  // 11 frames à 100ms pour le vampire
        }
 

        QTimer::singleShot(deathDuration, this, [this]() {
            if (scene()) scene()->removeItem(this);
                deleteLater();
        });

        if (targetPlayer) {
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
    playWalkAnimation(); // Joue l'animation de marche
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
    updateFacingDirection(); 
    playWalkAnimation();

    QPointF direction = target - pos();
    qreal dist = std::hypot(direction.x(), direction.y());

  

    // S'il est trop proche, on n'avance plus et on attaque
    if (dist <= 40) {
        if(type == "Physique"){
            punch(targetPlayer); // Attaque le joueur
            return;
        }
        
    }

    if(dist <= 200){
        if(type == "Distance"){
            shoot(targetPlayer); // Attaque le joueur
            return;
        }
    }

    // Sinon, on se rapproche du joueur
    direction /= dist; // normalisation
    if (canMoveInDirection(direction)) {
        setPos(x() + direction.x() * speed, y() + direction.y() * speed);
    }

    

}

void Enemy::showHitEffect() {
    auto* effect = new QGraphicsColorizeEffect(this);
    effect->setColor(Qt::red);
    effect->setStrength(1.0);
    setGraphicsEffect(effect);

    QTimer::singleShot(100, this, [this, effect]() {
        if (graphicsEffect() == effect) {
            setGraphicsEffect(nullptr);
        }
        // Pas besoin de delete manuellement, Qt s'en occupe grâce au parent
    });
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

