#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#define tam_max 1161;
#define PI 3.14159265

using namespace std;
using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  ui->angLab_2->setVisible(false);
  ui->angSlid_2->setVisible(false);
  ui->xLab_2->setVisible(false);
  ui->xSlid_2->setVisible(false);
  ui->yLab_2->setVisible(false);
  ui->ySlid_2->setVisible(false);
  connect(ui->gridLab, SIGNAL(Mouse_Pressed()), this, SLOT(Mouse_Pressed()));
}

MainWindow::~MainWindow()
{
  delete ui;
}

double *MainWindow::multiplication(double *a, double **b, int size)
{
  double *c;
      c = (double*)malloc(size*sizeof(double));
      for(int i = 0; i<size; i++) c[i] = 0;
      for (int i = 0; i<size;i++){
        for(int j = 0; j<size; j++){
          c[i] = c[i] + b[i][j]*a[j];
        }
      }
      return c;
}


Mat MainWindow::translation(double x, double y, Size size)
{
  Mat aux;
  aux = final_image.clone();
  double **a;
  double *b;
  double *c;
  a = (double**)malloc(3*sizeof(double*));
  b = (double*)malloc(3*sizeof(double));
  c = (double*)malloc(3*sizeof(double));

  for (int i = 0; i<3; i++){
      a[i] = (double*)malloc(3*sizeof(double));
    }

  a[0][1] = a[1][0] = a[2][0] = a[2][1] = 0;
  a[2][2] = a[1][1] = a[0][0] = 1;
  a[0][2] = x;
  a[1][2] = y;

  for (int i = 0; i<size.height; i++){
      for(int j = 0; j<size.width; j++){
          b[0] = i;
          b[1] = j;
          b[2] = 1;
          c = multiplication(b, a, 3);
          if(c[0] <= 1120 && c[1] <= 750) aux.at<Vec3b>(c[1], c[0]) = final_image.at<Vec3b>(j,i);
        }
    }
  return aux;
}

Mat MainWindow::shearing(double x, double y, Size size)
{
  double **shearing;
  double *cord;
  double *res;
  Mat aux;
  aux = final_image.clone();

  shearing = (double**)malloc(3*sizeof(double*));
  for (int i = 0; i<3; i++){
      shearing[i] = (double*)malloc(3*sizeof(double));
    }
  cord = (double*)malloc(3*sizeof(double));
  res = (double*)malloc(3*sizeof(double));

  shearing[0][1] = x;
  shearing[1][0] = y;
  shearing[2][0] = shearing[2][1] = shearing[0][2] = shearing[1][2] = 0.0;
  shearing[2][2] = shearing[1][1] = shearing[0][0] = 1.0;

  for (double i = 0.0; i<size.height; i = i + 0.1){
      for(double j = 0.0; j<size.width; j = j + 0.1){
          cord[0] = i;
          cord[1] = j;
          cord[2] = 1.0;
          res = multiplication(cord, shearing, 3);
          if(res[0] < 1120 && res[1] < 750) {
              aux.at<Vec3b>(res[1], res[0]) = final_image.at<Vec3b>(j,i);
            }
        }
    }

  return aux;
  free(shearing);
  free(res);
  free(cord);
}

Mat MainWindow::scale(double x, double y, Size size)
{
  Mat aux;
    double **a;
    double *b;
    double *c;
    aux = final_image.clone();
    a = (double**)malloc(3*sizeof(double*));
    b = (double*)malloc(3*sizeof(double));
    c = (double*)malloc(3*sizeof(double));

    for (int i = 0; i<3; i++){
        a[i] = (double*)malloc(3*sizeof(double));
    }

    a[0][1] = a[0][2] = a[1][0] = a[1][2] = a[2][1] = a[2][0] = 0.0;
    a[0][0] = x;
    a[1][1] = y;
    a[2][2] = 1.0;

    for (double i = 0; i<size.height; i = i + 0.1){
        for(double j = 0; j<size.width; j = j + 0.1){
            b[0] = i;
            b[1] = j;
            b[2] = 1.0;
            c = multiplication(b, a, 3);
            if(c[0] < 1120 && c[1] < 750) aux.at<Vec3b>(c[1], c[0]) = final_image.at<Vec3b>(j,i);
        }
    }
    return aux;
}

Mat MainWindow::rotation(double x, double y, double ang, Size size)
{
  Mat aux;
  double cos_ang, sen_ang;
  cos_ang = cos (ang*PI/180.0);
  sen_ang = sin (ang*PI/180.0);
  double **rot;
  double **trans;
  double **inv_trans;
  double *cord;
  double *res;

  aux = final_image.clone();

  inv_trans = (double**)malloc(3*sizeof(double*));
  rot = (double**)malloc(3*sizeof(double*));
  trans = (double**)malloc(3*sizeof(double*));
  cord = (double*)malloc(3*sizeof(double));
  res = (double*)malloc(3*sizeof(double));

  for (int i = 0; i<3; i++){
      rot[i] = (double*)malloc(3*sizeof(double));
      trans[i] = (double*)malloc(3*sizeof(double));
      inv_trans[i] = (double*)malloc(3*sizeof(double));
    }
  //translacao para origem
  trans[0][1] = trans[1][0] = trans[2][0] = trans[2][1] = 0;
  trans[2][2] = trans[1][1] = trans[0][0] = 1;
  trans[0][2] = -x;
  trans[1][2] = -y;

  //rotacao horario
  rot[0][0] = rot[1][1] = cos_ang;
  rot[0][1] = -sen_ang;
  rot[1][0] = sen_ang;
  rot[0][2] = rot[1][2] = rot[2][0] = rot[2][1] = 0;
  rot[2][2] = 1;

  //translacao de volta para o ponto
  inv_trans[0][1] =inv_trans[1][0] = inv_trans[2][0] = inv_trans[2][1] = 0;
  inv_trans[2][2] = inv_trans[1][1] = inv_trans[0][0] = 1;
  inv_trans[0][2] = x;
  inv_trans[1][2] = y;


  for (double i = 0.0; i<size.height; i++){
      for(double j = 0.0; j<size.width; j++){
          cord[0] = i;
          cord[1] = j;
          cord[2] = 1.0;
          res = multiplication(cord, trans, 3);
          res = multiplication(res, rot, 3);
          res = multiplication(res, inv_trans, 3);
          if(res[0] < 1120 && res[1] < 750 && res[0] > -1 && res[1] > -1){
              aux.at<Vec3b>(res[1], res[0]) = final_image.at<Vec3b>(j,i);
            }

        }
    }

  return aux;
  free(cord);
  free(res);
  free(inv_trans);
  free(rot);
  free(trans);
}

Mat MainWindow::reflection (double x1, double x2, double y1, double y2, double ang_ref, Size size){
        //angulo da "reflexao", a reflexao e dividida em n partes
        double cos_ang_ref, sen_ang_ref;
        cos_ang_ref = cos (ang_ref*PI/180.0);
        sen_ang_ref = sin (ang_ref*PI/180.0);
        Mat aux;
        double **rot;
        double **trans;
        double **inv_rot;
        double **inv_trans;
        double **op;
        double **reflec;
        double* cord;
        double* res;
        double vetor_reta[2];
        double ang_comp, ang, cos_ang, sen_ang, coef_ang;

        cord = (double*)malloc(4*sizeof(double));
        res = (double*)malloc(4*sizeof(double));
        reflec = (double**)malloc(4*sizeof(double*));
        inv_rot = (double**)malloc(4*sizeof(double*));
        inv_trans = (double**)malloc(4*sizeof(double*));
        op = (double**)malloc(4*sizeof(double*));
        rot = (double**)malloc(4*sizeof(double*));
        trans = (double**)malloc(4*sizeof(double*));
        for (int i = 0; i<4; i++){
            rot[i] = (double*)malloc(4*sizeof(double));
            trans[i] = (double*)malloc(4*sizeof(double));
            inv_rot[i] = (double*)malloc(4*sizeof(double));
            inv_trans[i] = (double*)malloc(4*sizeof(double));
            op[i] = (double*)malloc(4*sizeof(double));
            reflec[i] = (double*)malloc(4*sizeof(double*));
        }

        aux = final_image.clone();
        //vetor da reta
        vetor_reta[0] = x2-x1;
        vetor_reta[1] = y2-y1;
        //ponto que sera transladado para a origem sera x1,y1

        //sera necessario pegar o angulo que a reta faz com o eixo OY
        coef_ang = vetor_reta[1]/vetor_reta[0];
    //se o coeficiente da reta for positivo, a rotacao sera horaria em relacao ao angulo que a reta faz com eixo das abcissas
        if(coef_ang >= 0){
            //se o coeficiente for zero, signfica que nao nao existe a tangente, loog o angulo e zero ou 180
            if(coef_ang > 0) ang = atan(coef_ang)*180.0/PI;
            else if (coef_ang == 0) ang = 0.0;

            cos_ang = cos (ang*PI/180.0);
            sen_ang = sin (ang*PI/180.0);
            rot[0][0] = rot[1][1] = cos_ang;
            rot[0][1] = -sen_ang;
            rot[1][0] = sen_ang;
            rot[0][2] = rot[1][2] = rot[2][0] = rot[2][1] = rot[0][3] = rot[2][3] = rot[3][0] = rot[1][3] = rot[3][2] = 0;
            rot[2][2] =  rot[3][3] = 1;

            inv_rot[0][0] = inv_rot[1][1] = cos_ang;
            inv_rot[0][1] = sen_ang;
            inv_rot[1][0] = -sen_ang;
            inv_rot[0][2] = inv_rot[1][2] = inv_rot[2][0] = inv_rot[2][1] = inv_rot[0][3] = inv_rot[2][3] = inv_rot[0][3] = inv_rot[1][3] = inv_rot[3][2] = 0;
            inv_rot[2][2] =  inv_rot[3][3] = 1;

    //se o coeficiente for negativo, entao a hotacao sera feita em relacao ao suplmento do angulo
        }else if (coef_ang < 0) {
            double ang_sup;
            ang = atan(coef_ang)*180.0/PI;
            ang_sup = 180.0 - ang;
            cos_ang = cos (ang_sup*PI/180.0);
            sen_ang = sin (ang_sup*PI/180.0);
            rot[0][0] = rot[1][1] = cos_ang;
            rot[0][1] = -sen_ang;
            rot[1][0] = sen_ang;
            rot[0][2] = rot[1][2] = rot[2][0] = rot[2][1] = rot[0][3] = rot[2][3] = rot[3][0] = rot[1][3] = rot[3][2] = 0;
            rot[2][2] =  rot[3][3] = 1;

            inv_rot[0][0] = inv_rot[1][1] = cos_ang;
            inv_rot[0][1] = sen_ang;
            inv_rot[1][0] = -sen_ang;
            inv_rot[0][2] = inv_rot[1][2] = inv_rot[2][0] = inv_rot[2][1] = inv_rot[0][3] = inv_rot[2][3] = inv_rot[0][3] = inv_rot[1][3] = inv_rot[3][2] = 0;
            inv_rot[2][2] =  inv_rot[3][3] = 1;
        }

    //transladando a reta para a origem
        trans[0][1] = trans[0][2] = trans[1][0] = trans[1][2] = trans[2][0] = trans[2][1] = trans[3][0] = trans[3][1] = trans[3][2] = 0;
        trans[0][0] = trans[1][1] = trans[2][2] = trans[3][3] = 1;
        trans[0][3] = -x1;
        trans[1][3] = -y1;
        trans[2][3] = 0;


    //reflexao em relacao ao eixo y = 0
        reflec[0][0] = reflec[3][3] = 1;
        reflec[1][1] = reflec [2][2] = cos_ang_ref;
        reflec[1][2] = -sen_ang_ref;
        reflec[2][1] = sen_ang_ref;
        reflec[0][1] = reflec[0][2] = reflec[0][3] = reflec[1][0] = reflec[1][3] = reflec[2][0] = reflec[2][3] = reflec[3][0] = reflec[3][1] = reflec[3][2] = 0;


    //reverter a translacao da reta
        inv_trans[0][1] = inv_trans[0][2] = inv_trans[1][0] = inv_trans[1][2] = inv_trans[2][0] = inv_trans[2][1] = inv_trans[3][0] = inv_trans[3][1] = inv_trans[3][2] = 0;
        inv_trans[0][0] = inv_trans[1][1] = inv_trans[2][2] = inv_trans[3][3] = 1;
        inv_trans[0][3] = x1;
        inv_trans[1][3] = y1;
        inv_trans[2][3] = 0;


        for (double i = 0.0; i<size.height; i = i + 1){
            for(double j = 0.0; j<size.width; j = j + 1){
                 cord[0] = i;
                 cord[1] = j;
                 cord[2] = 1.0;
                 res = multiplication(cord, trans, 4);
                 res = multiplication(res, rot, 4);
                 res = multiplication(res, reflec, 4);
                 res = multiplication(res, inv_rot, 4);
                 res = multiplication(res, inv_trans, 4);

                if(res[0] < 1120 && res[1] < 750 && res[0] > -1 && res[1] > -1){
                     aux.at<Vec3b>(res[1], res[0]) = final_image.at<Vec3b>(j,i);
                }
            }
        }
        return aux;
         free(cord);
         free(res);
         free(rot);
         free(trans);
         free(reflec);
         free(inv_trans);
         free(inv_rot);

}

//metodo coloca a imagem escolhida pelo usuario no grid
Mat MainWindow::convert_im(Mat image)
{
  Size size;
  size = image.size();


  Mat grHistogram(750, 1120, CV_8UC3, Scalar(0, 0, 0));

  for (double i =0.0; i<size.height; i++){
      for(double j =0.0; j<size.width;j++){
          grHistogram.at<Vec3b>(j,i) = image.at<Vec3b>(j,i);
        }
    }

  return grHistogram;
}

void MainWindow::on_apButton_clicked()
{
  QString cam = ui->lineEdit_2->text();
  QString pas = ui->nPassos_2->text();
  QMessageBox msg;
  msg.setWindowTitle("Alerta");
  if(cam.isEmpty() || pas.isEmpty()){
      msg.setText("Você não selecionou uma imagem ou não inseriu o número passos :c");
      msg.exec();
    } else {

      string str = pas.toStdString();
      int passos = atoi(str.c_str()); //Número de Passos escolhido
      Size size = image.size(); //Tamanho da imagem

      if(ui->rotCheck_2->isChecked()){
          if(point1.isNull()){}

          double angulo = ui->angSlid_2->value(); //Ângulo escolhido

          //O centro de rotação ta no point1
          for(int i = 1; i <= passos; i++){ //Exibe na GUI o resultado de cada passo
              Mat aux = MainWindow::rotation(point1.x(), point1.y(), angulo*i/passos, size);
              QImage img = convert_mat2qimage(aux);
              ui->gridLab->setPixmap(QPixmap::fromImage(img));
              waitKey(10);
            }
          count = 1;
        } else if(ui->escCheck_2->isChecked()) {
          double x = ui->xSlid_2->value(); //Fatores de Escala
          double y = ui->ySlid_2->value();

          for(int i = 1; i <= passos; i++){ //Exibe na GUI o resultado de cada passo
              Mat aux = MainWindow::scale(x*i/passos, y*i/passos, size);
              QImage img = convert_mat2qimage(aux);
              ui->gridLab->setPixmap(QPixmap::fromImage(img));
              waitKey(10);
            }
        } else if(ui->tranCheck_2->isChecked()){
          //O ponto de translação ta no point1
          double x = (double)point1.x();
          double y = (double)point1.y();

          for(int i = 1; i <= passos; i++){ //Exibe na GUI o resultado de cada passo
              Mat aux = MainWindow::translation(x*i/passos, y*i/passos, size);
              QImage img = convert_mat2qimage(aux);
              ui->gridLab->setPixmap(QPixmap::fromImage(img));
              waitKey(10);
            }
          count = 1;
        } else if(ui->refCheck_2->isChecked()){
          //A reta ta definida pelos point1 e point2
          double x1, y1, x2, y2;
          x1 = (double) point1.x();
          y1 = (double) point1.y();
          x2 = (double) point2.x();
          y2 = (double) point2.y();
          double ang = 180.0;

          for(int i = 1; i <= passos; i++){ //Exibe na GUI o resultado de cada passo
              Mat aux = MainWindow::reflection(x1, x2, y1, y2, i*ang/passos, size);
              QImage img = QImage((uchar*) aux.data, aux.cols, aux.rows, aux.step, QImage::Format_RGB888);
              ui->gridLab->setPixmap(QPixmap::fromImage(img));
              if(i+1 > passos){
                  ui->gridLab->setPixmap(QPixmap::fromImage(img.rgbSwapped()));
              }
              waitKey(10);
           }
        } else if(ui->cisCheck_2->isChecked()){
          double x = ui->xSlid_2->value(); //Fatores de Cisalhamento
          double y = ui->ySlid_2->value();
          for(int i = 1; i <= passos; i++){ //Exibe na GUI o resultado de cada passo
              Mat aux = MainWindow::shearing(x*i/passos, y*i/passos, size);
              QImage img = convert_mat2qimage(aux);
              ui->gridLab->setPixmap(QPixmap::fromImage(img));
              waitKey(10);
            }
        } else {
          msg.setText("Você não selecionou nenhum CheckBox :(");
          msg.exec();
        }
    }
}

void MainWindow::on_imgButton_clicked()
{
  QString filecam = QFileDialog::getOpenFileName(this, tr("Carregar Imagem"));
  ui->lineEdit_2->setText(filecam);
  image = imread(ui->lineEdit_2->text().toStdString()); //Ler a imagem no diretorio informado
  if(!image.data){
      QMessageBox msg;
      msg.setText("A imagem não foi encontrada :c");
      msg.exec();
    } else {
      final_image = convert_im(image);
      //Converter cv::Mat para QImage
      QImage img = convert_mat2qimage(final_image);
      ui->gridLab->setPixmap(QPixmap::fromImage(img)); //Joga na QLabel
    }
}

void MainWindow::Mouse_Pressed()
{
  if(count == 1){
      point1 = QPoint(ui->gridLab->x, ui->gridLab->y);
      ui->label->setText(QString("Point1: X = %1, Y = %2").arg(point1.x()).arg(point1.y()));
      count++;
    } else {
      point2 = QPoint(ui->gridLab->x, ui->gridLab->y);
      ui->label->setText(QString("Point2: X = %1, Y = %2").arg(point2.x()).arg(point2.y()));
      count = 1;
    }
}

QImage MainWindow::convert_mat2qimage (Mat image){
    QImage aux;

    QImage img = QImage((uchar*) image.data, image.cols, image.rows, image.step, QImage::Format_RGB888);
    aux = img.rgbSwapped();
    return aux;
}
