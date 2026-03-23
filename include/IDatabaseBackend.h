#ifndef IDATABASEBACKEND_H
#define IDATABASEBACKEND_H

#include <QString>
#include <QVariantMap>
#include <QSqlQuery>

class IDatabaseBackend {
public:
    virtual ~IDatabaseBackend() = default;

    virtual bool connectToDatabase(const QString& locationOrCredentials) = 0;
    virtual void disconnectDatabase() = 0;

    virtual QSqlQuery executeRawCommand(const QString& command) = 0;

    virtual bool createTable(const QString& tableName, const QString& schema) = 0;
    virtual bool dropTable(const QString& tableName) = 0;
};

#endif