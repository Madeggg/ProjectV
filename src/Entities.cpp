// Cette page sert à tout ce qui est hostile (enemies, bombes etc..) + les projectiles tirés par joueur ou ennemis + les armes

#include "Entities.h"


// Méthodes de Enemy

Enemy::Enemy(QGraphicsItem* parent, Player* player, int health, int damage, int speed)
    : QGraphicsPixmapItem(parent), targetPlayer(player), health(health), damage(damage), speed(speed)
{
    setShapeMode(QGraphicsPixmapItem::BoundingRectShape); 
    setData(0,"enemy"); // Pour les collisions

    attackTimer = new QTimer(this);
    attackTimer->setInterval(2000);
    attackTimer->setSingleShot(true);
    connect(attackTimer, &QTimer::timeout, this, [this]() {
        canAttack = true;
    });

    movementTimer = new QTimer(this);
    connect(movementTimer, &QTimer::timeout, this, [this]() {
        if (targetPlayer) {
            moveTowardsPlayerOrWander(targetPlayer);
        }
    });
    movementTimer->start(100);

   
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



void Enemy::moveTowardsPlayerOrWander(Player* player) {
    if (hasLineOfSightTo(player)) {
        moveTowards(player->pos());
    } else {
        wander();
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

 //==================================ZOMBIE==================================

Zombie::Zombie(QGraphicsItem* parent, Player* player) 
    : Enemy(nullptr,player, 60, 1, 5) {
         loadAnimations(); // Charger les animations
}


void Zombie::loadAnimations() {
    // Charger les sprites pour les animations

    QPixmap walkSheet("img/WalkZombie.png");
    QPixmap attackSheet("img/AttackZombie.png");
    QPixmap deathSheet("img/DeathZombie.png");
    QPixmap idleSheet("img/IdleZombie.png");

    int frameWidthWalk = 32;
    int frameHeightWalk =  32;



     for (int i = 0; i < 10; ++i) {
        walkFront.append(new QPixmap(walkSheet.copy(i * frameWidthWalk, 0 * frameHeightWalk, frameWidthWalk, frameHeightWalk))); // down
        walkBack.append(new QPixmap(walkSheet.copy(i * frameWidthWalk, 1 * frameHeightWalk, frameWidthWalk, frameHeightWalk))); // right
        walkRight.append(new QPixmap(walkSheet.copy(i * frameWidthWalk, 2 * frameHeightWalk, frameWidthWalk, frameHeightWalk)));  // left
        walkLeft.append(new QPixmap(walkSheet.copy(i * frameWidthWalk, 3 * frameHeightWalk, frameWidthWalk, frameHeightWalk)));  // up
    }

    int frameWidthAttack = 32;
    int frameHeightAttack = 32;

    for (int i = 0; i < 8; ++i) {
        attackFront.append(new QPixmap(attackSheet.copy(i * frameWidthAttack, 0 * frameHeightAttack, frameWidthAttack, frameHeightAttack))); // down
        attackBack.append(new QPixmap(attackSheet.copy(i * frameWidthAttack, 1 * frameHeightAttack, frameWidthAttack, frameHeightAttack))); // right
        attackRight.append(new QPixmap(attackSheet.copy(i * frameWidthAttack, 2 * frameHeightAttack, frameWidthAttack, frameHeightAttack)));  // left
        attackLeft.append(new QPixmap(attackSheet.copy(i * frameWidthAttack, 3 * frameHeightAttack, frameWidthAttack, frameHeightAttack)));  // up
    }
        
    int frameWidthDeath = 32;
    int frameHeightDeath = 32;

    for (int i = 0; i < 7; ++i) {
        deathFront.append(new QPixmap(deathSheet.copy(i * frameWidthDeath, 0 * frameHeightDeath, frameWidthDeath, frameHeightDeath))); // down
        deathBack.append(new QPixmap(deathSheet.copy(i * frameWidthDeath, 1 * frameHeightDeath, frameWidthDeath, frameHeightDeath))); // right
        deathRight.append(new QPixmap(deathSheet.copy(i * frameWidthDeath, 2 *frameHeightDeath, frameWidthDeath, frameHeightDeath)));  // left
        deathLeft.append(new QPixmap(deathSheet.copy(i * frameWidthDeath, 3 * frameHeightDeath, frameWidthDeath, frameHeightDeath)));  // up
    }
}

void Zombie::playWalkAnimation() {
    QVector<QPixmap*>* currentAnim = nullptr;
    QString direction = facingDirection;

   
        if (direction == "down") {
            currentAnim = &walkFront;
        } else if (direction == "up") {
            currentAnim = &walkBack;
        } else if (direction == "left") {
            currentAnim = &walkLeft;
        } else if (direction == "right") {
            currentAnim = &walkRight;
        }
    

        if (currentAnim && !currentAnim->isEmpty()) {
                currentFrame = (currentFrame + 1) % currentAnim->size();
                setPixmap(*(*currentAnim)[currentFrame]);
        }
    
}

void Zombie::playAttackAnimation() {
    QVector<QPixmap*>* currentAnim = nullptr;
    QString direction = facingDirection;

    

        if (direction == "down") {
            currentAnim = &attackFront;
        } else if (direction == "up") {
            currentAnim = &attackBack;
        } else if (direction == "left") {
            currentAnim = &attackLeft;
        } else if (direction == "right") {
            currentAnim = &attackRight;
        }
    

        if (currentAnim && !currentAnim->isEmpty()) {
            for (int i = 0; i < currentAnim->size(); ++i) {
                QTimer::singleShot(i * 100, this, [this, currentAnim, i]() {
                    setPixmap(*(*currentAnim)[i]);
                });
            }
        }
    
    
}

void Zombie::playDeathAnimation() {
    QVector<QPixmap*>* currentAnim = nullptr;
    QString direction = facingDirection;

    

    if (direction == "down") {
        currentAnim = &deathFront;
    } else if (direction == "up") {
        currentAnim = &deathBack;
    } else if (direction == "left") {
        currentAnim = &deathLeft;
    } else if (direction == "right") {
        currentAnim = &deathRight;
    }

    if (currentAnim && !currentAnim->isEmpty()) {
            for (int i = 0; i < currentAnim->size(); ++i) {
                QTimer::singleShot(i * 100, this, [this, currentAnim, i]() {
                    setPixmap(*(*currentAnim)[i]);
                });
            }
        }
    
}

void Zombie::wander() {
    updateFacingDirection();
    playWalkAnimation(); // Joue l'animation de marche

    QList<QPointF> directions = {
        QPointF(1, 0), QPointF(-1, 0), QPointF(0, 1), QPointF(0, -1)
    };
    std::shuffle(directions.begin(), directions.end(), *QRandomGenerator::global());

    for (const QPointF& dir : directions) {
        if (canMoveInDirection(dir)) {
            setPos(x() + dir.x() * speed, y() + dir.y() * speed);
        }
    }

}


void Zombie::moveTowards(QPointF target) {

    if(!isDead){

    
        updateFacingDirection();
        playWalkAnimation();

        QPointF direction = target - pos();
        qreal dist = std::hypot(direction.x(), direction.y());

        // Stop walking animation if too close to attack
        if (dist <= 40) {
            attack(targetPlayer);
            return;
        }

        // Déplacement
        direction /= dist;
        if (canMoveInDirection(direction)) {
            setPos(x() + direction.x() * speed, y() + direction.y() * speed);

        }
    }    
}

void Zombie::moveTowardsPlayerOrWander(Player* player) {
    Enemy::moveTowardsPlayerOrWander(player); // Appelle la méthode de la classe de base
}

void Zombie::takeDamage(int amount) {
    if (isDead) return;

    health -= amount;
    if (health <= 0 && !isDead) {
        isDead = true;
        playDeathAnimation();
        qDebug() << "L'ennemi est mort. Suppression de la scène";

        deathDuration = 7*100;  // 7 frames à 100ms pour l'animation de mort du zombie 


        QTimer::singleShot(deathDuration, this, [this]() {
            if (scene()) scene()->removeItem(this);
                deleteLater();
        });

        if (targetPlayer) {
            targetPlayer->setLastKillPosition(pos()); // ← mémorise la position de l'ennemi avant de supprimer l’ennemi
            targetPlayer->checkKillCount();
        }

    }

}

void Zombie::attack(Player* player) {
    if (!canAttack) return;  // Empêche de frapper trop souvent
    canAttack = false;

    if(!isDead){

    

        updateFacingDirection(); 
        playAttackAnimation();  
        player->takeDamage(damage); 
        player->showHitEffect(); 
        qDebug() << "Le joueur a pris des dégats ! PV restants : " << player->getHealth();  

        attackTimer->start();  // Redémarre le timer pour autoriser la prochaine attaque dans 2 sec
    }
}


//==================================VAMPIRE==================================

Vampire::Vampire(QGraphicsItem* parent, Player* player) 
    : Enemy(nullptr,player, 100, 1, 6) {
         loadAnimations(); // Charger les animations
}

void Vampire::loadAnimations(){
    
    QPixmap walkSheet("img/Vampires3_Walk_full.png");
    QPixmap attackSheet("img/Vampires3_Attack_full.png");
    QPixmap deathSheet("img/Vampires3_Death_full.png");

    int frameWidthWalk = walkSheet.width() / 6;  
    int frameHeightWalk = walkSheet.height() / 4; 


     for (int i = 0; i < 6; ++i) {
        walkFront.append(new QPixmap(walkSheet.copy(i * frameWidthWalk, 0 * frameHeightWalk, frameWidthWalk, frameHeightWalk))); // down
        walkBack.append(new QPixmap(walkSheet.copy(i * frameWidthWalk, 1 * frameHeightWalk, frameWidthWalk, frameHeightWalk))); // right
        walkLeft.append(new QPixmap(walkSheet.copy(i * frameWidthWalk, 2 * frameHeightWalk, frameWidthWalk, frameHeightWalk)));  // left
        walkRight.append(new QPixmap(walkSheet.copy(i * frameWidthWalk, 3 * frameHeightWalk, frameWidthWalk, frameHeightWalk)));  // up
    }

    int frameWidthAttack = attackSheet.width() / 12;
    int frameHeightAttack = attackSheet.height() / 4;

    for (int i = 0; i < 12; ++i) {
        attackFront.append(new QPixmap(attackSheet.copy(i * frameWidthAttack, 0 * frameHeightAttack, frameWidthAttack, frameHeightAttack))); // down
        attackBack.append(new QPixmap(attackSheet.copy(i * frameWidthAttack, 1 * frameHeightAttack, frameWidthAttack, frameHeightAttack))); // right
        attackLeft.append(new QPixmap(attackSheet.copy(i * frameWidthAttack, 2 * frameHeightAttack, frameWidthAttack, frameHeightAttack)));  // left
        attackRight.append(new QPixmap(attackSheet.copy(i * frameWidthAttack, 3 * frameHeightAttack, frameWidthAttack, frameHeightAttack)));  // up
    }
        
    int frameWidthDeath = deathSheet.width() / 11;
    int frameHeightDeath = deathSheet.height() / 4;

    for (int i = 0; i < 11; ++i) {
        deathFront.append(new QPixmap(deathSheet.copy(i * frameWidthDeath, 0 * frameHeightDeath, frameWidthDeath, frameHeightDeath))); // down
        deathBack.append(new QPixmap(deathSheet.copy(i * frameWidthDeath, 1 * frameHeightDeath, frameWidthDeath, frameHeightDeath))); // right
        deathLeft.append(new QPixmap(deathSheet.copy(i * frameWidthDeath, 2 *frameHeightDeath, frameWidthDeath, frameHeightDeath)));  // left
        deathRight.append(new QPixmap(deathSheet.copy(i * frameWidthDeath, 3 * frameHeightDeath, frameWidthDeath, frameHeightDeath)));  // up
    }

}

void Vampire::playWalkAnimation() {
    QVector<QPixmap*>* currentAnim = nullptr;
    QString direction = facingDirection;

  
        if (direction == "down") {
            currentAnim = &walkFront;
        } else if (direction == "up") {
            currentAnim = &walkBack;
        } else if (direction == "left") {
            currentAnim = &walkLeft;
        } else if (direction == "right") {
            currentAnim = &walkRight;
        }
    

    if (currentAnim && !currentAnim->isEmpty()) {
            currentFrame = (currentFrame + 1) % currentAnim->size();
            setPixmap(*(*currentAnim)[currentFrame]);
    }
}

void Vampire::playAttackAnimation() {
    QVector<QPixmap*>* currentAnim = nullptr;
    QString direction = facingDirection;

    
        if (direction == "down") {
            currentAnim = &attackFront;
        } else if (direction == "up") {
            currentAnim = &attackBack;
        } else if (direction == "left") {
            currentAnim = &attackLeft;
        } else if (direction == "right") {
            currentAnim = &attackRight;
        }
    

    if (currentAnim && !currentAnim->isEmpty()) {
            for (int i = 0; i < currentAnim->size(); ++i) {
                QTimer::singleShot(i * 100, this, [this, currentAnim, i]() {
                    setPixmap(*(*currentAnim)[i]);
                });
            }
        }
    
}

void Vampire::playDeathAnimation() {
    QVector<QPixmap*>* currentAnim = nullptr;
    QString direction = facingDirection;

    if (direction == "down") {
        currentAnim = &deathFront;
    } else if (direction == "up") {
        currentAnim = &deathBack;
    } else if (direction == "left") {
        currentAnim = &deathLeft;
    } else if (direction == "right") {
        currentAnim = &deathRight;
    }

    
    if (currentAnim && !currentAnim->isEmpty()) {
        for (int i = 0; i < currentAnim->size(); ++i) {
            QTimer::singleShot(i * 100, this, [this, currentAnim, i]() {
                setPixmap(*(*currentAnim)[i]);
            });
        }
    }

}

void Vampire::wander(){
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

void Vampire::moveTowards(QPointF target) {
    if(!isDead){
        updateFacingDirection(); 
        playWalkAnimation();

        QPointF direction = target - pos();
        qreal dist = std::hypot(direction.x(), direction.y());

        if(dist <= 200){
            attack(targetPlayer);
            return;
        }

        // Sinon, on se rapproche du joueur
        direction /= dist; // normalisation
        if (canMoveInDirection(direction)) {
            setPos(x() + direction.x() * speed, y() + direction.y() * speed);
        }

    }
    

}

void Vampire::moveTowardsPlayerOrWander(Player* player) {
    Enemy::moveTowardsPlayerOrWander(player); // Appelle la méthode de la classe de base
}

void Vampire::takeDamage(int amount) {
    if (isDead) return;

    health -= amount;
    if (health <= 0 && !isDead) {
        isDead = true;
        playDeathAnimation();
        qDebug() << "L'ennemi est mort. Suppression de la scène";

        deathDuration = 11*100;  // 11 frames à 100ms pour l'animation de mort du vampire


        QTimer::singleShot(deathDuration, this, [this]() {
            if (scene()) scene()->removeItem(this);
                deleteLater();
        });

        if (targetPlayer) {
            targetPlayer->checkKillCount();
        }

    }
    
}

void Vampire::attack(Player* player){
    if (!scene() || !player || !canShoot) return;
    canShoot = false;

    if(!isDead){
        updateFacingDirection(); 
        // Joue l'animation de tir
        playAttackAnimation();

        // Calcul du délai basé sur le nombre de frames
        int frameCount = 12;     // ← nombre de frames dans l'animation shoot
        int frameDuration = 100; // ← durée d'une frame en ms (comme dans playAttackAnimation)
        int totalDuration = frameCount * frameDuration;

        QTimer::singleShot(totalDuration, this, [this, player]() {
            if (!scene() || !player) return;
            
            QRectF rect = boundingRect();
            QPointF start = pos() + QPointF(rect.width() / 2, rect.height() / 2);

        

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
}

Reaper::Reaper(QGraphicsItem* parent, Player* player) 
    : Enemy(nullptr,player, 100, 1, 7) {
         loadAnimations(); // Charger les animations
}

void Reaper::loadAnimations(){
    QPixmap walkSheet("img/Walk_reaper.png");
    QPixmap attackSheet("img/Attack_punch_reaper.png");
    
    int frameWidthWalk = walkSheet.width() / 9;
    int frameHeightWalk = walkSheet.height() / 4;

    for (int i = 0; i < 9; ++i) {
        walkBack.append(new QPixmap(walkSheet.copy(i * frameWidthWalk, 0 * frameHeightWalk, frameWidthWalk, frameHeightWalk))); 
        walkLeft.append(new QPixmap(walkSheet.copy(i * frameWidthWalk, 1 * frameHeightWalk, frameWidthWalk, frameHeightWalk))); 
        walkFront.append(new QPixmap(walkSheet.copy(i * frameWidthWalk, 2 * frameHeightWalk, frameWidthWalk, frameHeightWalk)));  
        walkRight.append(new QPixmap(walkSheet.copy(i * frameWidthWalk, 3 * frameHeightWalk, frameWidthWalk, frameHeightWalk)));  
    }

    int frameWidthAttack = attackSheet.width() / 6;
    int frameHeightAttack = attackSheet.height() / 4;

    for (int i = 0; i < 6; ++i) {
        attackBack.append(new QPixmap(attackSheet.copy(i * frameWidthAttack, 0 * frameHeightAttack, frameWidthAttack, frameHeightAttack))); 
        attackLeft.append(new QPixmap(attackSheet.copy(i * frameWidthAttack, 1 * frameHeightAttack, frameWidthAttack, frameHeightAttack))); 
        attackFront.append(new QPixmap(attackSheet.copy(i * frameWidthAttack, 2 * frameHeightAttack, frameWidthAttack, frameHeightAttack)));  
        attackRight.append(new QPixmap(attackSheet.copy(i * frameWidthAttack, 3 * frameHeightAttack, frameWidthAttack, frameHeightAttack)));  
    }

}

void Reaper::playWalkAnimation() {
    QVector<QPixmap*>* currentAnim = nullptr;
    QString direction = facingDirection;

    if(canPlayWalkAnimation){
        if (direction == "down") {
            currentAnim = &walkFront;
        } else if (direction == "up") {
            currentAnim = &walkBack;
        } else if (direction == "left") {
            currentAnim = &walkLeft;
        } else if (direction == "right") {
            currentAnim = &walkRight;
        }
    

        if (currentAnim && !currentAnim->isEmpty()) {
                currentFrame = (currentFrame + 1) % currentAnim->size();
                setPixmap(*(*currentAnim)[currentFrame]);
        }
    }
    
    
}

void Reaper::playAttackAnimation() {
    QVector<QPixmap*>* currentAnim = nullptr;
    QString direction = facingDirection;

    
        if (direction == "down") {
            currentAnim = &attackFront;
        } else if (direction == "up") {
            currentAnim = &attackBack;
        } else if (direction == "left") {
            currentAnim = &attackLeft;
        } else if (direction == "right") {
            currentAnim = &attackRight;
        }
    

    if (currentAnim && !currentAnim->isEmpty()) {
            for (int i = 0; i < currentAnim->size(); ++i) {
                QTimer::singleShot(i * 100, this, [this, currentAnim, i]() {
                    setPixmap(*(*currentAnim)[i]);
                });
            }
        }
    
}

void Reaper::playDeathAnimation() {
    // Pas d'animation de mort pour le Reaper, mais on peut en ajouter une si nécessaire
    qDebug() << "Reaper death animation not implemented yet.";
}


void Reaper::wander() {
    updateFacingDirection();
    playWalkAnimation(); // Joue l'animation de marche

    QList<QPointF> directions = {
        QPointF(1, 0), QPointF(-1, 0), QPointF(0, 1), QPointF(0, -1)
    };
    std::shuffle(directions.begin(), directions.end(), *QRandomGenerator::global());

    for (const QPointF& dir : directions) {
        if (canMoveInDirection(dir)) {
            setPos(x() + dir.x() * speed, y() + dir.y() * speed);
        }
    }

}


void Reaper::moveTowards(QPointF target) {

    if(!isDead){

        canPlayWalkAnimation = true; // Autorise l'animation de marche
        updateFacingDirection();
        playWalkAnimation();

        QPointF direction = target - pos();
        qreal dist = std::hypot(direction.x(), direction.y());

        // Stop walking animation if too close to attack
        if (dist <= 40) {   
            canPlayWalkAnimation = false; // Empêche de jouer l'animation de marche pendant l'attaque
            attack(targetPlayer);
            return;
        }

        // Déplacement
        direction /= dist;
        if (canMoveInDirection(direction)) {
            setPos(x() + direction.x() * speed, y() + direction.y() * speed);

        }
    }    
}

void Reaper::moveTowardsPlayerOrWander(Player* player) {
    Enemy::moveTowardsPlayerOrWander(player); // Appelle la méthode de la classe de base
}

void Reaper::takeDamage(int amount) {
    if (isDead) return;

    health -= amount;
    if (health <= 0 && !isDead) {
        isDead = true;
        playDeathAnimation();
        qDebug() << "L'ennemi est mort. Suppression de la scène";

      

        QTimer::singleShot(50, this, [this]() {
            if (scene()) scene()->removeItem(this);
                deleteLater();
        });

        if (targetPlayer) {
            targetPlayer->setLastKillPosition(pos()); // ← mémorise la position de l'ennemi avant de supprimer l’ennemi
            targetPlayer->checkKillCount();
        }

    }

}

void Reaper::attack(Player* player) {
    if (!canAttack) return;  // Empêche de frapper trop souvent
    canAttack = false;

    if(!isDead){

    

        updateFacingDirection(); 
        playAttackAnimation();  
        player->takeDamage(damage); 
        player->showHitEffect(); 
        qDebug() << "Le joueur a pris des dégats ! PV restants : " << player->getHealth();  

        attackTimer->start();  // Redémarre le timer pour autoriser la prochaine attaque dans 2 sec
    }
}




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

