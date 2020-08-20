#ifndef MODIFYPOPUP_H
#define MODIFYPOPUP_H

#include <QDialog>

class BoughtData;
class PrivateModifyPopUp;
class ModifyPopUp : public QDialog
{
    Q_OBJECT
public:
    ModifyPopUp(QWidget* parent = nullptr);
    ~ModifyPopUp();

    void showPopUp(const BoughtData& data, const QStringList& addresses, int currentSelected);

signals:
    void requestUpdate(const BoughtData& data);
    void requestRemoveAddress(const QString& name, const QString& address);
    void restoreCurrentSelected(int currentSelected);

public slots:
    void okClicked();
    void cancelClicked();
    void removeAddressClicked();

private:
    void updateData();

private:
    PrivateModifyPopUp*     m_data;
};

#endif // MODIFYPOPUP_H
