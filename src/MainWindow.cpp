#include "../include/MainWindow.h"
#include "../include/SQLiteBackend.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QIcon>
#include <QDir>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    auto* centralWidget = new QWidget(this);
    // Ustawiamy główny layout jako poziomy (HBox), aby objął całego splittera
    auto* mainLayout = new QHBoxLayout(centralWidget);

    // --- LEWA STRONA (Eksplorator) ---
    dbExplorer = new QTreeWidget();
    dbExplorer->setHeaderLabel("Tabele w bazie");
    dbExplorer->setMinimumWidth(200); // Żeby nie był na starcie zbyt wąski

    // --- PRAWA STRONA (Panel operacyjny) ---
    auto* rightPanel = new QWidget();
    auto* rightLayout = new QVBoxLayout(rightPanel); // Stary layout przenosimy tutaj

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

    // Pakujemy wszystkie Twoje dotychczasowe elementy do prawego panelu
    rightLayout->addWidget(dbTypeSelector);
    rightLayout->addWidget(connectBtn);
    rightLayout->addWidget(queryInput);
    rightLayout->addWidget(executeBtn);
    rightLayout->addWidget(resultView);

    // --- ŁĄCZENIE (Splitter) ---
    // QSplitter to specjalny kontener, który daje użytkownikowi suwak między elementami
    auto* splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(dbExplorer);
    splitter->addWidget(rightPanel);

    // Dodajemy splitter do głównego widoku
    mainLayout->addWidget(splitter);

    setCentralWidget(centralWidget);
    resize(900, 600); // Trochę poszerzyłem okno na start

    connect(connectBtn, &QPushButton::clicked, this, &MainWindow::handleConnect);
    connect(executeBtn, &QPushButton::clicked, this, &MainWindow::handleExecuteQuery);

    setWindowIcon(QIcon(":/icon.ico"));
    setWindowTitle("Universal Database Manager");
}
MainWindow::~MainWindow() = default;

void MainWindow::handleConnect() {
    if (dbTypeSelector->currentText() == "SQLite") {
        dbBackend = std::make_unique<SQLiteBackend>();

        QString fileName = QFileDialog::getSaveFileName(this, "Wybierz lokalizację bazy SQLite");
        if (!fileName.isEmpty()) {
            if (dbBackend->connectToDatabase(fileName)) {
                QMessageBox::information(this, "Sukces", "Połączono z bazą SQLite!");

                // --- DODANO ---
                // Po udanym połączeniu wywołujemy odświeżenie listy tabel w panelu bocznym
                refreshTableList();
            }
        }
    } else {
        QMessageBox::warning(this, "Brak wsparcia", "Ten silnik nie jest jeszcze zaimplementowany.");
    }
}

void MainWindow::refreshTableList() const {
    dbExplorer->clear(); // Czyścimy listę na wypadek, gdybyśmy łączyli się z kolejną bazą
    if (!dbBackend) return;

    QStringList tables = dbBackend->getTables();

    for (const QString& tableName : tables) {
        // Tworzymy nowy "liść" w drzewie dla każdej nazwy tabeli
        auto* item = new QTreeWidgetItem(dbExplorer);
        item->setText(0, tableName);
        item->setIcon(0, QIcon(":/icon.ico")); // Dla ozdoby używamy Twojej ikony
    }
}

void MainWindow::handleExecuteQuery() {
    if (!dbBackend) {
        QMessageBox::warning(this, "Błąd", "Najpierw połącz się z bazą!");
        return;
    }

    QString sql = queryInput->toPlainText();
    QSqlQuery result = dbBackend->executeRawCommand(sql);

    queryModel->setQuery(std::move(result));
}