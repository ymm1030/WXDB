#ifndef BASEDEFINE_H
#define BASEDEFINE_H

#include <QString>
#include <QDateTime>
#include <QVariant>

struct NamedData {
    QString         name;
    QVariant        data;

    NamedData() {}
    NamedData(const QString& _name, const QVariant& _data)
        : name(_name)
        , data(_data)
    {}
};

struct BoughtData
{
    QString     name;
    QString     good;
    QDateTime   timestamp;
    int         fin;
    QString     address;

    bool operator == (const BoughtData& ano) {
        return (name == ano.name) && (good == ano.good) && (timestamp == ano.timestamp)
                && (fin == ano.fin) && (address == ano.address);
    }
};

struct ColumnDetail
{
    QString     name;
    int         width;
};

enum FIN_STATUS
{
    FIN_STATUS_NOTCARE = 0,
    FIN_STATUS_YES,
    FIN_STATUS_NO
};

enum TIME_COMPARE_METHOD
{
    TIME_COMPARE_METHOD_NOTCARE = 0,
    TIME_COMPARE_METHOD_GREATER,
    TIME_COMPARE_METHOD_EQUAL,
    TIME_COMPARE_METHOD_LESS
};

struct RowData
{
    bool            special;
    QStringList     data;
};

#endif // BASEDEFINE_H
