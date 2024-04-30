#include "maskdetector.h"
#include "ui_maskdetector.h"

maskDetector::maskDetector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::maskDetector)
{
    ui->setupUi(this);
}

maskDetector::~maskDetector()
{
    delete ui;
}
