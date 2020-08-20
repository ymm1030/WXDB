#ifndef DATAADAPTERBASE_H
#define DATAADAPTERBASE_H

#include <QObject>
#include "basedefine.h"

class DataAdapterBase : public QObject
{
    Q_OBJECT
public:
    explicit DataAdapterBase(QObject *parent = nullptr);
    virtual QList<RowData> getRowData() const = 0;

signals:
    void rowDataChanged();
};

#endif // DATAADAPTERBASE_H
