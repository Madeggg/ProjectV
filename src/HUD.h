#ifndef HUD_H
#define HUD_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include "Entities.h" // Pour Weapon

class Weapon; // Déclaration anticipée

class HUD : public QWidget {
    Q_OBJECT
public:
    explicit HUD(QWidget* parent = nullptr);

    void addScore(int points);
    void setAmmo(int n);            // Pour mettre à jour les munitions
    void setScore(int score);

    void setWeapon(Weapon* weapon);
    int getScore() const { return score; }

    void pauseTimer();
    void resumeTimer();

private slots:
    void updateTime();              // Mise à jour du timer interne (timer à 1s)

public slots:
    void updateHealth(int newHealth);
    void updateAmmo(int newAmmo);   // Slot appelé par Weapon
    void updateTime(int seconds);   // Mise à jour externe du timer (optionnel)
    void addPoints(int points);
    void updateWeaponLabel(const QString& weaponName);

private:
    QLabel* scoreLabel;
    QLabel* ammoLabel;
    QLabel* timeLabel;
    QLabel* healthLabel;
    QLabel* weaponLabel;

    int score;
    int ammo;
    int timeElapsed;

    QTimer* timer;

    Weapon* weapon = nullptr;
};

#endif // HUD_H
