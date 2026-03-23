#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>
#include <QTableView>
#include <QComboBox>
#include <QVBoxLayout>
#include <QSqlQueryModel>
#include <memory>
#include "IDatabaseBackend.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    std::unique_ptr<IDatabaseBackend> dbBackend;
    QSqlQueryModel* queryModel;

    // Elementy UI
    QComboBox* dbTypeSelector;
    QPushButton* connectBtn;
    QTextEdit* queryInput;
    QPushButton* executeBtn;
    QTableView* resultView;

private slots:
    void handleConnect();
    void handleExecuteQuery();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif