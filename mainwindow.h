#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QPoint>

using namespace cv;

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  double* multiplication (double* a, double** b, int size);
  Mat translation (double x, double y, Size size);
  Mat shearing (double x, double y, Size size);
  Mat scale (double x, double y, Size size);
  Mat rotation (double x, double y, double ang, Size size);
  Mat reflection (double x1, double x2, double y1, double y2, double ang, Size size);
  Mat convert_im (Mat image);
  QImage convert_mat2qimage (Mat image);

  Mat image;
  Mat final_image;
  QPoint point1;
  QPoint point2;

private slots:
  void on_apButton_clicked();
  void on_imgButton_clicked();
  void Mouse_Pressed();

private:
  Ui::MainWindow *ui;
  int count = 1;
  double ang_ref = 180.0;
};

#endif // MAINWINDOW_H
