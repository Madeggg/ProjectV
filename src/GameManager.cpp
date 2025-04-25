// Cette page, nous sert à gérer la vie, le score, le HUD, mais aussi le système de sauvegarde/chargement du meilleur score et pseudo
#include "GameManager.h"

GameManager::GameManager(MyScene* scene) : scene(scene) {
    // Exemple de création d'un Enemy dans GameManager ou ailleurs
    Player* player = new Player();
    Enemy* enemy = new Enemy(nullptr, player);

    // Connecte le signal de l'ennemi au slot du joueur
    QObject::connect(enemy, &Enemy::damagePlayer, player, &Player::takeDamage);


    gameLoopTimer = new QTimer(this); // création d'un timer pour gérer la boucle de jeu
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
    // chose à rajouter quand entités sera fini :
    // - le déplacement automatique des ennemis
    // - les tirs
    // - les collisions
    // - la vérification de la mort du joueur


    frameCount++; // Incrémente le compteur de frames

    if (player->getHealth() <= 0) {
        gameLoopTimer->stop();
        // plus tard : afficher "Game Over" ou lancer un reset
    }

    // TODO : appel à spawnEnemies() et checkCollisions() plus tard
}

Player* GameManager::getPlayer() const {
    return player;
}

//SpawnHeros et enemy à refaire

