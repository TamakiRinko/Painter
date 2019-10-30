#ifndef LINESEGMENT_H
#define LINESEGMENT_H

#include "graphics.h"

class LineSegment: public Graphics{
public:
    LineSegment();
    LineSegment(QPoint startPoint, QColor color = DEFAULT_COLOR, int width = DEFAULT_WIDTH, LineAlgorithm alg = DEFAULT_ALG);
    LineSegment(QPoint startPoint, QPoint endPoint, QColor color = DEFAULT_COLOR, int width = DEFAULT_WIDTH, LineAlgorithm alg = DEFAULT_ALG);
    virtual ~LineSegment(){}
    void setEndPoint(QPoint endPoint);
    void drawLogic();
    bool isNotGraphics();
private:
    QPoint startPoint;
    QPoint endPoint;
    LineAlgorithm alg;
    void bresenHam();
    void DDA();
};

#endif // LINESEGMENT_H
