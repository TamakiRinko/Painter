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
using namespace std;


class Graphics;

enum Mode{
    NONE, LINESEGMENT, RANDOMLINE, POLYGON, CIRCLE, ELLIPSE, ERASER, TRANSLATION, SELECT
};

enum LineAlgorithm{
    DDA, BRESENHAM
};

const QColor DEFAULT_COLOR = Qt::black;
const int DEFAULT_WIDTH = 1;
const LineAlgorithm DEFAULT_ALG = LineAlgorithm::BRESENHAM;

class Graphics{
public:
    Graphics(): isErased(false){}
    Graphics(QColor c, int w);
    Graphics(const Graphics& g);
    virtual ~Graphics();

    const QVector<QPoint* >& getPoints() const;                   //退而求其次，不可修改，只可拿到
    void append(QPoint* point);
    void clear();
    bool pointIsIn(QPoint point);                           //point是否在图形中

    void setIsErased(bool b);
    bool getIsErased();
    QColor& getColor();
    void setColor(QColor c);
    void resetColor();
    int getNum();
    int getWidth();
    Mode getMode();

    QPoint& operator[](int i);

    virtual void drawLogic() = 0;
    virtual bool isNotGraphics() = 0;
    virtual void translation(int xOffset, int yOffset) = 0; //图元平移
protected:
    QVector<QPoint* > points;
    QColor color;
    QColor oldColor;                                        //之前的颜色
    int width;                                              //像素
    bool isErased;                                          //是否已经被删除
    Mode mode;                                              //自己的类型
};



#endif // GRAPHICS_H
