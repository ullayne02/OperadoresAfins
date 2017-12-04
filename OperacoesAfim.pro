#-------------------------------------------------
#
# Project created by QtCreator 2017-06-08T14:08:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OperacoesAfim
TEMPLATE = app
unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
}
INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include/opencv2

LIBS += -L/usr/local/lib\
     -lopencv_calib3d\
     -lopencv_contrib\
     -lopencv_core\
     -lopencv_features2d \
     -lopencv_flann\
     -lopencv_gpu\
     -lopencv_highgui\
     -lopencv_imgproc\
     -lopencv_legacy\
     -lopencv_ml\
     -lopencv_objdetect\
     -lopencv_ocl\
     -lopencv_photo\
     -lopencv_stitching\
     -lopencv_superres\
     -lopencv_ts\
     -lopencv_video\
     -lopencv_videostab

SOURCES += main.cpp\
        mainwindow.cpp \
    my_qlabel.cpp

HEADERS  += mainwindow.h \
    my_qlabel.h

FORMS    += mainwindow.ui
