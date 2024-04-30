#include "attendancewin.h"
#include "ui_attendancewin.h"


AttendanceWin::AttendanceWin(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AttendanceWin)
{
    ui->setupUi(this);
    //qtcpServer当有客户端连接会发送newConnection
    connect(&mserver,&QTcpServer::newConnection,this,&AttendanceWin::accept_client);
    mserver.listen(QHostAddress::Any,9999);//监听，启动服务器

    bsize=0;

    //    //给sql模型绑定表格
    //    model.setTable("employee");

    //创建一个线程
    QThread *thread=new QThread();
    //把QFaceObject对象移动到thread线程中执行
    fobj.moveToThread(thread);
    //启动线程
    thread->start();

    connect(this,&AttendanceWin::query,&fobj,&QFaceObject::face_query);

    //关联QFaceObject对象里的send_faceid信号
    connect(&fobj,&QFaceObject::send_faceid,this,&AttendanceWin::recv_faceid);
}

AttendanceWin::~AttendanceWin()
{
    delete ui;
}

//接受客户端连接
void AttendanceWin::accept_client()
{
    //获取与客户端通信的套接字
    msocket=mserver.nextPendingConnection();

    //当客户端有数据到达，会发送readyRead信号
    connect(msocket,&QTcpSocket::readyRead,this,&AttendanceWin::read_data);
}

//读取客户端发送的数据
void AttendanceWin::read_data()
{
    //把套接字绑定到数据流
    QDataStream stream(msocket);
    stream.setVersion(QDataStream::Qt_5_15);

    //先接受到数据的大小
    if(bsize == 0)
    {
        //bytesAvailable返回当前已经获取的数据大小
        if(msocket->bytesAvailable()<(qint64)sizeof(bsize)) return;
        //采集数据的长度
        stream>>bsize;
    }

    if(msocket->bytesAvailable()<bsize)//说明数据还没有发送完全，返回继续等待
    {
        return;
    }

    QByteArray data;
    stream>>data;
    bsize=0;
    if(data.size()==0)//没有读取到数据
    {
        return;
    }

    //显示图片
    QPixmap mmp;
    mmp.loadFromData(data,"jpg");
    mmp=mmp.scaled(ui->picLb->size());
    ui->picLb->setPixmap(mmp);

    //识别人脸
    cv::Mat faceImage;
    std::vector<uchar> decode;
    decode.resize(data.size());
    memcpy(decode.data(),data.data(),data.size());
    faceImage=cv::imdecode(decode,cv::IMREAD_COLOR);

    //int faceid=fobj.face_query(faceImage);//消耗资源较多
    emit query(faceImage);


}

void AttendanceWin::recv_faceid(int64_t faceid)
{
    if(faceid<0)
    {
        QString sdmsg=QString("{\"employeeID\":\" \",\"name\":\" \",\"type\":\" \",\"time\":\" \"}");
        msocket->write(sdmsg.toUtf8());//把打包好的数据发送给客户端
        qDebug()<<"数据为空，未注册";
        return ;
    }
    QSqlQuery query;
    QString querystring=QString("select * from employee where faceID=%1")
                              .arg(faceid);

    if(!query.exec(querystring))
    {
        qDebug()<<"查询不到faceid对应的人脸数据";
    }
    QString employeeID;
    QString name;
    QString type;
    while(query.next())
    {
        employeeID=query.value("employeeID").toString();
        name=query.value("name").toString();
        type=query.value("type").toString();
    }
    QString dateTime=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    qDebug()<<employeeID;
    qDebug()<<name;
    qDebug()<<type;
    //工号，姓名，部门，时间
    QString sdmsg=QString("{\"employeeID\":\"%1\",\"name\":\"%2\",\"type\":\"%3\",\"time\":\"%4\"}")
                        .arg(employeeID)
                        .arg(name)
                        .arg(type)
                        .arg(dateTime);

    QString sdmsgerror=QString("{\"employeeID\":\"\",\"name\":\"\",\"type\":\"\",\"time\":\"\"}");



    //把数据写入数据库--考勤表
    //先查询数据库数据是否存在，再更新数据库的内容
    QSqlQuery queryn;
    QString personChecked=QString("select 1 from attendance where employeeID = '%1' limit 1;").arg(employeeID);
    qDebug()<<"queryn.exec(personChecked)"<<queryn.exec(personChecked);
    if(!queryn.exec(personChecked))
    {
        qDebug()<<"考勤数据表查询失败，或为空";
        qDebug()<<queryn.lastError().text();
    }
    else
    {
        int nIndex=queryn.record().indexOf("1");
        queryn.next();
        int nCount=queryn.value(nIndex).toInt();
        qDebug()<<"nCount"<<nCount;
        if(nCount==1)
        {
            //当前数据存在,进行数据库更新数据
            QString updateData=QString("update attendance set attendanceTime='%1' where employeeID = '%2'")
                                     .arg(dateTime).arg(employeeID);
            qDebug()<<updateData;
            if(!queryn.exec(updateData))
            {
                qDebug()<<"updateData next";
                msocket->write(sdmsgerror.toUtf8());//把打包好的数据发送给客户端
                qDebug()<<"考勤数据更新失败";
            }

        }
        else
        {
            //数据不存在，插入数据
            QString insertData=QString("insert into attendance(employeeID,name,type) values ('%1','%2','%3')")
                                     .arg(employeeID)
                                     .arg(name)
                                     .arg(type);
            qDebug()<<insertData;
            if(!queryn.exec(insertData))
            {
                msocket->write(sdmsgerror.toUtf8());//把打包好的数据发送给客户端
                qDebug()<<"考勤数据插入失败";
            }
        }

        msocket->write(sdmsg.toUtf8());//把打包好的数据发送给客户端
    }


}
