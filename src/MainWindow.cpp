#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    mainMenu = new MainMenuWidget(this);
    setCentralWidget(mainMenu);
    setWindowTitle("Project V");
    resize(600, 800);

    connect(mainMenu, &MainMenuWidget::startGameRequested, this, &MainWindow::startGame);
}

MainWindow::~MainWindow(){
}

void MainWindow::slot_aboutMenu(){
    QMessageBox msgBox;
    msgBox.setText("A small QT/C++ projet...");
    msgBox.setModal(true); // on souhaite que la fenetre soit modale i.e qu'on ne puisse plus cliquer ailleurs
    msgBox.exec();
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
    if (mainView && mainScene) {
        // Ajuste la vue pour garder le ratio de l'image de fond
        mainView->fitInView(mainScene->sceneRect(), Qt::KeepAspectRatioByExpanding);

        // Centre la vue sur le joueur
        Player* player = mainScene->getPlayer(); 
        if (player) {
            mainView->centerOn(player);
        }
    }
}


void MainWindow::startGame() {
    mainScene = new MyScene;
    mainView = new QGraphicsView;
    mainView->setScene(mainScene);

    setCentralWidget(mainView);
    mainView->setMouseTracking(true);
    mainView->setFocusPolicy(Qt::StrongFocus);
    mainView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainView->scale(5, 5);

    connect(mainScene->getPlayer(), &Player::playerMoved, this, [this]() {
        mainView->centerOn(mainScene->getPlayer());
    });

    resize(600, 800);
}

