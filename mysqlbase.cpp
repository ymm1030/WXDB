#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include "mysqlbase.h"

class PrivateMySqlBase
{
public:
    QSqlDatabase        db;
    QSqlError           lastError;
};

MySqlBase::MySqlBase(QObject *parent)
    : QObject(parent)
    , m_data(new PrivateMySqlBase)
{

}

MySqlBase::~MySqlBase()
{
    delete m_data;
}

bool MySqlBase::hasError() const
{
    return m_data->lastError.isValid();
}

QString MySqlBase::errorString() const
{
    return m_data->lastError.databaseText();
}

bool MySqlBase::open(const QString &host, int port, const QString &dbname, const QString &user, const QString &passwd)
{
    m_data->db = QSqlDatabase::addDatabase("QMYSQL");
    m_data->db.setHostName(host);
    m_data->db.setPort(port);
    m_data->db.setDatabaseName(dbname);
    m_data->db.setUserName(user);
    m_data->db.setPassword(passwd);
    return m_data->db.open();
}

QList<QVariant> MySqlBase::select(const QString &exec)
{
    QList<QVariant> results;
    QSqlQuery query(exec, m_data->db);
    m_data->lastError = query.lastError();
    while (query.next()) {
        int index = 0;
        int count = query.record().count();
        while (index < count) {
            results.push_back(query.value(index++));
        }
    }
    return results;
}

void MySqlBase::insert(const QString &exec, const QList<QVariant> &values)
{
    QSqlQuery query(m_data->db);
    query.prepare(exec);
    for (int i = 0; i < values.size(); ++i) {
        query.addBindValue(values.at(i));
    }
    query.exec();
    m_data->lastError = query.lastError();
}

void MySqlBase::update(const QString &exec, const QList<NamedData> &conditions)
{

    QString realExec = exec + " " + makeCondition(conditions);
    QSqlQuery query(realExec, m_data->db);
    m_data->lastError = query.lastError();
}

QString MySqlBase::makeCondition(const QList<NamedData> &data) const
{
    QString result;
    for (int i = 0; i < data.size(); ++i) {
        const NamedData& d = data.at(i);
        result += d.name;
        result += "=";
        if (d.data.type() == QVariant::String) {
            result += "'";
        }
        result += d.data.toString();
        if (d.data.type() == QVariant::String) {
            result += "'";
        }
        if (i != data.size() - 1) {
            result += " and ";
        }
    }
    return result;
}

void MySqlBase::deleteData(const QString &table, const QList<NamedData> &conditions)
{
    QString exec = "delete from " + table + " where " + makeCondition(conditions);
    QSqlQuery query(exec, m_data->db);
    m_data->lastError = query.lastError();
}
