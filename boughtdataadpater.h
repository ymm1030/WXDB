#ifndef BOUGHTDATAADPATER_H
#define BOUGHTDATAADPATER_H

#include "dataadapterbase.h"

class MySqlData;
class PrivateBoughtDataAdapter;
class BoughtDataAdpater : public DataAdapterBase
{
    Q_OBJECT
public:
    BoughtDataAdpater(QObject* parent = nullptr);
    ~BoughtDataAdpater();

    void installDataHandler(MySqlData* dataHandler);

private:
    PrivateBoughtDataAdapter*       m_data;
};

#endif // BOUGHTDATAADPATER_H
