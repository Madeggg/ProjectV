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
        // mainView->fitInView(mainScene->sceneRect(), Qt::KeepAspectRatioByExpanding);

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
    mainView->scale(2.5, 2.5);

    this->showFullScreen();


    // --- Création du HUD ---
    hud = new HUD(mainView);
    hud->move(10, 10);  // fixe le HUD en haut à gauche
    hud->setAttribute(Qt::WA_TransparentForMouseEvents); // clics passent à la vue dessous
    hud->show();
    mainScene->setHUD(hud); 

    connect(mainScene->getPlayer(), &Player::scoreChanged, hud, &HUD::setScore);
    connect(mainScene->getPlayer(), &Player::healthChanged, hud, &HUD::updateHealth);
    connect(mainScene->getPlayer(), &Player::weaponChanged, hud, &HUD::setWeapon);





    connect(mainScene->getPlayer(), &Player::playerMoved, this, [this]() {
        mainView->centerOn(mainScene->getPlayer());    
    });


    Weapon* weapon = mainScene->getPlayer()->getCurrentWeapon();
    if (weapon) {
        connect(weapon, &Weapon::ammoChanged, hud, &HUD::updateAmmo);
    }

    connect(mainScene->getGameManager(), &GameManager::gameOver, this, &MainWindow::showGameOverMenu);

    resize(600, 800);
}


void MainWindow::showGameOverMenu() {
    if (!gameOverWidget) {
        gameOverWidget = new GameOverWidget(mainView);
        gameOverWidget->move((mainView->width() - gameOverWidget->width()) / 2,
                             (mainView->height() - gameOverWidget->height()) / 2);
        connect(gameOverWidget, &GameOverWidget::restartClicked, this, &MainWindow::restartGame);
    }
    if (hud) {
        gameOverWidget->setScore(hud->getScore());
    }

    gameOverWidget->show();

    if (mainScene) {
        mainScene->getGameManager()->setPause(true);
        if (mainScene->timer) mainScene->timer->stop();
    }
}


void MainWindow::restartGame() {
    if (gameOverWidget) {
        gameOverWidget->hide();
        delete gameOverWidget;
        gameOverWidget = nullptr;
    }

    // Réinitialiser la scène et la vue
    if (mainView) {
        delete mainView;
        mainView = nullptr;
    }
    if (mainScene) {
        delete mainScene;
        mainScene = nullptr;
    }

    // Retourner au menu principal
    mainMenu = new MainMenuWidget(this); // Créer un nouveau menu
    setCentralWidget(mainMenu);
    mainMenu->setGeometry(this->geometry()); // Assure-toi que le menu occupe toute la fenêtre

    // Connecter à nouveau l'événement de démarrage du jeu
    connect(mainMenu, &MainMenuWidget::startGameRequested, this, &MainWindow::startGame);


    if (mainScene) {
        mainScene->getGameManager()->setPause(false);
    }

}


