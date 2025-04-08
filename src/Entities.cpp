// Cette page sert à tout ce qui est hostile (enemies, bombes etc..) + les projectiles tirés par joueur ou ennemis + les armes

#include "Entities.h"
#include <QGraphicsScene>
#include <QPixmap>
#include <QRandomGenerator>

Enemy::Enemy(EnemyType type, QGraphicsItem* parent) : QGraphicsPixmapItem(parent), type(type){
    setAppearance();
}

void Enemy::setAppearance(){
    QString imagePath;              //Déclaration du lien vers l'image comme un objet QString

    if (type == EnemyType::Soldier) {
        imagePath = " ";                                    // Sprite unique pour les soldats
    } else if (type == EnemyType::SuperHero) {
        QStringList heroSprites = {
            "homelander",
            "a_train",
            "black_noir",                   //Exemple d'une liste contenant les sprites de super héros
            "starlight",
            "the_deep"
        };

        int index = QRandomGenerator::global()->bounded(heroSprites.size());
        QString chosenHero = heroSprites[index];
        imagePath = ":/img/super_" + chosenHero + ".png";


    }

    QPixmap sprite(imagePath);
    if (!sprite.isNull()) {
        setPixmap(sprite.scaled(64, 64)); // pixelart, taille à adapter
    } else {
        qDebug("Image introuvable : %s", qPrintable(imagePath));
    }
}
}