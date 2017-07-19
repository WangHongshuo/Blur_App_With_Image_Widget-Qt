#include "ImageWidget.h"
#include <iostream>
#include <QDebug>
#include "selectrect.h"

ImageWidget::ImageWidget(QWidget *parent):QWidget(parent),isImageLoad(false),scalex(1),scaley(1),xtranslate(0),ytranslate(0),mouse(No),maxmum(false)
{
    iniActions();
}

ImageWidget::~ImageWidget(void)
{
    if(isImageLoad)
        delete mp_img;
}


void ImageWidget::wheelEvent(QWheelEvent *e)
{
    int numDegrees = e->delta();
    if(numDegrees > 0)
    {
        zoomout();
    }
    if(numDegrees < 0)
    {
        zoomin();
    }
    update();
}
void ImageWidget::mousePressEvent(QMouseEvent *e)
{
    if(isImageLoad)
    {
        switch(e->button())
        {
        case Qt::LeftButton:
            mouse = Left;
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
//        qDebug() << mouse;
        //初次按键事件鼠标坐标
        mousePosX = e->x();
        mousePosY = e->y();
    }
}

void ImageWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if(mouse == Left)
    {
        last_x_pos = xtranslate;
        last_y_pos = ytranslate;
    }
}


void ImageWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(mouse == Left )
    {
        //xtranslate = e->x() - mousePosX;
        //ytranslate = e->y() - mousePosY;

//        qDebug() << "e.mouseposX: " << mousePosX << "e.mouseposY: " << mousePosY;
//        qDebug() << "e.x: " << e->x() << "e.y: " << e->y();
        //相对移动坐标
        translate(e->x() - mousePosX,e->y() - mousePosY);
    }
}

void ImageWidget::translate(int x,int y)
{
    xtranslate = last_x_pos + x;
    ytranslate = last_y_pos + y;
    update();
}

void ImageWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setBrush(QBrush(QColor(200,200,200)));
    painter.drawRect(0,0,this->width(),this->height());
    if(!isImageLoad)
        return;
    painter.drawImage(QPoint(xtranslate,ytranslate),mp_img->scaled(this->width()*scalex,this->height()*scaley,Qt::KeepAspectRatio));
//    qDebug() << mp_img->width() << mp_img->height();

}

void ImageWidget::contextMenuEvent(QContextMenuEvent *e)
{
    mMenu->exec(QCursor::pos());
    //mousePosX = QCursor::pos().x();
    //mousePosY = QCursor::pos().y();
}

void ImageWidget::resetPos()
{
    xtranslate = 0;
    ytranslate = 0;
    scalex = 1;
    scaley = 1;
    last_x_pos = 0;
    last_y_pos = 0;
    update();
}

void ImageWidget::zoomout()
{
    if(scalex<=100&&scaley<=100)
    {
        scalex *= 1.2;
        scaley *= 1.2;
    }
    update();
}

void ImageWidget::zoomin()
{
    if(scalex>=0.01&&scaley>=0.01)
    {
        scalex *= 1/1.2;
        scaley *= 1/1.2;
    }
    update();
}

void ImageWidget::select()
{
//    qDebug() << flag;
    emit is_select_press();
}

void ImageWidget::receiver_rect(QVariant rect_data)
{
    // 接受到截取框信息
    if(isImageLoad)
    {
        rect_info data;
        data = rect_data.value<rect_info>();
        // 修正矩形坐标
        if(data.w < 0)
        {
            data.x += data.w;
            data.w = -data.w;
        }
        if(data.h < 0)
        {
            data.y += data.h;
            data.h = -data.h;
        }
//        qDebug() << data.x << data.y << data.w << data.h;
        if(data.w == 0 && data.h == 0)
        {
            QMessageBox msgBox(QMessageBox::Critical,tr("错误"),tr("未选取区域！"));
            msgBox.exec();
        }
        else
        {
            QImage* temp = new QImage;
            QImage* save_img = new QImage;
            *temp = mp_img->copy();
            //        qDebug()<< temp << &mp_img;
            //        qDebug() << temp->size();
            //        qDebug() << this->width() * scalex << this->height() * scalex;
            *temp = temp->scaled(this->width() * scalex,this->height() * scalex,Qt::KeepAspectRatio);
            //        qDebug() << temp->size();
            // 计算相对于图像内的坐标
            int x,y,w,h;
            x = data.x - xtranslate;
            y = data.y - ytranslate;
            w = data.w;
            h = data.h;
            // 限定截取范围在图像内
            // 修正顶点
            if(x < 0)
            {
                w = data.w + x;
                x = 0;
            }
            if (y < 0)
            {
                h = data.h + y;
                y = 0;
            }
            if(x + data.w > temp->width())
                w = temp->width() - x;
            if(y + data.h > temp->height())
                h = temp->height() - y;
//            qDebug() << xtranslate << ytranslate << temp->width() << temp->height();
//            qDebug() << x << y << w << h;
            if(w > 0 && h > 0)
            {
                *save_img = temp->copy(x,y,w,h);
                QString filename = QFileDialog::getSaveFileName(this, tr("Open File"),
                                                                "C:/Users/Aaron/Pictures/",
                                                                tr("Images (*.png *.xpm *.jpg *.tiff *.bmp)"));
                save_img->save(filename);
            }
            else
            {
                QMessageBox msgBox(QMessageBox::Critical,tr("错误"),tr("未选中图像！"));
                msgBox.exec();
            }
            delete temp;
            delete save_img;
        }
    }
    else
    {
        QMessageBox msgBox(QMessageBox::Critical,tr("错误"),tr("未选中图像！"));
        msgBox.exec();
    }
}

void ImageWidget::iniActions()
{
    mMenu = new QMenu();
    mActionResetPos = mMenu->addAction(tr("重置"));
    mActionSave		= mMenu->addAction(tr("另存为"));
    mActionSelect   = mMenu->addAction(tr("截取"));

    connect(mActionResetPos,SIGNAL(triggered()),this,SLOT(resetPos()));
    connect(mActionSave,SIGNAL(triggered()),this,SLOT(save()));
    connect(mActionSelect,SIGNAL(triggered()),this,SLOT(select()));

    if(widgetid == 3)
    {
        conditionDialation = mMenu->addAction(tr("Conditionnal dilation"));
        connect(conditionDialation,SIGNAL(triggered()),this,SLOT(con_dia()));
    }
}

void ImageWidget::con_dia()
{
    if(isImageLoad)
    {
        QImage tmp = mp_img->scaled(this->width()*scalex,this->height()*scaley,Qt::KeepAspectRatio);

        emit sig_condional_dia(int(mousePosX*1.0 / tmp.width() * mp_img->width()),int(mousePosY*1.0 / tmp.height() * mp_img->height()));
    }
}

void ImageWidget::save()
{
    if(isImageLoad)
    {
        QString filename = QFileDialog::getSaveFileName(this, tr("Open File"),
                                                        "C:/Users/Aaron/Pictures/",
                                                        tr("Images (*.png *.xpm *.jpg *.tiff *.bmp)"));
        mp_img->save(filename);
    }
}

//void ImageWidget::mouseDoubleClickEvent(QMouseEvent *e)
//{
//	if(!maxmum)
//	{
//		QSplitter *splitter = (QSplitter*)parentWidget();
//		QSplitter *splitter1 = (QSplitter*)splitter->parentWidget();

//		if(widgetid==0 || widgetid==1)
//		{
//			splitter1->widget(1)->hide();
//			splitter->widget(widgetid%2==0?1:0)->hide();
//		}

//		if(widgetid==2||widgetid==3)
//		{
//			splitter1->widget(0)->hide();
//			splitter->widget(widgetid%2==0?1:0)->hide();
//		}
//		maxmum = true;
//	}
//	else
//	{
//		QSplitter *splitter = (QSplitter*)parentWidget();
//		QSplitter *splitter1 = (QSplitter*)splitter->parentWidget();

//		for(int i=0;i!=splitter->count();i++)
//		{
//			splitter->widget(i)->show();
//		}

//		maxmum = false;
//		for (int i=0;i!=splitter1->count();i++)
//		{
//			splitter1->widget(i)->show();
//		}
//	}
//}
