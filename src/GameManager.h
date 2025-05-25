// Cette page, nous sert à gérer la vie, le score, le HUD, mais aussi le système de sauvegarde/chargement du meilleur score et pseudo
#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QObject>
#include <QTimer>
#include <QRandomGenerator>
#include "Player.h"
#include "MyScene.h"
#include "Entities.h"

// Déclaration anticipée
class MyScene;

class GameManager : public QObject {
    Q_OBJECT

public:
    GameManager(MyScene* scene, Player* player); // construit le gestionnaire en lien avec la scène
    void startGame(); // initialise et démarre la partie
    void resetGame(); // redémarre le jeu sans redémarrer le programme
    Player* getPlayer() const;
    void setPause(bool pause);
    bool getPause() const { return paused; };

public slots:
    void update(); // appelée régulièrement par QTimer pour faire tourner le jeu
private:
    bool paused = false;  
    MyScene* scene;
    Player* player;
    Enemy* enemy;
    QTimer* gameLoopTimer;
    QTimer* enemySpawnTimer; // Timer pour le spawn des ennemis
    int frameCount = 0; // Compteur de frames
    int lastKillCountForReaper = -1;
    int lastKillCountForVampire = -1;

    void checkCollisions(); // À développer plus tard
    void spawnZombies();
    void spawnVampires();
    void spawnReapers();
signals:
    void gameOver();
};

#endif 