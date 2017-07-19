#include "selectrect.h"
#include <iostream>
#include <QDebug>

SelectRect::SelectRect(QWidget *parent) : QWidget(parent)
{
    //注册结构体
    rect_info r;
    QVariant DataVar;
    DataVar.setValue(r);
    qRegisterMetaType<QVariant>("QVariant");
    this->setAttribute(Qt::WA_DeleteOnClose);
}

SelectRect::~SelectRect()
{

}


void SelectRect::paintEvent(QPaintEvent *event)
{
    // 背景
    QPainterPath mask;
    // 选中的范围
    QPainterPath select_area;
    // 椭圆
//    QRect boundingRectangle(rect.x,rect.y,rect.w,rect.h);
//    select_area.addEllipse(boundingRectangle);
    select_area.addRect(rect.x,rect.y,rect.w,rect.h);
    mask.addRect(this->geometry());
    QPainterPath drawMask =mask.subtracted(select_area);

    QPainter paint(this);
    paint.setOpacity(0.5);
    paint.fillPath(drawMask,QBrush(Qt::black));

}

void SelectRect::mousePressEvent(QMouseEvent *event)
{
        switch(event->button())
        {
        case Qt::LeftButton:
            mouse = Left;
            rect.x = event->x();
            rect.y = event->y();
//            qDebug() << first_mousePosX << first_mousePosX;
            break;
        case Qt::RightButton:
            mouse = Right;
            break;
        case Qt::MiddleButton:
            mouse = Mid;
            break;
        default:
            mouse = No;
    }
}

void SelectRect::mouseMoveEvent(QMouseEvent *event)
{
    if (mouse == Left)
    {
        // 限定在mask内
        int x = event->x();
        int y = event->y();
        if(x < 0)
            x = 0;
        else if(x > this->width())
            x = this->width();
        if(y < 0)
            y = 0;
        else if (y > this->height())
            y = this->height();
        rect.w = x - rect.x;
        rect.h = y - rect.y;

    }
    update();
}

void SelectRect::contextMenuEvent(QContextMenuEvent *event)
{
    QCursor cur=this->cursor();
    subMenu = new QMenu();
    subActionReset = subMenu->addAction(tr("重选"));
    subActionSave = subMenu->addAction(tr("另存为"));
    subActionExit = subMenu->addAction(tr("退出"));
    connect(subActionExit,SIGNAL(triggered()),this,SLOT(select_exit()));
    connect(subActionSave,SIGNAL(triggered()),this,SLOT(send_rect_info()));
    connect(subActionReset,SIGNAL(triggered()),this,SLOT(select_reset()));
    subMenu->exec(cur.pos());
    delete subMenu;
}

void SelectRect::select_exit()
{
    this->close();
}
void SelectRect::send_rect_info()
{
    QVariant send_rect_data;
    send_rect_data.setValue(rect);
    emit send_data(send_rect_data);
}

void SelectRect::select_reset()
{
    rect.w = 0;
    rect.h = 0;
    update();
}
