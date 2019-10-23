#ifndef LINESEGMENT_H
#define LINESEGMENT_H

#include "graphics.h"

class LineSegment: public Graphics{
public:
    LineSegment();
    LineSegment(QPoint startPoint, QColor color = DEFAULT_COLOR);
    ~LineSegment(){}
    void setEndPoint(QPoint endPoint);
    void drawLogic();
    bool isPoint();
private:
    QPoint startPoint;
    QPoint endPoint;
    void bresenHam();
};

#endif // LINESEGMENT_H
