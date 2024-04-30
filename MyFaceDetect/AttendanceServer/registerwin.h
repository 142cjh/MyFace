#ifndef REGISTERWIN_H
#define REGISTERWIN_H

#include <QWidget>
#include <opencv.hpp>
#include <QSqlQuery>
#include <QSqlError>

namespace Ui {
class RegisterWin;
}

class RegisterWin : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterWin(QWidget *parent = nullptr);
    ~RegisterWin();

    //定时器事件,开启关闭摄像头
    void timerEvent(QTimerEvent *event);

private slots:
    void on_resetBt_clicked();

    void on_addpicBt_clicked();

    void on_registerBt_clicked();

    void on_videoswitchBt_clicked();

    void on_cameraBt_clicked();

private:
    Ui::RegisterWin *ui;
    int timerid;
    cv::VideoCapture cap;
    cv::Mat image;
};

#endif // REGISTERWIN_H
