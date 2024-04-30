#ifndef DREGISTERWIN_H
#define DREGISTERWIN_H

#include <QWidget>
#include <opencv.hpp>
#include <QSqlQuery>
#include <QSqlError>

namespace Ui {
class DregisterWin;
}

class DregisterWin : public QWidget
{
    Q_OBJECT

public:
    explicit DregisterWin(QWidget *parent = nullptr);
    ~DregisterWin();

    //定时器事件，开启关闭摄像头
    void timerEvent(QTimerEvent *event);

private slots:
    void on_resetBt_clicked();

    void on_addpicBt_clicked();

    void on_registerBt_clicked();

    void on_videoswitchBt_clicked();

    void on_cameraBt_clicked();

private:
    Ui::DregisterWin *ui;
    //摄像头对象//1.videoCapture可以对图像进行处理，进行基本操作？
    cv::VideoCapture cap;

    cv::Mat image;

    //摄像头id
    int timerid;

};

#endif // DREGISTERWIN_H
