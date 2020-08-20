#ifndef DATAAREA_H
#define DATAAREA_H

#include <QWidget>

class PrivateDataArea;
class DataArea : public QWidget
{
    Q_OBJECT
public:
    explicit DataArea(QWidget *parent = nullptr);
    ~DataArea();

signals:

public slots:
    void barValueChanged(int hvalue, int vvalue);
    void requestInsert(const QString& name, const QString& address, const QString& good);
    void requestRefresh(const QString& nameFilter, const QString& addressFilter, const QString& goodsFilter,
                        int time_compare_method, const QString& time, int finStatus);
    void mouseWheelValue(int value);

protected:
    void resizeEvent(QResizeEvent *) override;

private:
    PrivateDataArea*    m_data;
};

#endif // DATAAREA_H
