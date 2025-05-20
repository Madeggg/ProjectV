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
    for (QGraphicsItem* item : scene->items()) { // Supprime tous les items sauf le joueur (à améliorer plus tard)
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

    if (frameCount % 300 == 0) { // toutes les 5 secondes à 60 FPS
        spawnEnemiesPhysique();
        
        if(player->getKillCount() > 5) { // Si le joueur a tué 5 ennemis
            spawnEnemiesDistance();
        }
    }

   
}

void GameManager::spawnEnemiesPhysique() {
    static QVector<QPointF> safeSpawnPoints = {
        {625,50}, {970,295}, {15,575}, {15,265},        
        {70,390}, {445,970}, {10,800}, {970,570},       //liste de points de spawns hors collisions pour les ennemis
        {685,590}, {965,790}, {970,600}, {175,870}  
    };

    int index = QRandomGenerator::global()->bounded(safeSpawnPoints.size());
    QPointF spawnPos = safeSpawnPoints[index];

    Enemy* enemy = new Enemy("Physique", nullptr, player);
    enemy->setPos(spawnPos);
    scene->addItem(enemy);
}

void GameManager::spawnEnemiesDistance() {
    static QVector<QPointF> safeSpawnPoints = {
        {625,50}, {970,295}, {15,575}, {15,265},        
        {70,390}, {445,970}, {10,800}, {970,570},       //liste de points de spawns hors collisions pour les ennemis
        {685,590}, {965,790}, {970,600}, {175,870}  
    };

    int index = QRandomGenerator::global()->bounded(safeSpawnPoints.size());
    QPointF spawnPos = safeSpawnPoints[index];

    Enemy* enemy = new Enemy("Distance", nullptr, player);
    enemy->setHealth(200);
    enemy->setSpeed(10);
    enemy->setPos(spawnPos);
    scene->addItem(enemy);
}




Player* GameManager::getPlayer() const {
    return player;
}




void GameManager::setPause(bool pause) {
    paused = pause;

    if (paused) {
        gameLoopTimer->stop(); // Arrêter la boucle de jeu
    } else {
        gameLoopTimer->start(16); // Redémarrer la boucle de jeu à 60 FPS
    }
}