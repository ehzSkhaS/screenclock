#include <QtMath>
#include <QTimer>
#include <QTime>
#include <QProcess>

#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWidget::showFullScreen();

    QTimer * timer = new QTimer(this);
    connect(timer,
            &QTimer::timeout,
            this,
            &MainWindow::updateTime);
    timer->start(10);

    applyGeometry(this->geometry().width(), this->geometry().height());    
    updateTime();
}

MainWindow::~MainWindow()
{
    delete ui;
}

QLabel * MainWindow::getSec(int pos)
{
    if(pos < 0 or pos > 59)
        pos = 0;

    QLabel * s_label [] = {ui->s0,ui->s1,ui->s2,ui->s3,ui->s4,ui->s5,ui->s6,ui->s7,ui->s8,ui->s9,ui->s10,ui->s11,ui->s12,ui->s13,ui->s14,
                           ui->s15,ui->s16,ui->s17,ui->s18,ui->s19,ui->s20,ui->s21,ui->s22,ui->s23,ui->s24,ui->s25,ui->s26,ui->s27,ui->s28,ui->s29,
                           ui->s30,ui->s31,ui->s32,ui->s33,ui->s34,ui->s35,ui->s36,ui->s37,ui->s38,ui->s39,ui->s40,ui->s41,ui->s42,ui->s43,ui->s44,
                           ui->s45,ui->s46,ui->s47,ui->s48,ui->s49,ui->s50,ui->s51,ui->s52,ui->s53,ui->s54,ui->s55,ui->s56,ui->s57,ui->s58,ui->s59};
    return s_label[pos];
}

QLabel * MainWindow::getMin(int pos)
{
    if(pos < 0 or pos > 11)
        pos = 0;

    QLabel * m_label [] = {ui->m0,ui->m5,ui->m10,ui->m15,ui->m20,ui->m25,ui->m30,ui->m35,ui->m40,ui->m45,ui->m50,ui->m55};

    return m_label[pos];
}

void MainWindow::applyGeometry(int w, int h)
{
    double prop = w/h;
    double dot_size = prop * 24;
    //old
    //double r = w/4.5;

    double r = h/2 - dot_size*3;
    double a = w/2;
    double b = h/2;
    double s_rad = 6 * M_PI /180;

    //old
    //double l_w = w/2.9;
    //double l_h = h/2.9;

    double l_w = h - h/4;
    double l_h = h/prop - h/prop/4;

    ui->lcd_time->setGeometry(w/2-l_w/2,h/2-l_h/2,l_w,l_h);

    for(int i=0; i<60; i++)
        if(i>=15)
            getSec(i)->setGeometry(a + r * cos(s_rad * (i-15)) - 8, b + r * sin(s_rad * (i-15)) - 8, dot_size, dot_size);
        else
            getSec(i)->setGeometry(a + r * cos(s_rad * (i + 45)) - 8, b + r * sin(s_rad * (i + 45)) - 8, dot_size, dot_size);

    for(int i=0; i<12; i++)
        if(i>=3)
            getMin(i)->setGeometry(a + (r + dot_size*2) * cos(s_rad * (i-3) * 5) - dot_size/2, b + (r + dot_size*2) * sin(s_rad * (i-3) * 5) - dot_size/2, dot_size, dot_size);
        else
            getMin(i)->setGeometry(a + (r + dot_size*2) * cos(s_rad * (i+9) * 5) - dot_size/2, b + (r + dot_size*2) * sin(s_rad * (i+9) * 5) - dot_size/2, dot_size, dot_size);

    QTime currentTime = QTime::currentTime();
    /*
    for(int i = 11; i > (int)currentTime.minute()/5; i--)
        get_min(i)->hide();
    */

    for(int i = 59; i >= currentTime.second(); i--)
        if(i!=0)
            getSec(i)->hide();
}

void MainWindow::updateTime()
{
    QTime currentTime = QTime::currentTime();
    int hour = currentTime.hour();
    int minute = currentTime.minute();
    string h_s;    

    if(hour == 0)
        hour = 12;

    if(hour > 12)
    {
        hour -= 12;
        h_s = " " + to_string(hour);
    }else
        h_s = "0" + to_string(hour);

    string m_s;
    if(minute < 10)
        m_s = "0" + to_string(minute);
    else
        m_s = to_string(minute);

    QString currentTimeText = "";
    if(currentTime.msec() <= 500)
        currentTimeText = QString::fromStdString(h_s + ":" + m_s);
    else
        currentTimeText = QString::fromStdString(h_s + " " + m_s);

    int s = currentTime.second();
    if(s == 0)
        for(int i = 1; i < 60; i++)
            getSec(i)->hide();
    if (getSec(s)->isHidden())
        getSec(s)->show();

    /*
    int m = currentTime.minute();
    if(m == 0)
        for(int i = 1; i < 12; i++)
            get_min(i)->hide();
    int c_m = (int)m/5;
    if (get_min(c_m)->isHidden())
        get_min(c_m)->show();
    */
    ui->lcd_time->display(currentTimeText);
}
