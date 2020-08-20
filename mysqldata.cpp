#include <QVariant>
#include <QMessageBox>
#include <QDebug>
#include "mysqldata.h"
#include "dataeditor.h"
#include "basedefine.h"

struct AddressData
{
    QString     name;
    QString     address;
};

class PrivateMySqlData
{
public:
    QList<BoughtData>       boughtData;
    QString                 lastSelect;
};

MySqlData::MySqlData(QObject* parent)
    : MySqlBase(parent)
    , m_data(new PrivateMySqlData)
{

}

MySqlData::~MySqlData()
{
    delete m_data;
}

const QList<BoughtData>& MySqlData::data() const
{
    return m_data->boughtData;
}

bool MySqlData::open()
{
    return MySqlBase::open("s3.nsloop.com", 27575, "xxdxx", "ymm", "29510992");
}

void MySqlData::add(const QString &name, const QString &address, const QString &good)
{
    QList<QVariant> values;
    values << name << good << QDateTime::currentDateTime() << 0 << address;

    insert("insert into bought(name, good, timestamp, fin, address) values(?, ?, ?, ?, ?)", values);

    if (hasError()) {
        QMessageBox::warning(nullptr, "错误", errorString());
        return;
    }

    values.clear();
    values = select("select * from addresses where name='" + name + "' and address='" + address + "'");
    if (values.isEmpty()) {
        values << name << address;
        insert("insert into addresses(name, address) values(?, ?)", values);
    }

    if (hasError()) {
        QMessageBox::warning(nullptr, "错误", errorString());
        return;
    }

    refreshAfterInsert();
}

void MySqlData::refresh(const QString &nameFilter, const QString &addressFilter, const QString &goodsFilter,
                        int time_compare_method, const QString& time, int finStatus)
{
    QString exec("select * from bought where ");
    QString filterCondition("检索条件：\n");
    bool addAnd = false;
    if (!nameFilter.isEmpty()) {
        exec += "name like '%" + nameFilter + "%' ";
        addAnd = true;
        filterCondition += "ID: " + nameFilter + '\n';
    }

    if (!addressFilter.isEmpty()) {
        if (addAnd) {
            exec += "and ";
        }
        exec += "address like '%" + addressFilter + "%' ";
        addAnd = true;
        filterCondition += "地址: " + addressFilter + '\n';
    }

    if (!goodsFilter.isEmpty()) {
        if (addAnd) {
            exec += "and ";
        }
        exec += "good like '%" + goodsFilter + "%' ";
        addAnd = true;
        filterCondition += "货品: " + goodsFilter + '\n';
    }

    switch (time_compare_method) {
    case TIME_COMPARE_METHOD_GREATER:
        if (addAnd) {
            exec += "and ";
        }
        exec += "timestamp > '" + time + "' ";
        addAnd = true;
        filterCondition += "日期大于: " + time + '\n';
        break;
    case TIME_COMPARE_METHOD_EQUAL:
        if (addAnd) {
            exec += "and ";
        }
        exec += "timestamp like '%" + time + "%' ";
        addAnd = true;
        filterCondition += "日期等于: " + time + '\n';
        break;
    case TIME_COMPARE_METHOD_LESS:
        if (addAnd) {
            exec += "and ";
        }
        exec += "timestamp < '" + time + "' ";
        addAnd = true;
        filterCondition += "日期小于: " + time + '\n';
        break;
    default:
        break;
    }

    switch (finStatus) {
    case FIN_STATUS_YES:
        if (addAnd) {
            exec += "and ";
        }
        exec += "fin = 1 ";
        addAnd = true;
        filterCondition += "已发货： 是\n";
        break;
    case FIN_STATUS_NO:
        if (addAnd) {
            exec += "and ";
        }
        exec += "fin = 0 ";
        addAnd = true;
        filterCondition += "已发货： 否\n";
        break;
    default:
        break;
    }

    exec += "order by timestamp";

//    qDebug() << exec;
    QList<QVariant> values = select(exec);

    if (hasError()) {
        QMessageBox::warning(nullptr, "错误", errorString());
        return;
    }

    m_data->boughtData.clear();

    for (int i = 0; i < values.size(); ++i) {
        BoughtData d;
        d.name = values.at(i++).toString();
        d.good = values.at(i++).toString();
        d.timestamp = values.at(i++).toDateTime();
        d.fin = values.at(i++).toInt();
        d.address = values.at(i).toString();
        m_data->boughtData.push_back(d);
//        qDebug() << d.name << d.good << d.timestamp.date() << d.fin << d.address;
    }

    emit dataChanged();

    filterCondition = filterCondition.trimmed();
    QMessageBox::information(nullptr, "已完成", filterCondition);
    m_data->lastSelect = exec;
}

bool MySqlData::drop(const BoughtData &data)
{
    NamedData d;
    QList<NamedData>    values;

    d.name = "name";
    d.data = data.name;
    values << d;

    d.name = "address";
    d.data = data.address;
    values << d;

    d.name = "good";
    d.data = data.good;
    values << d;

    d.name = "timestamp";
    d.data = data.timestamp.toString(Qt::ISODate);
    values << d;

    d.name = "fin";
    d.data = data.fin;
    values << d;

    deleteData("bought", values);

    if (hasError()) {
        QMessageBox::warning(nullptr, "错误", errorString());
        return false;
    }
    return true;
}

void MySqlData::update(const BoughtData &oldData, const BoughtData &newData)
{
    QString exec("update bought set ");
    exec += "address='" + newData.address + "', ";
    exec += "good='" + newData.good + "', ";
    exec += "fin=" + QString::number(newData.fin) + " ";
    exec += "where";

    QList<NamedData> values;
    NamedData d;
    d.name = "name";
    d.data = oldData.name;
    values << d;

    d.name = "address";
    d.data = oldData.address;
    values << d;

    d.name = "good";
    d.data = oldData.good;
    values << d;

    d.name = "timestamp";
    d.data = oldData.timestamp.toString(Qt::ISODate);
    values << d;

    d.name = "fin";
    d.data = oldData.fin;
    values << d;

    MySqlBase::update(exec, values);

    if (hasError()) {
        QMessageBox::warning(nullptr, "错误", errorString());
        return;
    }

    refreshAfterInsert();

    QList<QVariant> results = select("select * from addresses where name='" + newData.name + "' and address='" + newData.address + "'");
    if (results.isEmpty()) {
        results << newData.name << newData.address;
        insert("insert into addresses(name, address) values(?, ?)", results);
    }

    if (hasError()) {
        QMessageBox::warning(nullptr, "错误", errorString());
    }
}

QStringList MySqlData::getAddresses(const QString &name)
{
    QStringList list;
    QString exec("select * from addresses where name='" + name + "'");
    QList<QVariant> values = select(exec);

    if (hasError()) {
        QMessageBox::warning(nullptr, "错误", errorString());
        return list;
    }

    for (int i = 1; i < values.size(); i+=2) {
        list << values.at(i).toString();
    }
    return list;
}

void MySqlData::removeAddress(const QString &name, const QString &address)
{
    NamedData d;
    QList<NamedData>    values;

    d.name = "name";
    d.data = name;
    values << d;

    d.name = "address";
    d.data = address;
    values << d;

    deleteData("addresses", values);

    if (hasError()) {
        QMessageBox::warning(nullptr, "错误", errorString());
    }
}

void MySqlData::refreshAfterInsert()
{
    if (m_data->lastSelect.isEmpty()) {
        return;
    }
    QList<QVariant> values = select(m_data->lastSelect);
    if (hasError()) {
        QMessageBox::warning(nullptr, "错误", errorString());
        return;
    }

    m_data->boughtData.clear();

    for (int i = 0; i < values.size(); ++i) {
        BoughtData d;
        d.name = values.at(i++).toString();
        d.good = values.at(i++).toString();
        d.timestamp = values.at(i++).toDateTime();
        d.fin = values.at(i++).toInt();
        d.address = values.at(i).toString();
        m_data->boughtData.push_back(d);
    }

    emit dataChanged();
}
