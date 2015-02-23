#ifndef WIDGET_H
#define WIDGET_H

#include <QWebView>
#include <QWebFrame>
#include <QWebElement>
#include <QNetworkInterface>
#include <QDebug>
#include <QTimer>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void update();
private slots:
    void on_pushButton_clicked();

    void on_lineEdit_returnPressed();

private:
    Ui::Widget *ui;
    QWebView* view;
    QString userIP;
    QTimer timer;
    unsigned int resultInt;
};

#endif // WIDGET_H
