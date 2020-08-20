#ifndef MYSQLDATA_H
#define MYSQLDATA_H

#include "mysqlbase.h"

class PrivateMySqlData;
class MySqlData : public MySqlBase
{
    Q_OBJECT
public:
    MySqlData(QObject* parent = nullptr);
    ~MySqlData();

    const QList<BoughtData>& data() const;

    bool open();
    void add(const QString& name, const QString& address, const QString& good);
    void refresh(const QString& nameFilter, const QString& addressFilter, const QString& goodsFilter,
                 int time_compare_method, const QString& time, int finStatus);
    bool drop(const BoughtData& data);
    void update(const BoughtData& oldData, const BoughtData& newData);

    QStringList getAddresses(const QString& name);
    void removeAddress(const QString& name, const QString& address);

signals:
    void dataChanged();

private:
    void refreshAfterInsert();

private:
    PrivateMySqlData*       m_data;
};

#endif // MYSQLDATA_H
