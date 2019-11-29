#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include <QPainter>
#include <QColor>
#include <QVector>
#include <QPoint>
#include <QSet>
#include <QList>
#include <math.h>
#include <QDebug>
using namespace std;

class Graphics;

enum Mode{
    NONE, LINESEGMENT, RANDOMLINE, POLYGON, CIRCLE, ELLIPSE, ERASER,
    TRANSLATION, SELECT, ROTATION, SCALE, SELECTBOLCK, CROP
};

enum LineAlgorithm{
    DDA, BRESENHAM
};

enum CropAlgorithm{
    CS, LB, WA
};

const QColor DEFAULT_COLOR = Qt::black;
const int DEFAULT_WIDTH = 1;
const LineAlgorithm DEFAULT_ALG = LineAlgorithm::BRESENHAM;
const double PI = 3.1415;

const int MIDDLE = 0;
const int LEFT = 1;
const int RIGHT = 2;
const int DOWN = 4;
const int UP = 8;

class Graphics{
public:
    Graphics(): isErased(false){}
    Graphics(int id, QColor c, int w);
    Graphics(const Graphics& g);

    QPoint& operator[](int i);
    void clear();
    void append(QPoint* point);
    bool pointIsIn(QPoint point);                                   //point是否在图形中
    bool pointIsInBlock(QPoint startPoint, QPoint endPoint);        //图形是否有一部分在矩形中
    void pointRotation(QPoint* movePoint, const QPoint* basePoint, int degree);            //点的旋转
    void pointScale(QPoint* scalePoint, const QPoint* basePoint, double times);            //点的缩放
    int regionCode(int x, int y);                                  //获得该点的区域码

    const QVector<QPoint* >& getPoints() const;                     //退而求其次，不可修改，只可拿到
    void setIsErased(bool b);
    bool getIsErased();
    QColor& getColor();
    void setColor(QColor c);
    void resetColor();
    int getNum();
    int getWidth();
    Mode getMode();

    virtual ~Graphics();
    virtual void drawLogic() = 0;
    virtual bool isNotGraphics() = 0;
    virtual void translation(int xOffset, int yOffset) = 0;         //图元平移
    virtual void rotation(const QPoint* point, int degree) = 0;     //图元旋转
    virtual void scale(const QPoint* point, double times) = 0;      //图元缩放
    virtual bool crop(int xMin, int xMax, int yMin, int yMax, CropAlgorithm curAlg) = 0;    //图元裁剪
protected:
    int id;                                                 //编号
    QVector<QPoint* > points;                               //图元点集
    QColor color;
    QColor oldColor;                                        //之前的颜色
    int width;                                              //像素
    bool isErased;                                          //是否已经被删除
    Mode mode;                                              //自己的类型

    int xMin;
    int xMax;
    int yMin;
    int yMax;
};



#endif // GRAPHICS_H
