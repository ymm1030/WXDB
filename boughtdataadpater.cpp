#include "boughtdataadpater.h"
#include "mysqldata.h"

class PrivateBoughtDataAdapter
{
public:
    MySqlData*      sqlData;
};

BoughtDataAdpater::BoughtDataAdpater(QObject* parent)
    : DataAdapterBase(parent)
    , m_data(new PrivateBoughtDataAdapter)
{
    m_data->sqlData = nullptr;
}

BoughtDataAdpater::~BoughtDataAdpater()
{
    delete m_data;
}
