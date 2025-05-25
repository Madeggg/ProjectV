// Cette page, nous sert à gérer la vie, le score, le HUD, mais aussi le système de sauvegarde/chargement du meilleur score et pseudo
#include "GameManager.h"

GameManager::GameManager(MyScene* scene, Player* player) : scene(scene), player(player) {
    
    
        // // Connecte le signal de l'ennemi au slot du joueur pour infliger des dégâts
        //QObject::connect(enemy, &Enemy::damagePlayer, player, &Player::takeDamage);

    // Initialisation du timer pour la boucle de jeu
    gameLoopTimer = new QTimer(this);
    connect(gameLoopTimer, &QTimer::timeout, this, &GameManager::update);
}

void GameManager::startGame() {
    player->setHealth(100); // remet la vie à 100
    gameLoopTimer->start(16); // 60 FPS (1000 ms / 60 ≈ 16 ms)
}

void GameManager::resetGame() {
    for (QGraphicsItem* item : scene->items()) { // Supprime tous les items sauf le joueur 
        if (item != player)
            scene->removeItem(item);
    }

    player->setPos(2048, 2048);
    player->setHealth(10000);
    startGame(); // Redémarre la partie
}

void GameManager::update() {


    if (paused) return;

    frameCount++; // Incrémente le compteur de frames
    // qDebug() << "Le joueur est dans la scène ?" << (player->scene() != nullptr);
    // qDebug() << "Player position: " << player->pos();

    if (!player) {
        qDebug() << "Player is not initialized!";
        return;
    }
    
    if (player->getHealth() <= 0) {
        player->playDeathAnimation();
        gameLoopTimer->stop();
        emit gameOver(); // envoie le signal
        return;
    }

    //Gestions des collisions 
    QList<QGraphicsItem*> collisions = player->collidingItems();
    for (QGraphicsItem* item : collisions) {
        if (!item) continue;
        QVariant typeData = item->data(0); // Récupère le type de l'objet
        if (typeData.toString() == "collision") {
            player->revertToPreviousPosition();
            return; 
        }
    }


    // Zombie : toutes les 5 secondes
    if (frameCount % 300 == 0) {
        spawnZombies();
    }

    int killCount = player->getKillCount();

    // Reaper : tous les 3 kills
    if (killCount >= 3 && killCount % 3 == 0 && killCount != lastKillCountForReaper) {
        spawnReapers();
        lastKillCountForReaper = killCount;
    }

    // Vampire : tous les 10 kills
    if (killCount >= 10 && killCount % 10 == 0 && killCount != lastKillCountForVampire) {
        spawnVampires();
        lastKillCountForVampire = killCount;
    }

   
}

void GameManager::spawnZombies() {
    QPointF spawnPos = findValidSpawnPositionNearPlayer(200, 50);
    Enemy* zombie = new Zombie(nullptr, player);
    zombie->setScale(2.0);
    zombie->setPos(spawnPos);
    scene->addItem(zombie);
    qDebug() << "Zombie spawn à :" << spawnPos;
}


void GameManager::spawnReapers() {
    QPointF spawnPos = findValidSpawnPositionNearPlayer(230, 50);
    Enemy* reaper = new Reaper(nullptr, player);
    reaper->setScale(1.0);
    reaper->setPos(spawnPos);
    scene->addItem(reaper);
    qDebug() << "Reaper spawn à :" << spawnPos;
}

void GameManager::spawnVampires() {
    QPointF spawnPos = findValidSpawnPositionNearPlayer(250, 50);
    Enemy* vampire = new Vampire(nullptr, player);
    vampire->setScale(1.5);
    vampire->setPos(spawnPos);
    scene->addItem(vampire);
    qDebug() << "Vampire spawn à :" << spawnPos;
}



Player* GameManager::getPlayer() const {
    return player;
}


QPointF GameManager::findValidSpawnPositionNearPlayer(int offset, int maxAttempts) {
    const int step = offset;
    QPointF playerPos = player->pos();

    for (int i = 0; i < maxAttempts; ++i) {
        int dx = QRandomGenerator::global()->bounded(-1, 2); // -1, 0, ou 1
        int dy = QRandomGenerator::global()->bounded(-1, 2);

        // Ignore (0, 0) pour éviter le centre exact
        if (dx == 0 && dy == 0) continue;

        QPointF candidate = playerPos + QPointF(dx * step, dy * step);

        QRectF testRect(candidate, QSizeF(40, 40));
        QList<QGraphicsItem*> collisions = scene->items(testRect);

        bool blocked = false;
        for (QGraphicsItem* item : collisions) {
            if (item->data(0).toString() == "collision") {
                blocked = true;
                break;
            }
        }

        if (!blocked)
            return candidate;
    }

    // Si aucune position valide trouvée, retourne position joueur (sécurité)
    return playerPos + QPointF(step, step);
}


void GameManager::setPause(bool pause) {
    paused = pause;
    if (paused) {
        gameLoopTimer->stop(); // Arrêter la boucle de jeu
    } else {
        gameLoopTimer->start(16); // Redémarrer la boucle de jeu à 60 FPS
    }
}