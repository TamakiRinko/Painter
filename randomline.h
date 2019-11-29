#ifndef RANDOMLINE_H
#define RANDOMLINE_H

#include "graphics.h"
#include "linesegment.h"

class RandomLine: public Graphics{
public:
    RandomLine();
    RandomLine(QPoint startPoint, QColor color = DEFAULT_COLOR, int width = DEFAULT_WIDTH);
    virtual ~RandomLine(){}
    void setPoint(QPoint nextPoint);
    void drawLogic();
    bool isNotGraphics();
    void translation(int xOffset, int yOffset);
    void rotation(const QPoint* point, int degree);
    void scale(const QPoint* point, double times);
    bool crop(int xMin, int xMax, int yMin, int yMax, CropAlgorithm curAlg){
        return true;
    }
private:
    QPoint curPoint;
    LineSegment* line;                      //两点之间画线
};

#endif // RANDOMLINE_H
