#include "GameOverWidget.h"

GameOverWidget::GameOverWidget(QWidget* parent) : QWidget(parent) {
    setStyleSheet("background-color: rgba(0, 0, 0, 180); color: white; font-size: 24px;");
    setFixedSize(300, 200);

    label = new QLabel("Game Over", this);
    label->setAlignment(Qt::AlignCenter);

    restartButton = new QPushButton("Restart", this);
    connect(restartButton, &QPushButton::clicked, this, &GameOverWidget::restartClicked);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(label);
    layout->addWidget(restartButton);
    setLayout(layout);

    connect(restartButton, &QPushButton::clicked, this, [this]() {
    this->hide();                 // Cacher le widget Game Over
    emit restartRequested();     // Émettre le signal
});
}
