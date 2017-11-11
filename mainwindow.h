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
#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "ImageWidget.h"

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
    void auto_size(cv::Mat &src);
    void show_open_img(cv::Mat &input_img);
    void get_blur_img_value();
    void blur_img_process();

private:
    Ui::MainWindow *ui;
    cv::Mat input_file;
    cv::Mat blur_img;
    QImage* send_img = new QImage;
    QImage* ori_img;
    bool is_select_press = false;
    int size;
    int sigmaX;
    int sigmaY;
    bool img_data = false;
    QImage Mat2QImage(cv::Mat &mat);
    cv::Mat QImage2Mat(QImage image);

};

#endif // MAINWINDOW_H
