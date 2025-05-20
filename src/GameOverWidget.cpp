#include "GameOverWidget.h"

GameOverWidget::GameOverWidget(QWidget* parent) : QWidget(parent) {
    setStyleSheet("background-color: rgba(0, 0, 0, 180); color: white; font-size: 24px;");
    setFixedSize(800, 600); // Mets ici la taille de ton écran de jeu

    label = new QLabel("Game Over", this);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("background-color: #222; color: white; font-size: 64px; font-weight: bold;");
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    restartButton = new QPushButton("Restart", this);
    connect(restartButton, &QPushButton::clicked, this, &GameOverWidget::restartClicked);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(label, 1); // Le label prend tout l'espace disponible
    layout->addWidget(restartButton, 0, Qt::AlignHCenter | Qt::AlignBottom);
    setLayout(layout);

    connect(restartButton, &QPushButton::clicked, this, [this]() {
        this->hide();                 // Cacher le widget Game Over
        emit restartRequested();     // Émettre le signal
    });
}
