#include "facedata.h"
#include "ui_facedata.h"

faceData::faceData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::faceData)
{
    ui->setupUi(this);
}

faceData::~faceData()
{
    delete ui;
}

int faceData::faceAge()
{
    seeta::ModelSetting setting;

}

//seeta::AgePredictor *new_ap()
//{
//    seeta::ModelSetting setting;
//    setting.append("age_predictor.csta");
//    return new seeta::AgePredictor(setting);
//}

//void faceData::plot_age(seeta::AgePredictor *ap, const SeetaImageData &image, const std::vector<SeetaPointF> &points)
//{
//    assert(points.size() == 5);
//    int age = 0;
//    ap->PredictAgeWithCrop(image, points.data(), age);
//}
