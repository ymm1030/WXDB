#ifndef ADDNEWPOPUP_H
#define ADDNEWPOPUP_H

#include <QDialog>

class PrivateAddNewPopUp;
class AddNewPopUp : public QDialog
{
    Q_OBJECT
public:
    explicit AddNewPopUp(QWidget *parent = nullptr);
    ~AddNewPopUp();

    void showPopup(const QString& info);

public slots:
    void okClicked();
    void cancelClicked();

signals:
    void addNewRecord(const QString& name, const QString& address, const QString& good);

private:
    PrivateAddNewPopUp*     m_data;
};

#endif // ADDNEWPOPUP_H
