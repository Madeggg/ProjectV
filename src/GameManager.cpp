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

    int frameCount = 0; // Compteur de frames
    int heroSpawnTimer = 0; // Compteur pour le spawn des héros

    frameCount++;
    heroSpawnTimer++;

    // Génère des soldats toutes les 5 secondes (300 frames à 60 FPS)
    if (frameCount % 300 == 0) {
        spawnEnemies();
    }

    // Génère un héros toutes les 30 secondes (1800 frames à 60 FPS)
    if (heroSpawnTimer >= 1800) {
        spawnHero();
        heroSpawnTimer = 0; // Réinitialise le compteur pour les héros
    }


    if (player->getHealth() <= 0) {
        gameLoopTimer->stop();
        // plus tard : afficher "Game Over" ou lancer un reset
    }

    // TODO : appel à spawnEnemies() et checkCollisions() plus tard
}

Player* GameManager::getPlayer() const {
    return player;
}


void GameManager::spawnEnemies() {
    Enemy* enemy = new Enemy(EnemyType::Soldier);
    scene->addItem(enemy);

    // Positionne l'ennemi en haut de la scène (y=0) à une position aléatoire sur l'axe x:
    enemy->setPos(QRandomGenerator::global()->bounded(0, 400), 0);

    //Le parent est défini comme this (l'objet GameManager), ce qui garantit que le QTimer sera correctement géré et détruit lorsque l'objet GameManager sera détruit.
    QTimer* shootTimer = new QTimer(this);  
    connect(shootTimer, &QTimer::timeout, enemy, &Enemy::shoot);
    shootTimer->start(3000); // L'ennemi tire toutes les 3 secondes
}


void GameManager::spawnHero(){
    Enemy* hero = new Enemy(EnemyType::Hero);
    scene->addItem(hero);

    // Positionne le héros en bas de la scène (y=600) à une position aléatoire sur l'axe x:
    hero->setPos(QRandomGenerator::global()->bounded(0, 400), 600);

    QTimer* shootTimer = new QTimer(this);
    connect(shootTimer, &QTimer::timeout, hero, &Enemy::shoot);
    shootTimer->start(1500);                    // Le héros tire plus rapidement (toutes les 1,5 secondes)
}