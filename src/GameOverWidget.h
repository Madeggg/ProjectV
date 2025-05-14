#ifndef GAMEOVERWIDGET_H
#define GAMEOVERWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

class GameOverWidget : public QWidget {
    Q_OBJECT

public:
    GameOverWidget(QWidget* parent = nullptr);

signals:
    void restartClicked();
    void restartRequested();

private:
    QLabel* label;
    QPushButton* restartButton;
};

#endif // GAMEOVERWIDGET_H
