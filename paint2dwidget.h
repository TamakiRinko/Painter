#ifndef PAINTWINDOW_H
#define PAINTWINDOW_H

#include <QMainWindow>
//#include "graphics.h"
#include "linesegment.h"
#include "circle.h"
#include "eraser.h"
#include "ellipse.h"
#include <QWidget>
#include <QPen>
#include <QBrush>
#include <QMouseEvent>
#include <QDebug>


class Paint2DWidget : public QWidget{
    Q_OBJECT

public:
    explicit Paint2DWidget(QWidget *parent = nullptr);
    void setMode(Mode mode);
    void setColor(QColor color);
    void setWidth(int width);
    void saveTo(QString fileName, const char* format = "bmp");            //保存为指定格式
    bool getIsModified();
    ~Paint2DWidget();
private:
    Mode curMode;                               //当前的模式
    QColor curColor;                            //当前选择的颜色
    int curWidth;                               //当前的宽度
    QVector<Graphics* > graphicsList;           //界面上所有的图形
    Graphics* curGraphics;                      //当前正在画的图形
    Eraser* eraser;                              //橡皮擦
    bool isModified;

//    QPainter painter;                           //画家
//    QPen pen;                                   //画笔

    void drawGraphics(QPainter& painter, Graphics* graphics);
    void eraseGraphics();                       //擦除橡皮擦覆盖到的图形

protected:
    void paintEvent(QPaintEvent* e);
    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
};


#endif // PAINTWINDOW_H
