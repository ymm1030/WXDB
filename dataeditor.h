#ifndef DATAEDITOR_H
#define DATAEDITOR_H

#include <QWidget>

class BoughtData;
class PrivateDataEditor;
class DataEditor : public QWidget
{
    Q_OBJECT
public:
    explicit DataEditor(QWidget *parent = nullptr);
    ~DataEditor();

    void refresh(const QString& nameFilter, const QString& addressFilter, const QString& goodsFilter,
                 int time_compare_method, const QString& time, int finStatus);
    void insert(const QString& name, const QString& address, const QString& good);

signals:
    void barValueChanged(int hvalue, int vvalue);
    void mouseWheelValue(int value);

public slots:
    void dataChanged();
    void hbarValueChanged(int value);
    void vbarValueChanged(int value);
    void menuTriggered(QAction* action);
    void requestUpdate(const BoughtData& data);
    void requestRemoveAddress(const QString& name, const QString& address);
    void restoreCurrentSelected(int currentSelected);

protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    void mousePressEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent* e) override;
    void contextMenuEvent(QContextMenuEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;

private:
    void resetBarValues();
    void openModifyPopUp();

private:
    PrivateDataEditor*      m_data;
};

#endif // DATAEDITOR_H
