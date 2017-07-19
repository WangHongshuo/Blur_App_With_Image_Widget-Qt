#ifndef SELECTRECT_H
#define SELECTRECT_H

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QWheelEvent>
#include <QMenu>
#include <QCursor>
#include <QFileDialog>
#include <QSplitter>
#include <QMetaType>
#include <QVariant>
#include "ImageWidget.h"


typedef struct rect_info
{
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
}rect_info;
Q_DECLARE_METATYPE(rect_info)

class SelectRect : public QWidget
{
    Q_OBJECT
public:
    SelectRect(QWidget *parent);
    ~SelectRect();


    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);


signals:
    void send_data(QVariant send_rect_data);
public slots:
    void select_exit();
    void send_rect_info();
    void select_reset();
private:
    QMenu* subMenu;
    QAction* subActionReset;
    QAction* subActionSave;
    QAction* subActionExit;
    rect_info rect;

    int mouse;

};

#endif // SELECTRECT_H
