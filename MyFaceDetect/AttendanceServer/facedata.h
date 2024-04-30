#ifndef FACEDATA_H
#define FACEDATA_H

#include <QWidget>
#include <seeta/AgePredictor.h>

namespace Ui {
class faceData;
}

class faceData : public QWidget
{
    Q_OBJECT

public:
    explicit faceData(QWidget *parent = nullptr);
    ~faceData();

    //人脸年龄识别
    int faceAge();
    seeta::AgePredictor *new_ap();
    void plot_age(seeta::AgePredictor *ap,
                  const SeetaImageData &image,
                  const std::vector<SeetaPointF> &points);

private:
    Ui::faceData *ui;
};

#endif // FACEDATA_H
