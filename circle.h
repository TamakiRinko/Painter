#ifndef CIRCLE_H
#define CIRCLE_H

#include "graphics.h"
#include <math.h>

class Circle: public Graphics{
public:
    Circle();
    Circle(QPoint center, QColor color = DEFAULT_COLOR);
    void setPoint(QPoint point);
    void drawLogic();
    bool isPoint();
private:
    QPoint center;
    QPoint outPoint;                    //圆周上的一点
    int radius;
    void mid();
};

#endif // CIRCLE_H
