#include "MyScene.h"

MyScene::MyScene(QObject* parent) : QGraphicsScene(parent) {
    //Image de fond
    pixBackground = QPixmap("img/gameBackground.jpg"); 

    //Définir la taille de la scène en fonction de la taille de l'image de fond
    setSceneRect(0,0,pixBackground.width(),pixBackground.height()); 

    //Initialisation du timer
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MyScene::update);
    timer->start(30);  //Maj toutes les 30ms

    //Ajout du player
    player = new Player();
    this->addItem(player);

    

    //Initialisation du gameManager
    gameManager = new GameManager(this);

}

MyScene::~MyScene() {
    delete timer;
    delete gameManager;
}

//you can add your own methods here

void MyScene::update(){
    //Déplacement de l'objet texte par exemple
   gameManager->update(); // Appel à la méthode update du GameManager pour gérer les ennemis

    

}

Player* MyScene::getPlayer() const {
    return player;
}

void MyScene::keyPressEvent(QKeyEvent* event){
    QRectF sceneBounds = sceneRect(); // Récupère les limites de la scène
    //Pour mettre le jeu en pause en appuyant P par exemple
    if(event->key() == Qt::Key_P){
        if(timer->isActive()){
            timer->stop();
            qDebug() << "Game paused";
        } else {
            timer->start();             // Reprend le timer
            qDebug() << "Game resumed";
        }
    }

    // Déplacement vers la gauche
    if (event->key() == Qt::Key_Q) {
        QPointF newPosition = player->pos() + QPointF(-player->getSpeed(), 0);
        if (player->canMoveTo(newPosition, sceneBounds)) {
            player->moveLeft();
            player->setPixmap(QPixmap("img/Sprite_billy_left.png").scaled(40, 40));
        }
    }

    // Déplacement vers la droite
    else if (event->key() == Qt::Key_D) {
        QPointF newPosition = player->pos() + QPointF(player->getSpeed(), 0);
        if (player->canMoveTo(newPosition, sceneBounds)) {
            player->moveRight();
            player->setPixmap(QPixmap("img/Sprite_billy_right.png").scaled(40, 40));

        }
    }

    // Déplacement vers le haut
    else if (event->key() == Qt::Key_Z) {
        QPointF newPosition = player->pos() + QPointF(0, -player->getSpeed());
        if (player->canMoveTo(newPosition, sceneBounds)) {
            player->moveUp();
            player->setPixmap(QPixmap("img/Sprite_billy_up.png").scaled(40, 40));

        }
    }

    // Déplacement vers le bas
    else if (event->key() == Qt::Key_S) {
        QPointF newPosition = player->pos() + QPointF(0, player->getSpeed());
        if (player->canMoveTo(newPosition, sceneBounds)) {
            player->moveDown();
            player->setPixmap(QPixmap("img/Sprite_billy_down.png").scaled(40, 40));
        }
    }
}

void MyScene::mousePressEvent(QMouseEvent* event){
    if(event->button() == Qt::LeftButton){
        //Tirer un projectile
        QPointF projStartPos = player->pos();   // Position de départ du projectile sur le joueur

        QPointF projTargetPos = event->pos(); // Position au clic du joueur

        //Calcul de la direction
        QPointF direction = projTargetPos - projStartPos;
        qreal length = std::sqrt(direction.x() * direction.x() + direction.y() * direction.y());    //Calcul de la norme du vecteur avec la formule connue
        
        //On normalise le vecteur en le divisant par sa longueur, afin que sa norme vale 1 
        if(length != 0) {
            direction /= length; 
        }

        int speed = 10;     // Vitesse du projectile

        // Création du projectile
        Projectile* projectile = new Projectile(projStartPos, direction, speed); 

        //Ajout du projectile à la scène
        this->addItem(projectile);
    }   
}

void MyScene::drawBackground(QPainter* painter, const QRectF& rect) {
    Q_UNUSED(rect);
    if (pixBackground.isNull()) {
        qDebug() << "Erreur : pixBackground est nul dans drawBackground !";
        return;
    }
    // Redimensionne l'image de fond pour qu'elle corresponde à la taille de la scène
    painter->drawPixmap(sceneRect(), pixBackground, pixBackground.rect());
}