#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{

    this->mainScene = new MyScene;

    this->mainView = new QGraphicsView;
    this->mainView->setScene(mainScene);

    this->setCentralWidget(mainView);
    this->setWindowTitle("My main window");
    this->resize(400, 800);


     // Connecte le signal playerMoved au recentrage de la vue
     connect(mainScene->getPlayer(), &Player::playerMoved, this, [this]() {
        mainView->centerOn(mainScene->getPlayer());
    });

    mainView->scale(5,5);


    helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction* actionHelp = new QAction(tr("&About"), this);
    connect(actionHelp, SIGNAL(triggered()), this, SLOT(slot_aboutMenu()));
    helpMenu->addAction(actionHelp);

}

MainWindow::~MainWindow(){
    delete mainScene;
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
        Player* player = mainScene->getPlayer(); // Ajoutez une méthode getPlayer() dans MyScene
        if (player) {
            mainView->centerOn(player);
        }
    }
}

