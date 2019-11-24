#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "graphics.h"

class Ellipse: public Graphics{
public:
    Ellipse();
    Ellipse(QPoint center, QColor color = DEFAULT_COLOR, int width = DEFAULT_WIDTH);
    virtual ~Ellipse(){}
    void setPoint(QPoint point);
    void drawLogic();
    bool isNotGraphics();
    void translation(int xOffset, int yOffset);
    void rotation(const QPoint* point, int degree);
private:
    int rx;
    int ry;
    QPoint center;
    QPoint outPoint;
    void mid();
};

#endif // ELLIPSE_H
