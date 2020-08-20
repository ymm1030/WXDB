#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QtDebug>
#include "addnewpopup.h"

class PrivateAddNewPopUp
{
public:
    QLabel*     descLabel;
    QLabel*     nameLabel;
    QLineEdit*  nameInput;
    QLabel*     addressLabel;
    QLineEdit*  addressInput;
    QLabel*     goodsLabel;
    QLineEdit*  goodsInput;
    QPushButton*    okBtn;
    QPushButton*    cancelBtn;
};

AddNewPopUp::AddNewPopUp(QWidget *parent)
    : QDialog(parent)
    , m_data(new PrivateAddNewPopUp)
{
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addStretch();

    m_data->descLabel = new QLabel(this);
    m_data->descLabel->setWordWrap(true);
    m_data->descLabel->setText("内容已经自动分析为以下部分，如果不对，手动调整一下，按下确定完成添加。");
    layout->addWidget(m_data->descLabel);

    m_data->nameLabel = new QLabel(this);
    m_data->nameLabel->setText("ID");
    m_data->nameInput = new QLineEdit(this);
    QHBoxLayout* nameLayout = new QHBoxLayout;
    nameLayout->addWidget(m_data->nameLabel);
    nameLayout->addWidget(m_data->nameInput);

    m_data->addressLabel = new QLabel(this);
    m_data->addressLabel->setText("地址");
    m_data->addressInput = new QLineEdit(this);
    QHBoxLayout* addressLayout = new QHBoxLayout;
    addressLayout->addWidget(m_data->addressLabel);
    addressLayout->addWidget(m_data->addressInput);

    m_data->goodsLabel = new QLabel(this);
    m_data->goodsLabel->setText("货品");
    m_data->goodsInput = new QLineEdit(this);
    QHBoxLayout* goodsLayout = new QHBoxLayout;
    goodsLayout->addWidget(m_data->goodsLabel);
    goodsLayout->addWidget(m_data->goodsInput);

    m_data->okBtn = new QPushButton(this);
    m_data->okBtn->setText("确定");
    m_data->cancelBtn = new QPushButton(this);
    m_data->cancelBtn->setText("取消");
    QHBoxLayout* controlLayout = new QHBoxLayout;
    controlLayout->addWidget(m_data->okBtn);
    controlLayout->addWidget(m_data->cancelBtn);

    layout->addLayout(nameLayout);
    layout->addLayout(addressLayout);
    layout->addLayout(goodsLayout);
    layout->addLayout(controlLayout);
    layout->addStretch();
    setLayout(layout);

    connect(m_data->okBtn, SIGNAL(clicked()), this, SLOT(okClicked()));
    connect(m_data->cancelBtn, SIGNAL(clicked()), this, SLOT(cancelClicked()));
}

AddNewPopUp::~AddNewPopUp()
{
    delete m_data;
}

void AddNewPopUp::showPopup(const QString &info)
{
    QString str = info;
    str.replace('\r', ' ');
    str.replace('\n', ' ');

    QStringList list = str.split(' ');
    list.removeAll("");

    m_data->nameInput->setText(list.at(0));

    if (list.size() > 1) {
        m_data->goodsInput->setText(list.back());
    }

    QString address;
    for (int i = 1; i < list.size()-1; ++i) {
        address += list.at(i) + " ";
    }
    m_data->addressInput->setText(address.trimmed());
}

void AddNewPopUp::okClicked()
{
    QString name = m_data->nameInput->text().trimmed();
    QString address = m_data->addressInput->text().trimmed();
    QString good = m_data->goodsInput->text().trimmed();

    if (name.isEmpty() || address.isEmpty() || good.isEmpty()) {
        QMessageBox::warning(nullptr, "错误", "请填满所有字段。");
        return;
    }

    emit addNewRecord(name, address, good);
    close();
}

void AddNewPopUp::cancelClicked()
{
    close();
}
