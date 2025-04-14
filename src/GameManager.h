    // Cette page, nous sert à gérer la vie, le score, le HUD, mais aussi le système de sauvegarde/chargement du meilleur score et pseudo
#   ifndef GameManager_h
#define GameManager_h

#include <QObject>
#include <QTimer>
#include "Player.h"
#include "MyScene.h"
#include "Entities.h"

class GameManager : public QObject {
    Q_OBJECT

public:
    GameManager(MyScene* scene); // construit le gestionnaire en lien avec la scène
    void startGame(); // initialise et démarre la partie
    void resetGame(); // redémarre le jeu sans redémarrer le programme
    Player* getPlayer() const;

private slots:
    void update(); // appelée régulièrement par QTimer pour faire tourner le jeu

private:
    MyScene* scene;
    Player* player;
    QTimer* gameLoopTimer;

    void spawnEnemies(); // À développer plus tard
    void spawnHero();  // À développer plus tard
    void checkCollisions(); // À développer plus tard
};

#endif 