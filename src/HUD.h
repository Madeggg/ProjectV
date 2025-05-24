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
    void setAmmo(int n);
    void setScore(int score);

    void setWeapon(Weapon* weapon);
    int getScore() const { return score; }


    //Timer
    void pauseTimer();
    void resumeTimer();

private slots:
    void updateTime();

public slots:
    void updateHealth(int newHealth);
    void updateAmmo(int newAmmo);
    void updateTime(int seconds);
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
 