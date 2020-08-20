#ifndef MYSQLBASE_H
#define MYSQLBASE_H

#include <QObject>
#include "basedefine.h"

class PrivateMySqlBase;
class MySqlBase : public QObject
{
    Q_OBJECT
public:
    explicit MySqlBase(QObject *parent = nullptr);
    ~MySqlBase();

    bool hasError() const;
    QString errorString() const;

protected:
    bool open(const QString& host, int port, const QString& dbname, const QString& user, const QString& passwd);
    QList<QVariant> select(const QString& exec);
    void insert(const QString& exec, const QList<QVariant>& values);
    void update(const QString& exec, const QList<NamedData>& conditions);
    void deleteData(const QString& table, const QList<NamedData>& conditions);

private:
    QString makeCondition(const QList<NamedData>& data) const;

private:
    PrivateMySqlBase*   m_data;
};

#endif // MYSQLBASE_H
