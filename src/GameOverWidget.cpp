#include "GameOverWidget.h"


GameOverWidget::GameOverWidget(QWidget* parent) : QWidget(parent), currentScore(0) {
    setStyleSheet("background-color: black; color: white; font-size: 24px;");
    if (parent)
        setGeometry(parent->rect());

    QVBoxLayout* layout = new QVBoxLayout(this);

    label = new QLabel("Game Over");
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("font-size: 48px; font-weight: bold;");
    layout->addWidget(label);

    nameInput = new QLineEdit(this);
    nameInput->setPlaceholderText("Entrez votre pseudo...");
    layout->addWidget(nameInput);

    restartButton = new QPushButton("Restart");
    restartButton->setStyleSheet("font-size: 24px;");
    layout->addWidget(restartButton);
    connect(restartButton, &QPushButton::clicked, this, &GameOverWidget::saveScore);
    connect(restartButton, &QPushButton::clicked, this, [this]() {
        this->hide();
        emit restartClicked();
    });

    highScoreTable = new QTableWidget(0, 2, this);
    highScoreTable->setHorizontalHeaderLabels(QStringList() << "Nom" << "Score");
    highScoreTable->horizontalHeader()->setStretchLastSection(true);
    highScoreTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    highScoreTable->setSelectionMode(QAbstractItemView::NoSelection);
    highScoreTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    highScoreTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

    layout->addWidget(highScoreTable);

    loadHighScores();
}

void GameOverWidget::setScore(int score) {
    currentScore = score;
}

void GameOverWidget::saveScore() {
    QString name = nameInput->text().trimmed();
    if (name.isEmpty()) name = "Anonyme";

    QFile file("highscores.txt");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << name << ";" << currentScore << "\n";
        file.close();
    }

    loadHighScores();
}

void GameOverWidget::loadHighScores() {
    QList<QPair<QString, int>> scores;

    QFile file("highscores.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(';');
            if (parts.size() == 2) {
                QString name = parts[0];
                int score = parts[1].toInt();
                scores.append(qMakePair(name, score));
            }
        }
        file.close();
    }

    std::sort(scores.begin(), scores.end(), [](const QPair<QString, int>& a, const QPair<QString, int>& b) {
        return b.second < a.second; // ordre décroissant
    });

    highScoreTable->setRowCount(0);
    for (int i = 0; i < qMin(3, scores.size()); ++i) {
        highScoreTable->insertRow(i);
        highScoreTable->setItem(i, 0, new QTableWidgetItem(scores[i].first));
        highScoreTable->setItem(i, 1, new QTableWidgetItem(QString::number(scores[i].second)));
    }
}
