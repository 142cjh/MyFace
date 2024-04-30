QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#windows平台opencv，seetaface环境
win32{
LIBS +=D:\opencv4-5-2\x64\mingw\lib\libopencv*
LIBS +=D:\SeetaFace\lib\libSeeta*
INCLUDEPATH +=D:\opencv4-5-2\include
INCLUDEPATH +=D:\opencv4-5-2\include\opencv2
INCLUDEPATH +=D:\SeetaFace\include
INCLUDEPATH +=D:\SeetaFace\include\seeta
}


SOURCES += \
    dregisterwin.cpp \
    facedata.cpp \
    main.cpp \
    attendancewin.cpp \
    maskdetector.cpp \
    qfaceobject.cpp \
    registerwin.cpp

HEADERS += \
    attendancewin.h \
    dregisterwin.h \
    facedata.h \
    maskdetector.h \
    qfaceobject.h \
    registerwin.h

FORMS += \
    attendancewin.ui \
    dregisterwin.ui \
    facedata.ui \
    maskdetector.ui \
    registerwin.ui



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
