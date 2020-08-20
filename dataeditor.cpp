#include <QPainter>
#include <QMessageBox>
#include <QDebug>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QMenu>
#include <QAction>
#include "dataeditor.h"
#include "mysqldata.h"
#include "modifypopup.h"

static const int cell_name_width = 200;
static const int cell_address_width = 500;
static const int cell_goods_width = 500;
static const int cell_time_width = 150;
static const int cell_fin_width = 50;
static const int cell_height = 50;

class PrivateDataEditor
{
public:
    bool                success;
    MySqlData           data;
    QList<BoughtData>   selectedData;
    int                 h_position;
    int                 v_position;
    int                 currentSelected;
    QMenu*              contextMenu;
};

DataEditor::DataEditor(QWidget *parent)
    : QWidget(parent)
    , m_data(new PrivateDataEditor)
{
    m_data->success = true;
    if (!m_data->data.open()) {
        m_data->success = false;
        QMessageBox::warning(nullptr, "错误", "数据库连接错误，请检查网络是否正常。");
    }
    m_data->h_position = 0;
    m_data->v_position = 0;
    m_data->currentSelected = -1;
    connect(&m_data->data, SIGNAL(dataChanged()), this, SLOT(dataChanged()));

//    BoughtData d;
//    d.name = "Test";
//    d.address = "江苏省南京市长江大桥江苏省南京市长江大桥江苏省南京市长江大桥江苏省南京市长江大桥";
//    d.good = "银座洗面奶-2 长管隔离-3";
//    d.timestamp = QDateTime::currentDateTime();
//    d.fin = 0;

//    for (int i = 0; i < 50; i++) {
//        d.name = QString("Test") + QString::number(i);
//        m_data->selectedData << d;
//    }

    m_data->contextMenu = new QMenu;
    m_data->contextMenu->addAction("删除");
    m_data->contextMenu->addAction("修改");
    connect(m_data->contextMenu, SIGNAL(triggered(QAction*)), this, SLOT(menuTriggered(QAction*)));
}

DataEditor::~DataEditor()
{
    delete m_data;
}

void DataEditor::refresh(const QString &nameFilter, const QString &addressFilter, const QString &goodsFilter,
                         int time_compare_method, const QString& time, int finStatus)
{
    if (!m_data->success) {
        if (QMessageBox::Ok == QMessageBox::question(nullptr, "错误", "数据库连接错误，是否重试？")) {
            if (!m_data->data.open()) {
                QMessageBox::warning(nullptr, "错误", "数据库连接仍然错误，请检查网络是否正常。");
                return;
            }
            else {
                m_data->success = true;
            }
        }
    }

    m_data->data.refresh(nameFilter, addressFilter, goodsFilter,
                         time_compare_method, time, finStatus);
}

void DataEditor::insert(const QString &name, const QString &address, const QString &good)
{
    if (!m_data->success) {
        if (QMessageBox::Ok == QMessageBox::question(nullptr, "错误", "数据库连接错误，是否重试？")) {
            if (!m_data->data.open()) {
                QMessageBox::warning(nullptr, "错误", "数据库连接仍然错误，请检查网络是否正常。");
                return;
            }
            else {
                m_data->success = true;
            }
        }
    }

    m_data->data.add(name, address, good);
}

void DataEditor::dataChanged()
{
    m_data->selectedData = m_data->data.data();
    m_data->v_position = 0;
    m_data->h_position = 0;
    m_data->currentSelected = -1;

    resetBarValues();
    update();
}

void DataEditor::hbarValueChanged(int value)
{
    m_data->h_position = value;
    update();
}

void DataEditor::vbarValueChanged(int value)
{
    m_data->v_position = value;
    update();
}

void DataEditor::menuTriggered(QAction* action)
{
    if (action->text() == "删除") {
        if (QMessageBox::Yes == QMessageBox::question(nullptr, "确认", "确定删除本条吗？")) {
            if (m_data->data.drop(m_data->selectedData.at(m_data->currentSelected))) {
                m_data->selectedData.removeAt(m_data->currentSelected);
                m_data->currentSelected = -1;
                update();
            }
        }
    }
    else if (action->text() == "修改") {
        openModifyPopUp();
    }
}

void DataEditor::requestUpdate(const BoughtData& data)
{
    BoughtData d = data;
    BoughtData d1 = m_data->selectedData.at(m_data->currentSelected);
    d.timestamp = d1.timestamp;
    if (d == d1) {
        return;
    }
    m_data->data.update(d1, d);
}

void DataEditor::requestRemoveAddress(const QString& name, const QString& address)
{
    m_data->data.removeAddress(name, address);
}

void DataEditor::restoreCurrentSelected(int currentSelected)
{
    m_data->currentSelected = currentSelected;
    update();
}

void DataEditor::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int startIndex = m_data->v_position / cell_height;
    int startY = startIndex * cell_height - m_data->v_position;
    int endIndex = (m_data->v_position + height()) / cell_height + 1;

    int startX = -m_data->h_position;

    QPen gridPen;
    gridPen.setColor(QColor(57, 57, 57, 80));
    gridPen.setWidth(2);

    QPen textPen;
    textPen.setColor(QColor(5, 5, 5));

    for (int i = startIndex; i < m_data->selectedData.size() && i < endIndex; ++i) {
        BoughtData d = m_data->selectedData.at(i);
        int y = startY + (i-startIndex) * cell_height;

        if (i == m_data->currentSelected) {
            painter.setPen(Qt::NoPen);
            painter.setBrush(QColor(48, 84, 179, 150));
            painter.drawRect(QRect(0, y, width(), cell_height));
        }

        painter.setBrush(Qt::NoBrush);


        QRect r1(startX, y, cell_name_width, cell_height);
        painter.setPen(gridPen);
        painter.drawRect(r1);
        painter.setPen(textPen);
        painter.drawText(r1, Qt::AlignCenter, d.name);

        QRect r2(r1.right(), y, cell_address_width, cell_height);
        painter.setPen(gridPen);
        painter.drawRect(r2);
        painter.setPen(textPen);
        painter.drawText(r2.adjusted(3, 0, 0 ,0), Qt::AlignLeft|Qt::AlignVCenter, d.address);

        QRect r3(r2.right(), y, cell_goods_width, cell_height);
        painter.setPen(gridPen);
        painter.drawRect(r3);
        painter.setPen(textPen);
        painter.drawText(r3.adjusted(3, 0, 0 ,0), Qt::AlignLeft|Qt::AlignVCenter, d.good);

        QRect r4(r3.right(), y, cell_time_width, cell_height);
        painter.setPen(gridPen);
        painter.drawRect(r4);
        painter.setPen(textPen);
        painter.drawText(r4, Qt::AlignCenter, d.timestamp.date().toString(Qt::ISODate));

        QRect r5(r4.right(), y, cell_fin_width, cell_height);
        painter.setPen(gridPen);
        painter.drawRect(r5);
        painter.setPen(textPen);
        if (d.fin) {
            painter.drawText(r5, Qt::AlignCenter, "是");
        }
        else {
            painter.drawText(r5, Qt::AlignCenter, "否");
        }
    }
}

void DataEditor::resizeEvent(QResizeEvent *)
{
    resetBarValues();
}

void DataEditor::mousePressEvent(QMouseEvent *e)
{
    QPoint p = e->pos();
    int index = (p.y() + m_data->v_position) / cell_height;

    if (index >= m_data->selectedData.size()) {
        index = -1;
    }

    m_data->currentSelected = index;
    update();
}

void DataEditor::wheelEvent(QWheelEvent *e)
{
    if (e->delta() > 0) {
        emit mouseWheelValue(-50);
    }
    else {
        emit mouseWheelValue(50);
    }
}

void DataEditor::contextMenuEvent(QContextMenuEvent *e)
{
    if (m_data->currentSelected != -1) {
        m_data->contextMenu->move(e->globalPos());
        m_data->contextMenu->exec();
    }
}

void DataEditor::mouseDoubleClickEvent(QMouseEvent *)
{
    if (m_data->currentSelected != -1) {
        openModifyPopUp();
    }
}

void DataEditor::resetBarValues()
{
    int vvalue = m_data->selectedData.size() * cell_height - height();
    if (vvalue < 0) {
        vvalue = 0;
    }
    int hvalue = cell_name_width + cell_address_width + cell_goods_width + cell_time_width + cell_fin_width - width();
    if (hvalue < 0) {
        hvalue = 0;
    }
    emit barValueChanged(hvalue, vvalue);
}

void DataEditor::openModifyPopUp()
{
    ModifyPopUp pop;
    pop.setModal(true);
//    pop.resize(700, 300);
    BoughtData d = m_data->selectedData.at(m_data->currentSelected);
    pop.showPopUp(d, m_data->data.getAddresses(d.name), m_data->currentSelected);
    connect(&pop, SIGNAL(requestUpdate(BoughtData)), this, SLOT(requestUpdate(BoughtData)));
    connect(&pop, SIGNAL(requestRemoveAddress(QString, QString)), this, SLOT(requestRemoveAddress(QString, QString)));
    connect(&pop, SIGNAL(restoreCurrentSelected(int)), this, SLOT(restoreCurrentSelected(int)));
    pop.exec();
}
