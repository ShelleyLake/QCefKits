#include "widget.h"
#include "ui_widget.h"
#include <QCefWidget.h>
#include <QDebug>
#include <QTimer>
#include <QThread>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    m_browser = new QCefWidget(this);
    ui->verticalLayout->addWidget(m_browser, 10);
    m_browser->init(QUrl("https://github.com/panuins"));
    connectBrowserSignals();
}

Widget::Widget(QCefWidget *browser, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    m_browser = browser;
//    qDebug() << "Widget::Widget" << browser->parent();
    m_browser->setParent(this);
//    qDebug() << "Widget::Widget 1"
//                 << QThread::currentThread()
//             << m_browser->thread()
//             << this->thread();
//    m_browser->show();
    ui->verticalLayout->addWidget(m_browser, 10);
    connectBrowserSignals();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::closeEvent(QCloseEvent *event)
{
//    qDebug() << "Widget::closeEvent 1";
    m_browser->closeBrowser();
//    qDebug() << "Widget::closeEvent 2";
    QWidget::closeEvent(event);
//    qDebug() << "Widget::closeEvent 3";
}

void Widget::on_pushButton_clicked()
{
    m_browser->loadUrl(QUrl(ui->lineEdit->text()));
}

void Widget::on_lineEdit_returnPressed()
{
    on_pushButton_clicked();
}

void Widget::connectBrowserSignals()
{
    connect(m_browser, &QCefWidget::newBrowserCreated,
            this, [=](QCefWidget *newPage,
            int x,
            bool xSet,
            int y,
            bool ySet,
            int width,
            bool widthSet,
            int height,
            bool heightSet,
            bool menuBarVisible,
            bool statusBarVisible,
            bool toolBarVisible,
            bool scrollbarsVisible,
            bool isPopup)
    {
//        newPage->show();
//        qDebug() << "Widget::connectBrowserSignals newBrowserCreated" << newPage;
        Widget *w = new Widget(newPage, nullptr);
        w->show();
//        qDebug() << "Widget::connectBrowserSignals newBrowserCreated"
//                 << x << xSet << y << ySet << width << widthSet
//                 << height << heightSet << menuBarVisible
//                 << statusBarVisible << toolBarVisible
//                 << scrollbarsVisible << isPopup;
    });
    connect(m_browser, &QCefWidget::titleChanged,
            this, &Widget::setWindowTitle);
    connect(m_browser, &QCefWidget::urlChanged,
            this, [=](QUrl url){
        ui->lineEdit->setText(url.toString());
    });
}
