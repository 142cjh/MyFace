#include "dregisterwin.h"
#include "ui_dregisterwin.h"
#include <QPixmap>
#include <QFileDialog>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QMessageBox>
#include <QDebug>
#include "qfaceobject.h"

DregisterWin::DregisterWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DregisterWin)
{
    ui->setupUi(this);
}

DregisterWin::~DregisterWin()
{
    delete ui;
}

void DregisterWin::timerEvent(QTimerEvent *event)
{
    if(cap.isOpened())
    {
        /*
        //视频读取操作：视频帧读取到cv::Mat矩阵中，有两种方式：
        一种是read()操作
            cv::Mat frame;
        cap.read(frame);
        另一种是 “>>”操作。
            cv::Mat frame;
        cap >> frame;
        */
        cap.read(image);
        if(image.data==nullptr) return;
    }

    cv::Mat rgbImage;
    cv::cvtColor(image,rgbImage,cv::COLOR_BGR2RGB);
    QImage qImg(rgbImage.data,rgbImage.cols,rgbImage.rows,rgbImage.step1(),QImage::Format_RGB888);

    QPixmap mmp=QPixmap::fromImage(qImg);
    mmp=mmp.scaledToWidth(ui->headpicLb->width());
    ui->headpicLb->setPixmap(mmp);

}

void DregisterWin::on_resetBt_clicked()
{
    ui->nameEdit->clear();
    ui->birthdayEdit->setDate(QDate::currentDate());
    ui->addressEdit->clear();
    ui->phoneEdit->clear();
    ui->picFileEdit->clear();
}


void DregisterWin::on_addpicBt_clicked()
{
    QString filepath=QFileDialog::getOpenFileName(this);
    ui->picFileEdit->setText(filepath);

    QPixmap mmp(filepath);
    mmp=mmp.scaledToWidth(ui->headpicLb->width());
    ui->headpicLb->setPixmap(mmp);
}


void DregisterWin::on_registerBt_clicked()
{
    //读取路径的照片进行识别得到faceID
    QFaceObject faceobj;
    cv::Mat image=cv::imread(ui->picFileEdit->text().toUtf8().data());
    int faceID=faceobj.face_register(image);

    QString headfile=QString("D:/qtlsxm/MyFaceDetect/AttendanceServer/data/%1.jpg").arg(QString(ui->nameEdit->text().toUtf8().toBase64()));
    cv::imwrite(headfile.toUtf8().data(),image);

    //sqlite插入数据
    //create table if not exists employee(employeeID integer primary key autoincrement,name varchar(256),type varchar(256),sex varchar(32),"
    //"birthday text,address text,phone text,faceID integer unique,headfile text
    QString querystring=QString("insert into employee values(NULL,'%1','%2','%3','%4','%5','%6','%7','%8')")
                          .arg(ui->nameEdit->text())
                          .arg("管理员")
                          .arg((ui->mrb->isCheckable()?"男":"女"))
                          .arg(ui->birthdayEdit->text())
                          .arg(ui->addressEdit->text())
                          .arg(ui->phoneEdit->text())
                          .arg(faceID)
                          .arg(headfile);
    QSqlQuery query;
    if(query.exec(querystring))
    {
        QMessageBox::information(this,"注册提示","注册成功");
    }
    else
    {
        qDebug()<<query.lastError().text();
        QMessageBox::information(this,"注册提示","注册失败");
    }
}


void DregisterWin::on_videoswitchBt_clicked()
{
    if(ui->videoswitchBt->text()=="打开摄像头")
    {
        if(cap.open(0))
        {
            ui->videoswitchBt->setText("关闭摄像头");
            //启动定时器事件
            timerid=startTimer(100);
        }
    }
    else
    {
        //关闭定时器事件
        killTimer(timerid);
        ui->videoswitchBt->setText("打开摄像头");

        cap.release();
    }
}


void DregisterWin::on_cameraBt_clicked()
{
    //保存数据,将文件框中的路径保存成图片;
    QString headfile=QString("D:/qtlsxm/MyFaceDetect/AttendanceServer/data/%1.jpg")
                           .arg(QString(ui->nameEdit->text().toUtf8().toBase64()));
    ui->picFileEdit->setText(headfile);
    cv::imwrite(headfile.toUtf8().data(),image);

    killTimer(timerid);
    ui->videoswitchBt->setText("打开摄像头");

    cap.release();
}

