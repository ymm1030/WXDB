#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include "modifypopup.h"
#include "basedefine.h"

class PrivateModifyPopUp
{
public:
    QLabel*         nameLabel;
    QLineEdit*      nameInput;
    QLabel*         addressLabel;
    QComboBox*      addressInput;
    QPushButton*    removeAddressBtn;
    QLabel*         goodsLabel;
    QLineEdit*      goodsInput;
    QLabel*         finLabel;
    QComboBox*      finCombo;

    QPushButton*    okBtn;
    QPushButton*    cancelBtn;

    int             currentSelected;
};

ModifyPopUp::ModifyPopUp(QWidget* parent)
    : QDialog(parent)
    , m_data(new PrivateModifyPopUp)
{
    m_data->currentSelected = -1;
    QVBoxLayout* layout = new QVBoxLayout;

    m_data->nameLabel = new QLabel(this);
    m_data->nameLabel->setText("ID");
    m_data->nameInput = new QLineEdit(this);
    m_data->nameInput->setReadOnly(true);
    QHBoxLayout* nameLayout = new QHBoxLayout;
    nameLayout->addWidget(m_data->nameLabel);
    nameLayout->addWidget(m_data->nameInput);

    m_data->addressLabel = new QLabel(this);
    m_data->addressLabel->setText("地址");
    m_data->addressInput = new QComboBox(this);
    m_data->addressInput->setEditable(true);
    m_data->removeAddressBtn = new QPushButton(this);
    m_data->removeAddressBtn->setText("删掉这个地址");
    QHBoxLayout* addressLayout = new QHBoxLayout;
    addressLayout->addWidget(m_data->addressLabel);
    addressLayout->addWidget(m_data->addressInput);
    addressLayout->addWidget(m_data->removeAddressBtn);
    addressLayout->setStretchFactor(m_data->addressLabel, 0);
    addressLayout->setStretchFactor(m_data->addressInput, 2);
    addressLayout->setStretchFactor(m_data->removeAddressBtn, 0);

    m_data->goodsLabel = new QLabel(this);
    m_data->goodsLabel->setText("货品");
    m_data->goodsInput = new QLineEdit(this);
    QHBoxLayout* goodsLayout = new QHBoxLayout;
    goodsLayout->addWidget(m_data->goodsLabel);
    goodsLayout->addWidget(m_data->goodsInput);

    m_data->finLabel = new QLabel(this);
    m_data->finLabel->setText("已发货");
    m_data->finCombo = new QComboBox(this);
    m_data->finCombo->addItem("是");
    m_data->finCombo->addItem("否");
    QHBoxLayout* finLayout = new QHBoxLayout;
    finLayout->addWidget(m_data->finLabel);
    finLayout->addWidget(m_data->finCombo);

    m_data->okBtn = new QPushButton(this);
    m_data->okBtn->setText("确定");
    m_data->cancelBtn = new QPushButton(this);
    m_data->cancelBtn->setText("取消");
    QHBoxLayout* lastLayout = new QHBoxLayout;
    lastLayout->addWidget(m_data->okBtn);
    lastLayout->addWidget(m_data->cancelBtn);

    layout->addLayout(nameLayout);
    layout->addLayout(addressLayout);
    layout->addLayout(goodsLayout);
    layout->addLayout(finLayout);
    layout->addLayout(lastLayout);
    setLayout(layout);

    connect(m_data->okBtn, SIGNAL(clicked()), this, SLOT(okClicked()));
    connect(m_data->cancelBtn, SIGNAL(clicked()), this, SLOT(cancelClicked()));
    connect(m_data->removeAddressBtn, SIGNAL(clicked()), this, SLOT(removeAddressClicked()));
}

ModifyPopUp::~ModifyPopUp()
{
    delete m_data;
}

void ModifyPopUp::showPopUp(const BoughtData &data, const QStringList& addresses, int currentSelected)
{
    m_data->currentSelected = currentSelected;
    m_data->nameInput->setText(data.name);
    m_data->addressInput->addItem(data.address);
    for (int i = 0 ; i < addresses.size(); ++i) {
        if (addresses.at(i) != data.address) {
            m_data->addressInput->addItem(addresses.at(i));
        }
    }

    m_data->goodsInput->setText(data.good);
    if (data.fin) {
        m_data->finCombo->setCurrentIndex(0);
    }
    else {
        m_data->finCombo->setCurrentIndex(1);
    }

    m_data->cancelBtn->setFocus();
}

void ModifyPopUp::okClicked()
{
    updateData();
    close();
}

void ModifyPopUp::cancelClicked()
{
    close();
}

void ModifyPopUp::removeAddressClicked()
{
    if (m_data->addressInput->count() == 1) {
        QMessageBox::warning(nullptr, "错误", "唯一的地址不能删除！");
        return;
    }

    if (QMessageBox::No == QMessageBox::question(nullptr, "确认", "确定要删除这个地址吗？")) {
        return;
    }

    emit requestRemoveAddress(m_data->nameInput->text(), m_data->addressInput->lineEdit()->text());
    m_data->addressInput->removeItem(m_data->addressInput->currentIndex());
    updateData();
    emit restoreCurrentSelected(m_data->currentSelected);
}

void ModifyPopUp::updateData()
{
    BoughtData data;
    data.name = m_data->nameInput->text();
    data.address = m_data->addressInput->lineEdit()->text().trimmed();
    data.good = m_data->goodsInput->text().trimmed();
    if (m_data->finCombo->currentIndex() == 0) {
        data.fin = 1;
    }
    else {
        data.fin = 0;
    }
    emit requestUpdate(data);
}
