#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>

class PrivateControlPanel;
class ControlPanel : public QWidget
{
    Q_OBJECT
public:
    explicit ControlPanel(QWidget *parent = nullptr);
    ~ControlPanel();

public slots:
    void addNewClicked();
    void addNewRecord(const QString& name, const QString& address, const QString& good);
    void startFilter();
    void clearFilter();

signals:
    void requestInsert(const QString& name, const QString& address, const QString& good);
    void requestRefresh(const QString& nameFilter, const QString& addressFilter, const QString& goodsFilter,
                        int time_compare_method, const QString& time, int finStatus);

private:
    PrivateControlPanel*    m_data;
};

#endif // CONTROLPANEL_H
