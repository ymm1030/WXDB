#include <QScrollBar>
#include <QDebug>
#include "dataarea.h"
#include "dataeditor.h"
#include "tabletitle.h"

class PrivateDataArea
{
public:
    DataEditor*     editor;
    QScrollBar*     hbar;
    QScrollBar*     vbar;
    TableTitle*     tableTitle;
};

DataArea::DataArea(QWidget *parent)
    : QWidget(parent)
    , m_data(new PrivateDataArea)
{
    m_data->editor = new DataEditor(this);
    connect(m_data->editor, SIGNAL(barValueChanged(int, int)), this, SLOT(barValueChanged(int, int)));

    m_data->hbar = new QScrollBar(this);
    m_data->hbar->setOrientation(Qt::Horizontal);
    m_data->hbar->setMinimum(0);
    m_data->hbar->setMaximum(0);
    connect(m_data->hbar, SIGNAL(valueChanged(int)), m_data->editor, SLOT(hbarValueChanged(int)));
    m_data->vbar = new QScrollBar(this);
    m_data->vbar->setOrientation(Qt::Vertical);
    m_data->vbar->setMinimum(0);
    m_data->vbar->setMaximum(0);
    connect(m_data->vbar, SIGNAL(valueChanged(int)), m_data->editor, SLOT(vbarValueChanged(int)));

    m_data->tableTitle = new TableTitle(this);
    connect(m_data->hbar, SIGNAL(valueChanged(int)), m_data->tableTitle, SLOT(hvalueChanged(int)));

    connect(m_data->editor, SIGNAL(mouseWheelValue(int)), this, SLOT(mouseWheelValue(int)));

    QList<ColumnDetail> details;
    ColumnDetail columnDetail;
    columnDetail.name = "ID";
    columnDetail.width = 200;
    details << columnDetail;

    columnDetail.name = "地址";
    columnDetail.width = 500;
    details << columnDetail;

    columnDetail.name = "买的东西";
    columnDetail.width = 500;
    details << columnDetail;

    columnDetail.name = "购买日期";
    columnDetail.width = 150;
    details << columnDetail;

    columnDetail.name = "已发";
    columnDetail.width = 50;
    details << columnDetail;

    m_data->tableTitle->setColumnDetails(details);

//    m_data->editor->refresh("test", "", "", TIME_COMPARE_METHOD_NOTCARE, "", FIN_STATUS_NOTCARE);
//    m_data->editor->refresh("", "chongqing", "", TIME_COMPARE_METHOD_NOTCARE, "", FIN_STATUS_NOTCARE);
//    m_data->editor->refresh("", "", "changguan", TIME_COMPARE_METHOD_NOTCARE, "", FIN_STATUS_NOTCARE);
//    m_data->editor->refresh("", "", "", TIME_COMPARE_METHOD_GREATER, "2020-08-10", FIN_STATUS_NOTCARE);
//    m_data->editor->refresh("", "", "", TIME_COMPARE_METHOD_EQUAL, "2020-08-10", FIN_STATUS_NOTCARE);
//    m_data->editor->refresh("", "", "", TIME_COMPARE_METHOD_LESS, "2020-08-10", FIN_STATUS_NOTCARE);
//    m_data->editor->refresh("", "", "", TIME_COMPARE_METHOD_NOTCARE, "", FIN_STATUS_YES);
//    m_data->editor->refresh("", "", "", TIME_COMPARE_METHOD_NOTCARE, "", FIN_STATUS_NO);
//    m_data->editor->refresh("test", "chongqing", "changguan", TIME_COMPARE_METHOD_GREATER, "2020-08-10", FIN_STATUS_YES);
}

DataArea::~DataArea()
{
    delete m_data;
}

void DataArea::barValueChanged(int hvalue, int vvalue)
{
    m_data->hbar->setMaximum(hvalue);
    m_data->hbar->setPageStep(m_data->editor->width());
    m_data->vbar->setMaximum(vvalue);
    m_data->vbar->setPageStep(m_data->editor->height());
}

void DataArea::requestInsert(const QString& name, const QString& address, const QString& good)
{
    m_data->editor->insert(name, address, good);
}

void DataArea::requestRefresh(const QString& nameFilter, const QString& addressFilter, const QString& goodsFilter,
                              int time_compare_method, const QString& time, int finStatus)
{
    m_data->editor->refresh(nameFilter, addressFilter, goodsFilter, time_compare_method, time, finStatus);
}

void DataArea::mouseWheelValue(int value)
{
    m_data->vbar->setValue(m_data->vbar->value() + value);
}

void DataArea::resizeEvent(QResizeEvent *)
{
    m_data->tableTitle->setGeometry(0, 0, width() - 20, 50);
    m_data->editor->setGeometry(0, 50, width() - 20, height() - 20 - 50);
    m_data->vbar->setGeometry(width() - 20, 0, 20, height());
    m_data->hbar->setGeometry(0, height() - 20, width(), 20);
}
