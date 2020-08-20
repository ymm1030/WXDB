#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class PrivateMainWindow;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void adjustSize();

protected:
    void resizeEvent(QResizeEvent*) override;

private:
    PrivateMainWindow*      m_data;
};
#endif // MAINWINDOW_H
