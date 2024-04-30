#include "faceattendance.h"
#include <QSqlError>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //连接数据库
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
    //设置数据名称"D:\qtlsxm\MyFaceDetect\build-AttendanceServer-Desktop_Qt_5_15_2_MinGW_64_bit-Debug\face.db"
    db.setDatabaseName("D:/qtlsxm/MyFaceDetect/build-AttendanceServer-Desktop_Qt_5_15_2_MinGW_64_bit-Debug/server.db");
    //打开数据库
    if(db.open())
    {
        qDebug()<<"数据库打开成功";
    }
    else
    {
        qDebug()<<"无法打开数据库表:"<<db.lastError().text();
    }

    FaceAttendance w;
    w.show();
    return a.exec();
}
