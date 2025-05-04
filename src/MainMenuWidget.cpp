#include "MainMenuWidget.h"

MainMenuWidget::MainMenuWidget(QWidget* parent) : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);

    QPushButton* newGameButton = new QPushButton("Nouvelle Partie", this);
    QPushButton* quitButton = new QPushButton("Quitter", this);

    layout->addWidget(newGameButton);
    layout->addWidget(quitButton);

    connect(newGameButton, &QPushButton::clicked, this, &MainMenuWidget::startGameRequested);
    connect(quitButton, &QPushButton::clicked, qApp, &QApplication::quit);
}
