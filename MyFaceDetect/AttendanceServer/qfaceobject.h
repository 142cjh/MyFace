#ifndef QFACEOBJECT_H
#define QFACEOBJECT_H

#include <QObject>
#include <FaceEngine.h>
#include <opencv.hpp>


//人脸数据存储，人脸检测，人脸识别
class QFaceObject : public QObject
{
    Q_OBJECT
public:
    explicit QFaceObject(QObject *parent = nullptr);
    ~QFaceObject();

public slots:
    //人脸注册
    int64_t face_register(cv::Mat& faceImage);
    //人脸查询
    int face_query(cv::Mat& faceimage);

signals:
    //查询到人脸id时触发的信号
    void send_faceid(int64_t faceid);
private:
    seeta::FaceEngine *fengineptr;
};

#endif // QFACEOBJECT_H
