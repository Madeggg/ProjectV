#include "GameOverWidget.h"

GameOverWidget::GameOverWidget(QWidget* parent) : QWidget(parent) {
    setStyleSheet("background-color: rgba(0, 0, 0, 180); color: white; font-size: 24px;");
    // setFixedSize(2000, 2000); fixer la taille de l'écran mais pas optimal.

    if (parent)
        this->setGeometry(parent->rect());

    label = new QLabel("Game Over", this);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("background-color: #222; color: white; font-size: 64px; font-weight: bold;");
    label->setGeometry(0, height() / 4, width(), 100); // centré en haut

    restartButton = new QPushButton("Restart", this);
    restartButton->setStyleSheet("font-size: 24px; padding: 10px 20px;");
    connect(restartButton, &QPushButton::clicked, this, &GameOverWidget::restartClicked);

    int btnWidth = 200;
    int btnHeight = 50;
    restartButton->setGeometry((width() - btnWidth) / 2, (height() - btnHeight) / 2, btnWidth, btnHeight);

    connect(restartButton, &QPushButton::clicked, this, [this]() {
        this->hide();                 // Cacher le widget Game Over
        emit restartRequested();     // Émettre le signal
    });
}
