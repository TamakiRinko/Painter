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
const QColor DEFAULT_COLOR = Qt::black;

class Graphics;

enum Mode{
    NONE, LINESEGMENT, CIRCLE, ELLIPSE, ERASER
};

class Graphics{
public:
    Graphics(): isErased(false){}
    virtual ~Graphics();
    Graphics(QColor c): color(c), isErased(false){}
//    QVector<Point* >& getPoints();                        //不要有类似的函数，可以直接获得最重要的points
    void append(QPoint* point);
    void clear();
    int getNum();
    void setIsErased(bool b);
    bool getIsErased();
    QVector<QPoint* >::iterator getBeginIterator();
    QVector<QPoint* >::iterator getEndIterator();
    QColor& getColor();

    QPoint& operator[](int i);

    virtual void drawLogic() = 0;
    virtual bool isPoint() = 0;
protected:
    QVector<QPoint* > points;
    QColor color;
    bool isErased;                                          //是否已经被删除
};



#endif // GRAPHICS_H
