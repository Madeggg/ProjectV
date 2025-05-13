#include "MyScene.h"

#include <QTimer>
#include <QKeyEvent>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QGraphicsRectItem>
#include <QDebug>

MyScene::MyScene(QObject* parent) : QGraphicsScene(parent) {
    loadMap(); 

     // Définir les limites de la scène en fonction des dimensions de la map
     setSceneRect(0, 0, backgroundWidth, backgroundHeight);

    // Initialisation du timer
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MyScene::update);
    timer->start(30);  // Maj toutes les 30ms

    // Création du joueur
    player = new Player();
    this->addItem(player);  // Ajout à la scène
    player->setPos(445, 555);
    player->setZValue(50);

    //Setup HUD
    // SCORE
    scoreText = new QGraphicsTextItem("Score : 0");
    scoreText->setDefaultTextColor(Qt::white);
    scoreText->setFont(QFont("Arial", 14));
    scoreText->setPos(10, 10);
    addItem(scoreText);

    // TIMER
    timerText = new QGraphicsTextItem("Temps : 0 s");
    timerText->setDefaultTextColor(Qt::white);
    timerText->setFont(QFont("Arial", 14));
    timerText->setPos(10, 30);
    addItem(timerText);

    // BARRE DE VIE
    healthBarBack = new QGraphicsRectItem(10, 50, 100, 10);
    healthBarBack->setBrush(Qt::red);
    addItem(healthBarBack);

    healthBarFront = new QGraphicsRectItem(10, 50, 100, 10);
    healthBarFront->setBrush(Qt::green);
    addItem(healthBarFront);

    // TIMER DE MISE À JOUR
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &MyScene::updateHUD);
    gameTimer->start(1000);  // chaque seconde

    scoreText->setZValue(1000);
    timerText->setZValue(1000);
    healthBarFront->setZValue(1001);


    

    // Création de GameManager en passant le joueur existant
    gameManager = new GameManager(this, player);
}


MyScene::~MyScene() {
    delete timer;
    delete gameManager;
}



void MyScene::update() {
    gameManager->update(); // Appel à la méthode update du GameManager pour gérer les ennemis

    // Récupérer tous les projectiles dans la scène
    QList<QGraphicsItem*> itemsInScene = items();
    for (QGraphicsItem* item : itemsInScene) {
        Projectile* projectile = dynamic_cast<Projectile*>(item);
        if (projectile) {
            // 🔒 Sécurité : projectile déjà supprimé de la scène ?
            if (projectile->scene() == nullptr) continue;

            // Vérifie les collisions pour ce projectile
            QList<QGraphicsItem*> collisions = projectile->collidingItems();
            for (QGraphicsItem* collidingItem : collisions) {
                if (!collidingItem) continue;

                if (collidingItem->data(0).toString() == "enemy") {
                    Enemy* enemy = dynamic_cast<Enemy*>(collidingItem);
                    if (enemy && !enemy->getIsDead() && enemy->scene() != nullptr) {
                        enemy->takeDamage(projectile->getDamage());
                        enemy->setPixmap(QPixmap("img/sprite_ennemi_static_hit.png").scaled(40, 40));
                        qDebug() << "Projectile hit enemy! Enemy health:" << enemy->getHealth();

                        enemy->showHitEffect(); // Affiche l'effet de coup

                        removeItem(projectile);
                        projectile->deleteLater();
                        return;
                    }
                }
            }
        }
        Weapon* weapon = dynamic_cast<Weapon*>(item);
        if (weapon && weapon->scene() != nullptr) {
            if (weapon->collidesWithItem(player)) {
                qDebug() << "Player picked up a weapon!";
                player->setHasWeapon(true);
                removeItem(weapon);
                weapon->deleteLater();
                continue; // On passe à l'objet suivant
            }
        }
    }
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
        QPointF newPosition = player->pos() + QPointF(player->getSpeed(), 0);
        if (player->canMoveTo(newPosition, sceneBounds)) {
            player->moveLeft();
            player->setDirection("left"); 
            player->setPixmap(QPixmap("img/Sprite_billy_left.png").scaled(40, 40));
        }
    }

    // Déplacement vers la droite
    // else if (event->key() == Qt::Key_D) {
    //     QPointF newPosition = player->pos() + QPointF(player->getSpeed(), 0);
    //     if (player->canMoveTo(newPosition, sceneBounds)) {
    //         player->moveRight();
    //         player->setDirection("right"); // Met à jour la direction
    
    //         if (!player->walkTimer->isActive())
    //             player->walkTimer->start();
    
    //         player->updateWalkAnimation();  // lance la première image
    //     }
    // }
    
    else if (event->key() == Qt::Key_D) {
        QPointF newPosition = player->pos() + QPointF(-player->getSpeed(), 0);
        if (player->canMoveTo(newPosition, sceneBounds)) {
            player->moveRight();
            player->setDirection("right"); 
            player->setPixmap(QPixmap("img/Sprite_billy_right.png").scaled(40, 40));
        }
    }

    // Déplacement vers le haut
    else if (event->key() == Qt::Key_Z) {
        QPointF newPosition = player->pos() + QPointF(0, -player->getSpeed());
        if (player->canMoveTo(newPosition, sceneBounds)) {
            player->moveUp();
            player->setDirection("up"); 
            player->setPixmap(QPixmap("img/Sprite_billy_up.png").scaled(40, 40));

        }
    }

    // Déplacement vers le bas
    else if (event->key() == Qt::Key_S) {
        QPointF newPosition = player->pos() + QPointF(0, player->getSpeed());
        if (player->canMoveTo(newPosition, sceneBounds)) {
            player->moveDown();
            player->setDirection("down"); 
            player->setPixmap(QPixmap("img/Sprite_billy_down.png").scaled(40, 40));
        }
    }
}

void MyScene::keyReleaseEvent(QKeyEvent* event) {
    player->walkTimer->stop();
}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (!player) {
        qDebug() << "Error: Player is null!";
        return;
    }

    if(event->button() == Qt::LeftButton){
        if (player->getHasWeapon()) {
            player->shoot(event->scenePos());
        }
    //Sinon, le joueur n'a pas d'arme donc il va taper à la mano
        else{
            player->punch();
        }
    
    // Transmet l'événement à la classe parente
    QGraphicsScene::mousePressEvent(event);
    }
    
}




void MyScene::loadMap(){

    //Load and parse json file
    QFile file("test1.json");
    file.open(QIODevice::ReadOnly);

    QJsonDocument document = QJsonDocument::fromJson(file.readAll());
    QJsonObject mapObject = document.object();

 

    //listPixmap will contains every tiles
    QMap<int, QPixmap> listPixmap;

    int tileWidth = mapObject["tilewidth"].toInt();
    int tileHeight = mapObject["tileheight"].toInt();
    int numberTileWidth = mapObject["width"].toInt();
    int numberTileHeight = mapObject["height"].toInt();



    this->backgroundWidth = numberTileWidth*tileWidth;
    this->backgroundHeight = numberTileHeight*tileHeight;


    QPixmap background (this->backgroundWidth, this->backgroundHeight);
    QPainter painter(&background);

    //First we get every tiles to add it into listPixmap
    QJsonArray tilesets = mapObject["tilesets"].toArray();
    for(QJsonValue tilesetValue : tilesets){
        QJsonObject tileset = tilesetValue.toObject();

        int firstGid = tileset["firstgid"].toInt();
        QString source = tileset["image"].toString();
       
        QPixmap tilesetImage(source); //load image
        if(tilesetImage.isNull()){
            qWarning() << "Failed to load tileset image:" << source;
            continue;
        }
        int numColumns = tilesetImage.width() / tileWidth;
        int numRows = tilesetImage.height() / tileHeight;
        for (int row = 0; row < numRows; ++row) {
            for (int column = 0; column < numColumns; ++column) {
                int tileID = firstGid + (row * numColumns) + column;

                //We add one by one tiles so we 'cut' the image everytime
                QRect tileRect(column * tileWidth, row * tileHeight, tileWidth, tileHeight);
                QPixmap tilePixmap = tilesetImage.copy(tileRect);
                listPixmap[tileID] = tilePixmap;
            }
        }
    }

    QJsonArray layers = mapObject["layers"].toArray();
    for(QJsonValue layerValue : layers){
        QJsonObject layer = layerValue.toObject();

        if(layer["type"] == "tilelayer"){
            int width = layer["width"].toInt();
            int height = layer["height"].toInt();

            QJsonArray data = layer["data"].toArray();
            for(int y = 0; y < height; y++){ //line
                for(int x = 0; x < width; x++){ //column
                    int tileID = data[width * y + x].toInt();
                    if(tileID != 0){
                        if (!listPixmap.contains(tileID)) {
                            qWarning() << "TileID" << tileID << "not found in tileset map!";
                        }
                        
                        // QGraphicsPixmapItem* tile = new QGraphicsPixmapItem(listPixmap[tileID]);
                        // tile->setPos(x * 16, y * 16);
                        // tile->setOpacity(layer["opacity"].toDouble());
                        // this->addItem(tile);//draw the tile at the right position
                        painter.drawPixmap(x * 16, y * 16, listPixmap[tileID]);

                    }
                }
            }

            //Add collisions objects
        } else if(layer["type"] == "objectgroup" && layer["name"] == "collisions"){
            QJsonArray objects = layer["objects"].toArray();
            qDebug() << "Collisions layer found, processing objects...";

            for(QJsonValue objectValue : objects){
                QJsonObject object = objectValue.toObject();

                qDebug() << "Raw object data:" << object;
            
                double x = object["x"].toDouble();
                double y = object["y"].toDouble();
                double width = object["width"].toDouble();
                double height = object["height"].toDouble();

                qDebug() << "Raw coordinates - x:" << x << "y:" << y << "width:" << width << "height:" << height;

                

                if (x == 0 && y == 0) {
                    qDebug() << "Warning: Collision object has (x=0, y=0)";
                }

                

                // qDebug() << "Processing object: x=" << x << "y=" << y << "width=" << width << "height=" << height;

                else if (object.contains("polygon")) {
                    QJsonArray polygonArray = object["polygon"].toArray();
                    QPolygonF polygon;
                
                    // On récupère les coordonnées du polygon dans le tableau
                    for (const QJsonValue& pointVal : polygonArray) {
                        QJsonObject pointObj = pointVal.toObject();
                        qreal x = pointObj["x"].toDouble();
                        qreal y = pointObj["y"].toDouble();
                        polygon << QPointF(x, y);
                    }
                
                    // Déplacement du polygon en fonction des coordonnées x et y de l'objet
                    polygon.translate(object["x"].toDouble(), object["y"].toDouble());
                
                    // Création du polygon et ajout à la scène
                    QGraphicsPolygonItem* polygonItem = new QGraphicsPolygonItem(polygon);
                    polygonItem->setBrush(Qt::red); // Pour visualiser le polygon
                    polygonItem->setPen(Qt::NoPen); // Pas de bordure
                    polygonItem->setData(0, "collision"); // Définir comme objet de collision
                    polygonItem->setZValue(100); // Derrière les autres objets
                    this->addItem(polygonItem); // Ajout à la scène

                } else {
                    // qDebug() << "Creating rect collision at x=" << x << "y=" << y;
                    QGraphicsRectItem* rect = new QGraphicsRectItem(x, y, width, height);
                    rect->setBrush(Qt::red);
                    rect->setPen(QPen(Qt::transparent));  // ou laisse un fin contour
                    rect->setData(0, "collision");
                    rect->setZValue(100);
                    this->addItem(rect);
                }
            }
        }
    }
    painter.end(); // Fin de la peinture sur le QPixmap
    QGraphicsPixmapItem* backgroundItem = new QGraphicsPixmapItem(background); // Crée un item pour le fond
    this->addItem(backgroundItem); // Ajoute le fond à la scène
    file.close();
}

void MyScene::updateHUD() {
    elapsedTime++;
    timerText->setPlainText(QString("Temps : %1 s").arg(elapsedTime));
    scoreText->setPlainText(QString("Score : %1").arg(score));

    // Récupère la position du coin haut-gauche de la vue
    QPointF viewTopLeft = views().first()->mapToScene(0, 0);

    // Espacements
    const int margin = 20;
    const int lineSpacing = 25;

    // Positionnement du texte
    scoreText->setPos(viewTopLeft.x() + margin, viewTopLeft.y() + margin);
    timerText->setPos(viewTopLeft.x() + margin, viewTopLeft.y() + margin + lineSpacing);

    // Mise à jour de la barre de vie
    if (player) {
        qreal healthPercent = static_cast<qreal>(player->getHealth()) / 100.0;
        healthBarFront->setRect(0, 0, 100 * healthPercent, 10);
        healthBarFront->setPos(viewTopLeft.x() + margin, viewTopLeft.y() + margin + 2 * lineSpacing);
    }
}



void MyScene::addScore(int points) {
    score += points;
    if (scoreText) {
        scoreText->setPlainText(QString("Score : %1").arg(score));
    }
}