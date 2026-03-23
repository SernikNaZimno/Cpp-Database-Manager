#include "../include/MainWindow.h"
#include "../include/SQLiteBackend.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    dbTypeSelector = new QComboBox();
    dbTypeSelector->addItem("SQLite");
    dbTypeSelector->addItem("MySQL (Brak implementacji)");

    connectBtn = new QPushButton("Połącz / Utwórz Bazę");
    queryInput = new QTextEdit();
    queryInput->setPlaceholderText("Wpisz komendę SQL tutaj...");
    executeBtn = new QPushButton("Wykonaj SQL");
    resultView = new QTableView();

    queryModel = new QSqlQueryModel(this);
    resultView->setModel(queryModel);

    layout->addWidget(dbTypeSelector);
    layout->addWidget(connectBtn);
    layout->addWidget(queryInput);
    layout->addWidget(executeBtn);
    layout->addWidget(resultView);

    setCentralWidget(centralWidget);
    resize(800, 600);

    connect(connectBtn, &QPushButton::clicked, this, &MainWindow::handleConnect);
    connect(executeBtn, &QPushButton::clicked, this, &MainWindow::handleExecuteQuery);
}

MainWindow::~MainWindow() {}

void MainWindow::handleConnect() {
    if (dbTypeSelector->currentText() == "SQLite") {
        dbBackend = std::make_unique<SQLiteBackend>();

        // Wybór lokalizacji pliku dla SQLite
        QString fileName = QFileDialog::getSaveFileName(this, "Wybierz lokalizację bazy SQLite");
        if (!fileName.isEmpty()) {
            if (dbBackend->connectToDatabase(fileName)) {
                QMessageBox::information(this, "Sukces", "Połączono z bazą SQLite!");
            }
        }
    } else {
        QMessageBox::warning(this, "Brak wsparcia", "Ten silnik nie jest jeszcze zaimplementowany.");
    }
}

void MainWindow::handleExecuteQuery() {
    if (!dbBackend) {
        QMessageBox::warning(this, "Błąd", "Najpierw połącz się z bazą!");
        return;
    }

    QString sql = queryInput->toPlainText();
    QSqlQuery result = dbBackend->executeRawCommand(sql);

    // Podpinamy wyniki do tabeli w UI
    queryModel->setQuery(result);
}