#ifndef GAMEOVERWIDGET_H
#define GAMEOVERWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QTableWidget>
#include <QFile>
#include <QTextStream>
#include <QHeaderView>
#include <QDebug>
#include <algorithm>

class GameOverWidget : public QWidget {
    Q_OBJECT

public:
    GameOverWidget(QWidget* parent = nullptr);
    void setScore(int score);

signals:
    void restartClicked();
    void restartRequested();

private slots:
    void saveScore();

private:
    QLabel* label;
    QPushButton* restartButton;
    QLineEdit* nameInput;
    QTableWidget* highScoreTable;
    int currentScore;

    void loadHighScores();
    void updateHighScoreTable();
};

#endif // GAMEOVERWIDGET_H
