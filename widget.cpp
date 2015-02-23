#include "widget.h"
#include "ui_UI.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses())
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost) && address.toString().contains("140.118"))
            userIP = address.toString();
    }
    view = new QWebView(parent);
    view->load(QUrl("http://dotnet.cc.ntust.edu.tw/dormweb/flowquery.aspx"));
    connect(view, SIGNAL(loadFinished(bool)), this, SLOT(update()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    timer.start(300000);
    resultInt = 0;
    ui->IP->setText("IP : " + userIP);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::update()
{
    QString cmd("ipdata.value = \"" + userIP + "\";document.forms[\"Form1\"].submit()");
    view->page()->mainFrame()->evaluateJavaScript(cmd);
    QVariant result = view->page()->mainFrame()->evaluateJavaScript("tablelist__ctl3_tid1.value;");
    QString resultStr = result.toString();

    // Damn, dealing with these strings can kill me.
    if(resultStr.split(",").length() == 4)
        resultInt = resultStr.split(",")[0].toInt() * 1000000000 + resultStr.split(",")[1].toInt() * 1000000 + resultStr.split(",")[2].toInt() * 1000 + resultStr.split(",")[3].toInt();
    else if(resultStr.split(",").length() == 3)
        resultInt = resultStr.split(",")[0].toInt() * 1000000 + resultStr.split(",")[1].toInt() * 1000 + resultStr.split(",")[2].toInt();
    else if(resultStr.split(",").length() == 2)
        resultInt = resultStr.split(",")[0].toInt() * 1000 + resultStr.split(",")[1].toInt();
    else if(resultStr.split(",").length() == 1)
        resultInt = resultStr.split(",")[0].toInt();
    qDebug() << resultStr;
    if(resultInt != 0)
    {
        //Speed up!
        disconnect(view, SIGNAL(loadFinished(bool)), this, SLOT(update()));
    }
    ui->progressBar->setValue(resultInt / (ui->lineEdit->text().toInt() * 10000.0));
    ui->label->setText("Today : " + QString::number(resultInt/1000000) + "MB");
    //qDebug() << resultInt / (ui->lineEdit->text().toInt() * 10000);
}

void Widget::on_pushButton_clicked()
{
    this->update();
}

void Widget::on_lineEdit_returnPressed()
{
    this->update();
}
