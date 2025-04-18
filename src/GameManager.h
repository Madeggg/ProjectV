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
    GameManager(MyScene* scene); // construit le gestionnaire en lien avec la scène
    void startGame(); // initialise et démarre la partie
    void resetGame(); // redémarre le jeu sans redémarrer le programme
    Player* getPlayer() const;

public slots:
    void update(); // appelée régulièrement par QTimer pour faire tourner le jeu
    void onPlayerMoved(QPointF newPos); // appelée lorsque le joueur se déplace
private:
    MyScene* scene;
    Player* player;
    QTimer* gameLoopTimer;

    void spawnEnemies(); // À développer plus tard
    void spawnHero();  // À développer plus tard
    void checkCollisions(); // À développer plus tard
};

#endif 