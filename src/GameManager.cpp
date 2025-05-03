// Cette page, nous sert à gérer la vie, le score, le HUD, mais aussi le système de sauvegarde/chargement du meilleur score et pseudo
#include "GameManager.h"

GameManager::GameManager(MyScene* scene, Player* player) : scene(scene), player(player) {
    // Initialisation des autres objets, comme les ennemis
    Enemy* enemy = new Enemy(nullptr, player);
    
    // Connecte le signal de l'ennemi au slot du joueur pour infliger des dégâts
    QObject::connect(enemy, &Enemy::damagePlayer, player, &Player::takeDamage);

    // Initialisation du timer pour la boucle de jeu
    gameLoopTimer = new QTimer(this);
    connect(gameLoopTimer, &QTimer::timeout, this, &GameManager::update);
}

void GameManager::startGame() {
    player->setHealth(100); // remet la vie à 100
    gameLoopTimer->start(16); // 60 FPS (1000 ms / 60 ≈ 16 ms)
}

void GameManager::resetGame() {
    for (QGraphicsItem* item : scene->items()) { // Supprime tous les items sauf le joueur (à améliorer plus tard)
        if (item != player)
            scene->removeItem(item);
    }

    player->setPos(200, 400);
    player->setHealth(100);
    startGame(); // Redémarre la partie
}

void GameManager::update() {
    frameCount++; // Incrémente le compteur de frames
    qDebug() << "Le joueur est dans la scène ?" << (player->scene() != nullptr);
    qDebug() << "Player position: " << player->pos();

    if (!player) {
        qDebug() << "Player is not initialized!";
        return;
    }
    
    // if (player->getHealth() <= 0) {
    //     gameLoopTimer->stop();
    //     qDebug() << "Game Over";
    //     return;
    // }

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
}


Player* GameManager::getPlayer() const {
    return player;
}


