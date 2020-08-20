#ifndef TABLETITLE_H
#define TABLETITLE_H

#include <QWidget>
#include "basedefine.h"

class PrivateTableTitle;
class TableTitle : public QWidget
{
    Q_OBJECT
public:
    explicit TableTitle(QWidget *parent = nullptr);
    ~TableTitle();

    void setColumnDetails(const QList<ColumnDetail>& details);

public slots:
    void hvalueChanged(int value);

protected:
    void paintEvent(QPaintEvent *) override;

private:
    PrivateTableTitle*      m_data;
};

#endif // TABLETITLE_H
