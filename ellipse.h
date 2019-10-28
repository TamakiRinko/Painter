#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "graphics.h"

class Ellipse: public Graphics{
public:
    Ellipse();
    Ellipse(QPoint center, QColor color = DEFAULT_COLOR);
    void setPoint(QPoint point);
    void drawLogic();
    bool isPoint();
private:
    int rx;
    int ry;
    QPoint center;
    QPoint outPoint;
    void mid();
};

#endif // ELLIPSE_H
