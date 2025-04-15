#include "MyScene.h"

MyScene::MyScene(QObject* parent) : QGraphicsScene(parent) {
    //Image de fond
    pixBackground = QPixmap(":/img/background.png"); 

    //Définir la taille de la scène en fonction de la taille de l'image de fond
    setSceneRect(0,0,pixBackground.width(),pixBackground.height()); 

    //Initialisation du timer
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MyScene::update);
    timer->start(30);  //Maj toutes les 30ms

    

    //Exemple avec l'ajout d'un rectangle
    qgri = new QGraphicsRectItem(10, 100, 300, 200);
    qgri->setBrush(Qt::blue); // Couleur de remplissage
    this->addItem(qgri);

    //Ajout texte
    qgti = new QGraphicsTextItem("Hello World !");
    this->addItem(qgti);
    qgti->setPos(50, 50);   // Position du texte

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
    QPointF pos = qgti->pos();
    qgti->setPos(pos.x() + 1, pos.y());

    //Collisions
    if(qgri->collidesWithItem(player)) {
        qDebug() << "Collision detected!";
        player->setPos(200, 400); // Reset player position
    }

    gameManager->update(); // Appel à la méthode update du GameManager pour gérer les ennemis

    

}

void myScene::keyPressEvent(QKeyEvent* event){
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
    if(event->key() == Qt::Key_Q){
        player->setPos(player->x() - 10, player->y()); 
    }

    //Déplacement vers la droite
    else if(event->key() == Qt::Key_D){
        player->setPos(player->x() + 10, player->y()); 
    }

    //Déplacement vers le haut
    else if(event->key() == Qt::Key_Z){
        player->setPos(player->x(), player->y() - 10); 
    }

    //Déplacement vers le bas
    else if(event->key() == Qt::Key_S){
        player->setPos(player->x(), player->y() + 10); 
    }
}

void MyScene::mousePressEvent(QMouseEvent* event){
    if(event->button == Qt::LeftButton){
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
        Projectile* = new Projectile(projStartPos, direction, speed); 

        //Ajout du projectile à la scène
        this->addItem(projectile);
    }   
}

void MyScene::drawBackground(QPainter* painter, const QRectF& rect) {
    Q_UNUSED(rect); 
    // Dessine l'image de fond
    painter->drawPixmap(QPointF(0, 0),pixBackground,sceneRect());
}