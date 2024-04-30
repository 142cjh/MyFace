#ifndef FACEATTENDANCE_H
#define FACEATTENDANCE_H

#include <QMainWindow>
#include <opencv.hpp>
#include <QTcpSocket>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QMessageBox>
#include "selectwin.h"

using namespace  cv;
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class FaceAttendance; }
QT_END_NAMESPACE

class FaceAttendance : public QMainWindow
{
    Q_OBJECT

public:
    FaceAttendance(QWidget *parent = nullptr);
    ~FaceAttendance();
    //定时器事件
    void timerEvent(QTimerEvent *e);

protected slots:
    void rect_data();
private slots:
    /*定时器定时连接服务器,断线重连服务器*/
    //定时连接服务器
    void timer_connect();
    //连接成功，断开连接
    void stop_connect();
    //连接失败，开始连接
    void start_connect();
    void on_kqcxBt_clicked();

private:
    Ui::FaceAttendance *ui;

    //摄像头
    VideoCapture cap;

    //haar--级联分类器
    cv::CascadeClassifier cascade;

    //创建网络套接字，定时器
    QTcpSocket msocket;
    QTimer mtimer;

    //标志是否是同一个人脸进入到识别区域
    int flag;

    //保存类型
    QString type1;

    //保存人脸的数据
    cv::Mat faceMat;

    selectwin *s;


};
#endif // FACEATTENDANCE_H
