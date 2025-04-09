// Cette page, nous sert à gérer la vie, le score, le HUD, mais aussi le système de sauvegarde/chargement du meilleur score et pseudo
#include "GameManager.h"

GameManager::GameManager(MyScene* scene) : scene(scene) {
    player = new Player(); //création du personnage et ajoue à la scène
    scene->addItem(player);
    player->setPos(200, 400); // position initiale au centre approximatif

    
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

    if (player->getHealth() <= 0) {
        gameLoopTimer->stop();
        // plus tard : afficher "Game Over" ou lancer un reset
    }

    // TODO : appel à spawnEnemies() et checkCollisions() plus tard
}

Player* GameManager::getPlayer() const {
    return player;
}