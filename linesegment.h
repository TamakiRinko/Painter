#ifndef LINESEGMENT_H
#define LINESEGMENT_H

#include "graphics.h"

class LineSegment: public Graphics{
public:
    LineSegment(int id);
    LineSegment(int id, QPoint startPoint, QColor color = DEFAULT_COLOR, int width = DEFAULT_WIDTH, LineAlgorithm alg = DEFAULT_ALG);
    LineSegment(int id, QPoint startPoint, QPoint endPoint, QColor color = DEFAULT_COLOR, int width = DEFAULT_WIDTH, LineAlgorithm alg = DEFAULT_ALG);
    LineSegment(const LineSegment& l);
    virtual ~LineSegment(){}
    void setEndPoint(QPoint endPoint);
    void drawLogic();
    bool isNotGraphics();
    void translation(int xOffset, int yOffset);
    void rotation(const QPoint* point, int degree);
    void scale(const QPoint* point, double times);
    bool crop(int xMin, int xMax, int yMin, int yMax, CropAlgorithm curAlg);
private:
    QPoint startPoint;
    QPoint endPoint;
    LineAlgorithm alg;
    void bresenHam();
    void DDA();
    bool CS();
    bool LB();
};

#endif // LINESEGMENT_H
