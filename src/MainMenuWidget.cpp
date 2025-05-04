#include "MainMenuWidget.h"

//ligne à ajouter dans le .pro : QT += multimedia

MainMenuWidget::MainMenuWidget(QWidget* parent) : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);


     // Créer un objet QSoundEffect pour la musique
     QSoundEffect* musicEffect = new QSoundEffect(this);

     // Charger le fichier audio (assure-toi que le fichier existe)
     musicEffect->setSource(QUrl::fromLocalFile("sound/projectV.wav"));
 
     // Lancer la musique
     musicEffect->setLoopCount(QSoundEffect::Infinite);  // Jouer la musique en boucle infinie
     musicEffect->setVolume(0.2f);  // Ajuste le volume entre 0 et 1
     musicEffect->play();

    // Titre du jeu
    QLabel* title = new QLabel("Project V", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 30px; font-weight: bold; color: #333333;");
    layout->addWidget(title);
    
    // Bouton Nouvelle Partie
    QPushButton* newGameButton = new QPushButton("Nouvelle Partie", this);
    newGameButton->setStyleSheet("background-color: #4CAF50; color: white; border-radius: 10px; padding: 10px; font-size: 18px;");
    layout->addWidget(newGameButton);
    
    // Bouton Quitter
    QPushButton* quitButton = new QPushButton("Quitter", this);
    quitButton->setStyleSheet("background-color: #f44336; color: white; border-radius: 10px; padding: 10px; font-size: 18px;");
    layout->addWidget(quitButton);

    // Connecter les signaux
    connect(newGameButton, &QPushButton::clicked, this, &MainMenuWidget::startGameRequested);
    connect(quitButton, &QPushButton::clicked, qApp, &QApplication::quit);
    
    this->setStyleSheet("background-color: #f0f0f0;"); // Fond de couleur claire
}
