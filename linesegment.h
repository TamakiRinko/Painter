#ifndef LINESEGMENT_H
#define LINESEGMENT_H

#include "graphics.h"

class LineSegment: public Graphics{
public:
    LineSegment();
    LineSegment(QPoint startPoint, QColor color = DEFAULT_COLOR, int width = DEFAULT_WIDTH);
    LineSegment(QPoint startPoint, QPoint endPoint, QColor color = DEFAULT_COLOR, int width = DEFAULT_WIDTH);
    virtual ~LineSegment(){}
    void setEndPoint(QPoint endPoint);
    void drawLogic();
    bool isNotGraphics();
private:
    QPoint startPoint;
    QPoint endPoint;
    void bresenHam();
};

#endif // LINESEGMENT_H
