#ifndef MASKDETECTOR_H
#define MASKDETECTOR_H

#include <QWidget>
#include <seeta/FaceRecognizer.h>

namespace Ui {
class maskDetector;
}

class maskDetector : public QWidget
{
    Q_OBJECT

public:
    explicit maskDetector(QWidget *parent = nullptr);
    ~maskDetector();

private:
    Ui::maskDetector *ui;
};

#endif // MASKDETECTOR_H
