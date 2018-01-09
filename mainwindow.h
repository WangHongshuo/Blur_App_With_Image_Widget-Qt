#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPaintEvent>
#include <QMainWindow>
#include <QImage>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <assert.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "ImageWidget-Qt/ImageWidget.h"
#include "QImageMatConvert/mat_qimage_convert.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_open_file_clicked();
    void show_open_img(cv::Mat &input_img);
    void get_blur_img_value();
    void blur_img_process();

private:
    Ui::MainWindow *ui;
    cv::Mat input_file;
    cv::Mat blur_img;
    QImage* send_img = new QImage;
    QImage* ori_img = new QImage;
    bool is_select_press = false;
    int size;
    int sigmaX;
    int sigmaY;
    bool img_data = false;

};

#endif // MAINWINDOW_H
