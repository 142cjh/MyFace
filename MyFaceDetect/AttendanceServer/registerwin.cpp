#include "registerwin.h"
#include "ui_registerwin.h"
#include <QFileDialog>
#include "qfaceobject.h"
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QMessageBox>
#include <QDebug>

RegisterWin::RegisterWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegisterWin)
{
    ui->setupUi(this);
}

RegisterWin::~RegisterWin()
{
    delete ui;
}

void RegisterWin::timerEvent(QTimerEvent *event)
{
    //获取摄像头数据，并且显示到界面上
    if(cap.isOpened())
    {
        cap>>image;
        if(image.data==nullptr)return;
    }
    //Mat->QImage
    cv::Mat rgbImage;
    cv::cvtColor(image,rgbImage,cv::COLOR_BGR2RGB);
    QImage qImg(rgbImage.data,rgbImage.cols,rgbImage.rows,rgbImage.step1(),QImage::Format_RGB888);

    //在qt界面上显示
    QPixmap mmp=QPixmap::fromImage(qImg);
    mmp=mmp.scaledToWidth(ui->headpicLb->width());
    ui->headpicLb->setPixmap(mmp);
}

//清空数据
void RegisterWin::on_resetBt_clicked()
{
    ui->nameEdit->clear();
    ui->birthdayEdit->setDate(QDate::currentDate());
    ui->addressEdit->clear();
    ui->phoneEdit->clear();
    ui->picFileEdit->clear();
}

//添加图片
void RegisterWin::on_addpicBt_clicked()
{
    //通过文件对话框选择图片路径
    QString filepath=QFileDialog::getOpenFileName(this);
    ui->picFileEdit->setText(filepath);

    //显示图片
    QPixmap mmp(filepath);
    mmp=mmp.scaledToWidth(ui->headpicLb->width());
    ui->headpicLb->setPixmap(mmp);
}


void RegisterWin::on_registerBt_clicked()
{
     //1.通过照片，结合faceObject模块得到faceID
    QFaceObject faceobj;
    cv::Mat image=cv::imread(ui->picFileEdit->text().toUtf8().data());
    int faceID=faceobj.face_register(image);
    qDebug()<<faceID;

    //把头像保存到一个固定路径下
    QString headfile=QString("D:/qtlsxm/MyFaceDetect/AttendanceServer/data/%1.jpg").arg(QString(ui->nameEdit->text().toUtf8().toBase64()));
    cv::imwrite(headfile.toUtf8().data(),image);

    //sqlite插入数据
    //create table if not exists employee(employeeID integer primary key autoincrement,name varchar(256),type varchar(256),sex varchar(32),"
    //"birthday text,address text,phone text,faceID integer unique,headfile text
    QString querystring=QString("insert into employee values(NULL,'%1','%2','%3','%4','%5','%6','%7','%8')")
                          .arg(ui->nameEdit->text())
                          .arg("员工")
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
    /*
    //2.把个人信息存储到数据库employee
    QSqlTableModel model;
    model.setTable("employee");//设置表名,获得当前表的信息
    //record返回空记录，用于查询记录的字段名称
    QSqlRecord record=model.record();
    //设置数据,人脸数据facecID唯一不能相同，否则无法插入提示注册失败
    record.setValue("name",ui->nameEdit->text());

    record.setValue("type","员工");

    record.setValue("sex",ui->mrb->isChecked()?"男":"女");
    record.setValue("birthday",ui->birthdayEdit->text());
    record.setValue("address",ui->addressEdit->text());
    record.setValue("phone",ui->phoneEdit->text());
    record.setValue("faceID",faceID);
    //头像路径
    record.setValue("headfile",headfile);
    //把记录插入到数据库表格中
    bool ret=model.insertRecord(-1,record);

    //3.提示注册成功
    if(ret)
    {
        QMessageBox::information(this,"注册提示","注册成功");
        //提交
        model.submitAll();
    }
    else
    {
        QMessageBox::information(this,"注册提示","注册失败");
    }
    */
}


void RegisterWin::on_videoswitchBt_clicked()
{
    if(ui->videoswitchBt->text()=="打开摄像头")
    {
        //打开摄像头
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
        //关闭摄像头
        cap.release();
    }
}


void RegisterWin::on_cameraBt_clicked()
{
    //保存数据
    QString headfile=QString("D:/qtlsxm/MyFaceDetect/AttendanceServer/data/%1.jpg").arg(QString(ui->nameEdit->text().toUtf8().toBase64()));
    ui->picFileEdit->setText(headfile);
    cv::imwrite(headfile.toUtf8().data(),image);

    //关闭定时器事件
    killTimer(timerid);
    ui->videoswitchBt->setText("打开摄像头");
    //关闭摄像头
    cap.release();
}

