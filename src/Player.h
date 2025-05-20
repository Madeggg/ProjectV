    // Cette page est la afin de gérer le Personnage principal, et les controles par le joueur
    #ifndef PLAYER_H
    #define PLAYER_H

    #include <QGraphicsPixmapItem>
    #include <QObject>
    #include <QKeyEvent>
    #include <QTimer>
    #include <QVector>
    #include <QPixmap>
    #include "GameManager.h"
   

    // Déclaration anticipée
    class MyScene;
    class Weapon;



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
        Weapon* getCurrentWeapon() const;
        
        enum Slot {Melee = 0, Pistol = 1, Shotgun = 2};     // Enumération pour les types d'armes
        int getAmmo(Slot s) const; //Pour avoir les munitions de l'arme actuelle (au slot s)

       
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override; // Méthode pour afficher la hitbox du joueur
    QRectF boundingRect() const override; // Méthode pour changer la hitbox du player

        
        
        
        void setHealth(int newHealth);
        void setDirection(const QString& dir);
        void takeDamage(int amount);
        bool canMoveTo(const QPointF& newPos, const QRectF& sceneRect) const;
        bool selectSlot(Slot s);
        Slot pickWeapon(Weapon* w); // Méthode pour placer une arme dans l'inventaire, renvoie le slot occupé par cette arme
        void switchTo(Slot s);      // Méthode pour changer d'arme
        void setHasWeapon(bool newHasWeapon) ;
        void punch();                           // Méthode pour frapper un ennemi
        void shoot(QPointF targetPos);           // Méthode pour tirer un projectile 
        void loadAnimations();
        void playAttackAnimation();     // Méthode pour jouer l'animation d'attaque
        void playDeathAnimation();      // Méthode pour jouer l'animation de mort
        
    
        void revertToPreviousPosition();
       
        void checkKillCount();          // Incrémente le compteur de kills et fait spawn une arme 
        QPainterPath shape() const override;



    private:
        int health;
        int speed = 5;
        Weapon* weapon;
        QPointF previousPosition; // Position précédente pour la restauration
        QString direction;      // Direction actuelle du joueur (gauche, droite, haut, bas)
        int currentFrame = 0;
        int killCount = 0;
        bool hasWeapon;
        Weapon* inventory[3] = {nullptr,nullptr,nullptr};   // Tableau d'armes pour l'inventaire
        Slot currentSlot = Melee;   //Par défaut, le joueur utilise l'arme de mêlée

        //Déplacements classiques
        QVector<QPixmap*> walkFront;
        QVector<QPixmap*> walkBack;
        QVector<QPixmap*> walkRight;
        QVector<QPixmap*> walkLeft;

        //Quand le joueur attaque sans arme à distance
        QVector<QPixmap*> meleeFront;
        QVector<QPixmap*> meleeBack;
        QVector<QPixmap*> meleeRight;
        QVector<QPixmap*> meleeLeft;

        //Quand le joueur attaque avec une arme à distance
        QVector<QPixmap*> pistolFront;
        QVector<QPixmap*> pistolBack;
        QVector<QPixmap*> pistolRight;
        QVector<QPixmap*> pistolLeft;

        // Quand le joueur meurt
        QVector<QPixmap*> deathFront;
        QVector<QPixmap*> deathBack;
        QVector<QPixmap*> deathRight;
        QVector<QPixmap*> deathLeft;

        //Quand le joueur ne bouge pas
        QVector<QPixmap*> idleFront;
        QVector<QPixmap*> idleBack;
        QVector<QPixmap*> idleRight;
        QVector<QPixmap*> idleLeft;


        QTimer* walkTimer;

    signals:
        void playerMoved(QPointF newPos); // Signal émis lorsque le joueur se déplace
        void ammoBoxNeeded(); //Signal envoyé tous les 5 kills
        void healthChanged(int newHealth);
        void scoreChanged(int newScore); //     Signal émis lorsque le score change
        void weaponChanged(Weapon* newWeapon); // Signal émis lorsque l'arme change


    public slots:
         void updateWalkAnimation();            // Méthode pour mettre à jour l'animation de marche

    };

    #endif 