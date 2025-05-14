#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>



#include "MyScene.h"
#include "MainMenuWidget.h"
#include "GameOverWidget.h"

class MainMenuWidget; // Déclaration anticipée
class MyScene; // Déclaration anticipée
class GameOverWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

protected:
    void resizeEvent(QResizeEvent* event) override;
private :
    MyScene* mainScene;
    QGraphicsView* mainView;
    QMenu* helpMenu;

    MainMenuWidget* mainMenu;

    GameOverWidget* gameOverWidget = nullptr;

    void showGameOverMenu();
    void restartGame();


public:
    MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow();
    void startGame();

public slots:
    void slot_aboutMenu();
};


#endif //CPP_QT_TPMINIPROJET_MAINWINDOW_H
