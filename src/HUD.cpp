#include "HUD.h"
#include <QVBoxLayout>
#include <QFont>

HUD::HUD(QWidget* parent) : QWidget(parent), score(0), ammo(10), timeElapsed(0) {
    setFixedSize(200, 120);

    // Police simple et lisible
    QFont font("Arial", 14, QFont::Bold);

    scoreLabel = new QLabel("Score: 0", this);
    ammoLabel = new QLabel("Munitions: 10", this);
    timeLabel = new QLabel("Temps: 00:00", this);
    healthLabel = new QLabel("Vie: 100", this);

    // Appliquer la police et la couleur à tous les labels
    QList<QLabel*> labels = {scoreLabel, healthLabel, ammoLabel, timeLabel};
    for(auto label : labels) {
        label->setFont(font);
        label->setStyleSheet("color: white;");
        label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    }

    // Layout vertical simple avec espacement
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(8);
    layout->addWidget(scoreLabel);
    layout->addWidget(healthLabel);
    layout->addWidget(ammoLabel);
    layout->addWidget(timeLabel);
    setLayout(layout);

    // Fond noir opaque pour bien voir
    setStyleSheet("background-color: black; border-radius: 5px;");

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, static_cast<void (HUD::*)()>(&HUD::updateTime));
    timer->start(1000);
}

void HUD::addScore(int points) {
    score += points;
    scoreLabel->setText("Score: " + QString::number(score));
}


void HUD::updateTime() {
    timeElapsed++;
    int minutes = timeElapsed / 60;
    int seconds = timeElapsed % 60;
    timeLabel->setText(QString("Temps: %1:%2")
                       .arg(minutes, 2, 10, QChar('0'))
                       .arg(seconds, 2, 10, QChar('0')));
}

void HUD::updateHealth(int newHealth) {
    healthLabel->setText(QString("Vie: %1").arg(newHealth));
}

void HUD::updateAmmo(int newAmmo) {
    ammo = newAmmo;
    ammoLabel->setText(QString("Munitions: %1").arg(ammo));
}

void HUD::updateTime(int seconds) {
    int minutes = seconds / 60;
    int secs = seconds % 60;
    timeLabel->setText(QString("Temps: %1:%2")
                       .arg(minutes, 2, 10, QChar('0'))
                       .arg(secs, 2, 10, QChar('0')));
}

void HUD::addPoints(int points) {
    addScore(points);
}

void HUD::setScore(int newScore) {
    score = newScore;
    scoreLabel->setText("Score: " + QString::number(score));
}
