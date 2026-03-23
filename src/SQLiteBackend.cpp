#include "../include/SQLiteBackend.h"
#include <QDebug>

SQLiteBackend::SQLiteBackend() {
    // Rejestrujemy sterownik SQLite w Qt
    db = QSqlDatabase::addDatabase("QSQLITE");
}

SQLiteBackend::~SQLiteBackend() {
    SQLiteBackend::disconnectDatabase();
}

bool SQLiteBackend::connectToDatabase(const QString& filepath) {
    db.setDatabaseName(filepath); // W SQLite to ścieżka do pliku
    if (!db.open()) {
        qDebug() << "Błąd łączenia z SQLite:" << db.lastError().text();
        return false;
    }
    return true;
}

void SQLiteBackend::disconnectDatabase() {
    if (db.isOpen()) {
        db.close();
    }
}

QSqlQuery SQLiteBackend::executeRawCommand(const QString& command) {
    QSqlQuery query(db);
    if (!query.exec(command)) {
        qDebug() << "Błąd wykonania zapytania:" << query.lastError().text();
    }
    return query; // Zwracamy wynik, który Frontend podepnie pod widok tabeli
}

bool SQLiteBackend::createTable(const QString& tableName, const QString& schema) {
    QString sql = QString("CREATE TABLE %1 (%2)").arg(tableName, schema);
    return executeRawCommand(sql).isActive();
}

bool SQLiteBackend::dropTable(const QString& tableName) {
    QString sql = QString("DROP TABLE %1").arg(tableName);
    return executeRawCommand(sql).isActive();
}