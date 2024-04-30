#include "faceattendance.h"
#include "ui_faceattendance.h"

FaceAttendance::FaceAttendance(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FaceAttendance)
{
    ui->setupUi(this);

    s=new selectwin;

    //打开摄像头
    cap.open(0);
    //启动定时器事件
    startTimer(100);

    //导入级联分类器文件
    cascade.load("D:/opencv4-5-2/etc/haarcascades/haarcascade_frontalface_alt2.xml");

    //QTcpSocket当断开连接的时候disconnted信号，连接成功发送connected
    connect(&msocket,&QTcpSocket::disconnected,this,&FaceAttendance::start_connect);
    connect(&msocket,&QTcpSocket::connected,this,&FaceAttendance::stop_connect);
    //关联接受数据的槽函数
    connect(&msocket,&QTcpSocket::readyRead,this,&FaceAttendance::rect_data);

    //定时连接服务器
    connect(&mtimer,&QTimer::timeout,this,&FaceAttendance::timer_connect);
    //启动定时器
    mtimer.start(5000);//每5s钟启动一次连接，直到成功就不再连接

    flag=0;

    ui->widgetLb->hide();
}

FaceAttendance::~FaceAttendance()
{
    delete ui;
}

void FaceAttendance::timerEvent(QTimerEvent *e)
{
    //采集数据
    Mat srcImage;
    //VideoCapture.grab()用来指向下一帧
    if(cap.grab())
    {
        cap.read(srcImage);//读取一帧数据

    }

    //把图片大小设置与显示窗口一样大
    cv::resize(srcImage,srcImage,Size(480,480));

    //检测前转换为灰度图像，加快检测速度
    Mat grayImage;
    cvtColor(srcImage,grayImage,COLOR_BGR2GRAY);
    //检测人脸数据
    std::vector<Rect> faceRects;
    cascade.detectMultiScale(grayImage,faceRects);
    if(faceRects.size()>0 && flag>=0)
    {
        Rect rect=faceRects.at(0);//取第一个人脸的矩形框
        //rectangle(srcImage,rect,Scalar(0,0,255));//画出人脸矩形框

        //移动人脸框（图片--QLabel）
        ui->headpicLb->move(rect.x,rect.y);

        if(flag>2)
        {
            //把Mat数据转化为QbyteArray，--》编码成jpg格式（压缩过的）
            std::vector<uchar> buf;
            cv::imencode(".jpg",srcImage,buf);
            QByteArray byte((const char*)buf.data(),buf.size());
            //准备发送
            quint64 backsize=byte.size();
            QByteArray sendData;
            QDataStream stream(&sendData,QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Qt_5_15);
            stream<<backsize<<byte;//先发送大小，再发送数据
            //发送
            msocket.write(sendData);
            flag=-2;

            //保存人脸数据
            faceMat=srcImage(rect);
            imwrite("D:/qtlsxm/MyFaceDetect/FaceAttendance/face.jpg",faceMat);
        }
        flag++;
    }
    if(faceRects.size()==0)
    {
        //把人脸框移动到中心位置
        ui->headpicLb->move(100,60);
        ui->widgetLb->hide();
        flag=0;
    }

    if(srcImage.data == nullptr) return;
    //把opencv里的Mat格式数据（BGR）转化成Qt中的QImage（RGB）
    cvtColor(srcImage,srcImage,COLOR_BGR2RGB);
    QImage image(srcImage.data,srcImage.cols,srcImage.rows,srcImage.step1(),QImage::Format_RGB888);
    QPixmap mmp=QPixmap::fromImage(image);
    ui->videoLb->setPixmap(mmp);

}

void FaceAttendance::rect_data()
{
    //{employeeID:%1,name:%2,type:%3,time:%4}
    QByteArray array=msocket.readAll();
    //解析Json数据
    QJsonParseError err;
    QJsonDocument doc=QJsonDocument::fromJson(array,&err);
    if(err.error!=QJsonParseError::NoError)
    {
        qDebug()<<"json数据错误";
        return;
    }

    QJsonObject obj=doc.object();
    QString employeeID=obj.value("employee").toString();
    QString name=obj.value("name").toString();
    QString type=obj.value("type").toString();
    QString timestr=obj.value("time").toString();

    //都为空
    qDebug()<<obj.value("employee");
    qDebug()<<type;
    qDebug()<<employeeID;
    qDebug()<<name;
    qDebug()<<timestr;

    //保存类型
    type1=type;

    ui->numberEdit->setText(employeeID);
    ui->nameEdit->setText(name);
    ui->typeEdit->setText(type);
    ui->timeEdit->setText(timestr);

    //通过样式来显示图片
    ui->headLb->setStyleSheet("border-radius:75px;border-image:url(D:/qtlsxm/MyFaceDetect/FaceAttendance/face.jpg);");

    if(type==QString("管理员"))
    {
        ui->label_2->setText("管理员认证");
        ui->widgetLb->show();
    }
    else if(type==QString("员工"))
    {
        ui->label_2->setText("员工认证");
        ui->widgetLb->show();
    }
}

void FaceAttendance::timer_connect()
{
    //连接服务器
    msocket.connectToHost("192.168.43.195",9999);
    qDebug()<<"正在连接服务器";
}

void FaceAttendance::stop_connect()
{
    mtimer.stop();
    qDebug()<<"成功连接服务器";
}

void FaceAttendance::start_connect()
{
    mtimer.start(5000);//启动定时器
    qDebug()<<"断开连接";
}


void FaceAttendance::on_kqcxBt_clicked()
{
    //考勤按钮查询
    qDebug()<<type1;
    if(type1=="管理员")
    {
        s->show();
    }
    else if(type1=="员工")
    {
        QMessageBox::information(this,"身份错误","权限不足");
    }
    else
    {
        QMessageBox::information(this,"身份错误","未注册");
    }
}

