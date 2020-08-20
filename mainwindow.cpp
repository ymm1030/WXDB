#include <QGuiApplication>
#include <QScreen>
#include <QSplitter>
#include "mainwindow.h"
#include "dataarea.h"
#include "controlpanel.h"

class PrivateMainWindow
{
public:
    DataArea*       dataArea;
    ControlPanel*   controlPanel;
    QSplitter*      splitter;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_data(new PrivateMainWindow)
{
    m_data->dataArea = new DataArea(this);
    m_data->controlPanel = new ControlPanel(this);

    m_data->splitter = new QSplitter(this);
    m_data->splitter->setOrientation(Qt::Horizontal);
    m_data->splitter->addWidget(m_data->dataArea);
    m_data->splitter->addWidget(m_data->controlPanel);

    adjustSize();

    QList<int> initialSizes;
    initialSizes << width() * 0.7;
    initialSizes << width() - initialSizes.front();
    m_data->splitter->setSizes(initialSizes);

    connect(m_data->controlPanel, SIGNAL(requestInsert(QString, QString, QString)), m_data->dataArea, SLOT(requestInsert(QString, QString, QString)));
    connect(m_data->controlPanel, SIGNAL(requestRefresh(QString, QString,QString, int, QString, int)),
            m_data->dataArea, SLOT(requestRefresh(QString, QString,QString, int, QString, int)));
}

MainWindow::~MainWindow()
{
    delete m_data;
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    m_data->splitter->setGeometry(rect());
}

void MainWindow::adjustSize()
{
    QRect r = QGuiApplication::screens().at(0)->geometry();
    QRect r1;

    if (r.width() >= 1600) {
        r1.setWidth(1600);
    }
    else {
        r1.setWidth(r.width() - 200);
    }

    if (r.height() >= 1000) {
        r1.setHeight(900);
    }
    else {
        r1.setHeight(r.height() - 100);
    }

    r1 = QRect((r.width()-r1.width())/2, (r.height()-r1.height())/2, r1.width(), r1.height());
    setGeometry(r1);
}

