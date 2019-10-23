#ifndef PAINTWINDOW_H
#define PAINTWINDOW_H

#include <QMainWindow>
//#include "graphics.h"
#include "linesegment.h"
#include "circle.h"
#include "eraser.h"
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
    ~Paint2DWidget();
private:
    Mode curMode;                               //当前的模式
    QColor curColor;                            //当前选择的颜色
    QVector<Graphics* > graphicsList;           //界面上所有的图形
    Graphics* curGraphics;                      //当前正在画的图形
    Eraser* eraser;                              //橡皮擦

//    QPainter painter;                           //画家
//    QPen pen;                                   //画笔

    void drawGraphics(Graphics* graphics);
    void eraseGraphics();                       //擦除橡皮擦覆盖到的图形

protected:
    void paintEvent(QPaintEvent* e);
//    void keyPressEvent(QKeyEvent* e);
    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
};


#endif // PAINTWINDOW_H
