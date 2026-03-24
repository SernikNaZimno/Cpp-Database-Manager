#ifndef SQLITEBACKEND_H
#define SQLITEBACKEND_H

#include "IDatabaseBackend.h"
#include <QSqlDatabase>
#include <QSqlError>

class SQLiteBackend : public IDatabaseBackend {
private:
    QSqlDatabase db;

public:
    SQLiteBackend();
    ~SQLiteBackend();

    bool connectToDatabase(const QString& filepath) override;
    void disconnectDatabase() override;
    QSqlQuery executeRawCommand(const QString& command) override;
    bool createTable(const QString& tableName, const QString& schema) override;
    bool dropTable(const QString& tableName) override;
    QStringList getTables() override;
};

#endif