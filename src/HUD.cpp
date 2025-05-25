#include "HUD.h"


HUD::HUD(QWidget* parent) : QWidget(parent), score(0), ammo(0), timeElapsed(0) {
    setFixedSize(350, 215);

    // Police un peu plus moderne et agréable
    QFont font("Segoe UI", 14, QFont::Bold);
    if (!font.exactMatch()) { // fallback si non dispo
        font = QFont("Arial", 14, QFont::Bold);
    }

    // Création des labels "titres"
    QLabel* scoreTitle = new QLabel("🎯 Score:");
    QLabel* healthTitle = new QLabel("❤️ Vie:");
    QLabel* ammoTitle = new QLabel("🔫 Munitions:");
    QLabel* timeTitle = new QLabel("⏱ Temps:");
    QLabel* weaponTitle = new QLabel("🗡 Arme:");

    // Création des labels "valeurs"
    scoreLabel = new QLabel("0");
    healthLabel = new QLabel("100");
    ammoLabel = new QLabel("0");
    timeLabel = new QLabel("00:00");
    weaponLabel = new QLabel("-");

    QList<QLabel*> allTitles = {scoreTitle, healthTitle, ammoTitle, timeTitle, weaponTitle};
    QList<QLabel*> allValues = {scoreLabel, healthLabel, ammoLabel, timeLabel, weaponLabel};

    // Style commun titres
    for (auto lbl : allTitles) {
        lbl->setFont(font);
        lbl->setStyleSheet("color: #f0f0f0;");
        lbl->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    }

    // Style commun valeurs
    for (auto lbl : allValues) {
        lbl->setFont(font);
        lbl->setStyleSheet("color: #a0eaff;");
        lbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    }

    // Ombre portée légère sur les labels pour relief
    auto addShadow = [](QLabel* lbl) {
        QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(lbl);
        effect->setBlurRadius(4);
        effect->setOffset(1,1);
        effect->setColor(QColor(0,0,0,160));
        lbl->setGraphicsEffect(effect);
    };

    for(auto lbl : allTitles + allValues) {
        addShadow(lbl);
    }

    // Layout horizontal pour chaque ligne: titre + valeur
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(16);

    for (int i = 0; i < allTitles.size(); ++i) {
        QHBoxLayout* row = new QHBoxLayout();
        row->addWidget(allTitles[i], 1);
        row->addWidget(allValues[i], 1);
        mainLayout->addLayout(row);
    }
    setLayout(mainLayout);

    // Fond noir semi-transparent avec dégradé léger et bords arrondis
    setStyleSheet(R"(
        background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                    stop:0 rgba(0,0,0,220),
                                    stop:1 rgba(20,20,20,180));
        border-radius: 10px;
        border: 2px solid #00cfff;
    )");

    // Timer pour mise à jour temps
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
    timeLabel->setText(QString("%1:%2")
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


void HUD::setWeapon(Weapon* weapon) {
    this->weapon = weapon;
    if (weapon) {
        connect(weapon, &Weapon::ammoChanged, this, &HUD::updateAmmo);
        updateAmmo(weapon->getAmmo()); // Met à jour l'affichage immédiatement
    }
}


void HUD::pauseTimer() {
    if (timer) timer->stop();
}
void HUD::resumeTimer() {
    if (timer) timer->start(1000);
}

void HUD::updateWeaponLabel(const QString& weaponName) {
    weaponLabel->setText("Arme: " + weaponName);
}