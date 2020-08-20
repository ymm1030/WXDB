#include <QPalette>
#include <QTextEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QMessageBox>
#include "controlpanel.h"
#include "addnewpopup.h"
#include "basedefine.h"

class PrivateControlPanel
{
public:
    QLabel*         addNewdDesc;
    QTextEdit*      addNewInput;
    QPushButton*    addNewBtn;

    QLabel*         filterDesc;
    QLabel*         nameFilterLabel;
    QLineEdit*      nameFilterInput;
    QLabel*         addressFilterLabel;
    QLineEdit*      addressFilterInput;
    QLabel*         goodsFilterLabel;
    QLineEdit*      goodsFilterInput;
    QLabel*         timeFilterLabel;
    QComboBox*      timeFilterCombo;
    QDateEdit*      timeFilterInput;
    QLabel*         finFilterLabel;
    QComboBox*      finFilterCombo;
    QPushButton*    execFilterBtn;
    QPushButton*    clearFilterBtn;

    QPushButton*    exportPurchaseBtn;
};

ControlPanel::ControlPanel(QWidget *parent)
    : QWidget(parent)
    , m_data(new PrivateControlPanel)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    m_data->addNewdDesc = new QLabel(this);
    m_data->addNewdDesc->setWordWrap(true);
    m_data->addNewdDesc->setText("将发货内容贴到下面的输入框，然后点击【添加】，就可以添加一条记录。");
    m_data->addNewInput = new QTextEdit(this);
    m_data->addNewBtn = new QPushButton(this);
    m_data->addNewBtn->setText("添加");

    QVBoxLayout* addNewLayout = new QVBoxLayout;
    addNewLayout->addWidget(m_data->addNewdDesc);
    addNewLayout->addWidget(m_data->addNewInput);
    addNewLayout->addWidget(m_data->addNewBtn);

    QVBoxLayout* filterLayout = new QVBoxLayout;
    m_data->filterDesc = new QLabel(this);
    m_data->filterDesc->setWordWrap(true);
    m_data->filterDesc->setText("选择好筛选条件后，点击【检索】，结果会显示在左边列表。至少要设置一个条件。");
    filterLayout->addWidget(m_data->filterDesc);

    m_data->nameFilterLabel = new QLabel(this);
    m_data->nameFilterLabel->setText("ID");
    m_data->nameFilterInput = new QLineEdit(this);
    QHBoxLayout* nameFilterLayout = new QHBoxLayout;
    nameFilterLayout->addWidget(m_data->nameFilterLabel);
    nameFilterLayout->addWidget(m_data->nameFilterInput);
    filterLayout->addLayout(nameFilterLayout);

    m_data->addressFilterLabel = new QLabel(this);
    m_data->addressFilterLabel->setText("地址");
    m_data->addressFilterInput = new QLineEdit(this);
    QHBoxLayout* addressFilterLayout = new QHBoxLayout;
    addressFilterLayout->addWidget(m_data->addressFilterLabel);
    addressFilterLayout->addWidget(m_data->addressFilterInput);
    filterLayout->addLayout(addressFilterLayout);

    m_data->goodsFilterLabel = new QLabel(this);
    m_data->goodsFilterLabel->setText("货品");
    m_data->goodsFilterInput = new QLineEdit(this);
    QHBoxLayout* goodsFilterLayout = new QHBoxLayout;
    goodsFilterLayout->addWidget(m_data->goodsFilterLabel);
    goodsFilterLayout->addWidget(m_data->goodsFilterInput);
    filterLayout->addLayout(goodsFilterLayout);

    m_data->timeFilterLabel = new QLabel(this);
    m_data->timeFilterLabel->setText("日期");
    m_data->timeFilterCombo = new QComboBox(this);
    m_data->timeFilterCombo->addItem("不设置");
    m_data->timeFilterCombo->addItem("大于");
    m_data->timeFilterCombo->addItem("等于");
    m_data->timeFilterCombo->addItem("小于");
    m_data->timeFilterInput = new QDateEdit(this);
    m_data->timeFilterInput->setCalendarPopup(true);
    m_data->timeFilterInput->setDate(QDate::currentDate());
    QHBoxLayout* timeFilterLayout = new QHBoxLayout;
    timeFilterLayout->addWidget(m_data->timeFilterLabel);
    timeFilterLayout->addWidget(m_data->timeFilterCombo);
    timeFilterLayout->addWidget(m_data->timeFilterInput);
    timeFilterLayout->addStretch();
    filterLayout->addLayout(timeFilterLayout);

    m_data->finFilterLabel = new QLabel(this);
    m_data->finFilterLabel->setText("已发货");
    m_data->finFilterCombo = new QComboBox(this);
    m_data->finFilterCombo->addItem("不设置");
    m_data->finFilterCombo->addItem("是");
    m_data->finFilterCombo->addItem("否");
    QHBoxLayout* finFilterLayout = new QHBoxLayout;
    finFilterLayout->addWidget(m_data->finFilterLabel);
    finFilterLayout->addWidget(m_data->finFilterCombo);
    finFilterLayout->addStretch();
    filterLayout->addLayout(finFilterLayout);

    m_data->execFilterBtn = new QPushButton(this);
    m_data->execFilterBtn->setText("检索");

    m_data->clearFilterBtn = new QPushButton(this);
    m_data->clearFilterBtn->setText("清除检索条件");

    m_data->exportPurchaseBtn = new QPushButton(this);
    m_data->exportPurchaseBtn->setText("出采购单");

    layout->addLayout(addNewLayout);
    layout->addStretch();
    layout->addLayout(filterLayout);
    layout->addWidget(m_data->execFilterBtn);
    layout->addWidget(m_data->clearFilterBtn);
    layout->addStretch();
    layout->addWidget(m_data->exportPurchaseBtn);
    layout->addStretch();
    setLayout(layout);

    connect(m_data->addNewBtn, SIGNAL(clicked()), this, SLOT(addNewClicked()));
    connect(m_data->execFilterBtn, SIGNAL(clicked()), this, SLOT(startFilter()));
    connect(m_data->clearFilterBtn, SIGNAL(clicked()), this, SLOT(clearFilter()));
}

ControlPanel::~ControlPanel()
{
    delete m_data;
}

void ControlPanel::addNewClicked()
{
    QString text = m_data->addNewInput->toPlainText();
    if (text.isEmpty()) {
        QMessageBox::warning(nullptr, "错误", "请先粘贴内容到输入框。");
        return;
    }

    AddNewPopUp pop(this);
    pop.resize(700, 300);
    pop.setModal(true);
    pop.showPopup(text);
    connect(&pop, SIGNAL(addNewRecord(QString, QString, QString)), this, SLOT(addNewRecord(QString, QString, QString)));
    pop.exec();
}

void ControlPanel::addNewRecord(const QString& name, const QString& address, const QString& good)
{
    m_data->addNewInput->clear();
    emit requestInsert(name, address, good);
}

void ControlPanel::startFilter()
{
    QString nameFilter = m_data->nameFilterInput->text().trimmed();
    QString addressFilter = m_data->addressFilterInput->text().trimmed();
    QString goodFilter = m_data->goodsFilterInput->text().trimmed();
    QString timeFilter = m_data->timeFilterInput->date().toString(Qt::ISODate);
    int timeFilterMethod = TIME_COMPARE_METHOD_NOTCARE;
    if (m_data->timeFilterCombo->currentIndex() != 0) {
        switch (m_data->timeFilterCombo->currentIndex()) {
        case 1:
            timeFilterMethod = TIME_COMPARE_METHOD_GREATER;
            break;
        case 2:
            timeFilterMethod = TIME_COMPARE_METHOD_EQUAL;
            break;
        case 3:
            timeFilterMethod = TIME_COMPARE_METHOD_LESS;
            break;
        default:
            break;
        }
    }

    int finStatus = FIN_STATUS_NOTCARE;
    if (m_data->finFilterCombo->currentIndex() == 1) {
        finStatus = FIN_STATUS_YES;
    }
    else if (m_data->finFilterCombo->currentIndex() == 2) {
        finStatus = FIN_STATUS_NO;
    }

    if (nameFilter.isEmpty() && addressFilter.isEmpty() && goodFilter.isEmpty()
        && timeFilterMethod == 0 && finStatus == 0) {
        QMessageBox::warning(nullptr, "错误", "至少要设置一个条件！");
        return;
    }

    emit requestRefresh(nameFilter, addressFilter, goodFilter,
                        timeFilterMethod, timeFilter, finStatus);
}

void ControlPanel::clearFilter()
{
    m_data->nameFilterInput->clear();
    m_data->addressFilterInput->clear();
    m_data->goodsFilterInput->clear();
    m_data->timeFilterCombo->setCurrentIndex(0);
    m_data->timeFilterInput->setDate(QDate::currentDate());
    m_data->finFilterCombo->setCurrentIndex(0);
}
