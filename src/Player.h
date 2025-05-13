    // Cette page est la afin de gérer le Personnage principal, et les controles par le joueur
    #ifndef PLAYER_H
    #define PLAYER_H

    #include <QGraphicsPixmapItem>
    #include <QObject>
    #include <QKeyEvent>
    #include <QTimer>

    #include "GameManager.h"
    #include "Entities.h"

    // Déclaration anticipée
    class MyScene;



    class Player : public QObject, public QGraphicsPixmapItem { //QGraphicsPixmapItem, sous-classe de QGraphicsItem, affiche une image (pixmap) dans une scène graphique
        Q_OBJECT

    public:
        Player(QGraphicsItem* parent = nullptr); //constructeur de la classe, cet objet devient le parent de notre Player

        //Méthodes de déplacement 
        void moveLeft();
        void moveRight();
        void moveUp();
        void moveDown();
        // Méthodes de gestion de PV
        int getHealth() const;
        qreal getSpeed() const;
        QString getDirection() const;
        int getKillCount() const;
        bool getHasWeapon() const;
        
        QTimer* walkTimer;
        
        void setHealth(int newHealth);
        void setDirection(const QString& dir);
        void takeDamage(int amount);
        bool canMoveTo(const QPointF& newPos, const QRectF& sceneRect) const;
        void setHasWeapon(bool newHasWeapon) ;
        void punch();                           // Méthode pour frapper un ennemi
        void shoot(QPointF targetPos);           // Méthode pour tirer un projectile 
        

    
        void revertToPreviousPosition();
        void updateWalkAnimation();            // Méthode pour mettre à jour l'animation de marche
        void incrementKillCount();          // Incrémente le compteur de kills et fait spawn une arme 
        QPainterPath shape() const override;

    private:
        int health;
        int speed = 5;
        QPointF previousPosition; // Position précédente pour la restauration
        QString direction;      // Direction actuelle du joueur (gauche, droite, haut, bas)
        int walkframe = 0;
        int killCount = 0;
        bool hasWeapon;

    signals:
        void playerMoved(QPointF newPos); // Signal émis lorsque le joueur se déplace

    };

    #endif 