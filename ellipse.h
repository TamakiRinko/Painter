#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "graphics.h"

class Ellipse: public Graphics{
public:
    Ellipse(int id);
    Ellipse(int id, QPoint center, QColor color = DEFAULT_COLOR, int width = DEFAULT_WIDTH);
    virtual ~Ellipse(){}
    void setPoint(QPoint point);
    void setR(int rx, int ry);
    void drawLogic();
    bool isNotGraphics();
    void translation(int xOffset, int yOffset);
    void rotation(const QPoint* point, int degree);
    void scale(const QPoint* point, double times);
    bool crop(int xMin, int xMax, int yMin, int yMax, CropAlgorithm curAlg){
        return true;
    }
private:
    int rx;
    int ry;
    QPoint center;
    QPoint outPoint;
    void mid();
};

#endif // ELLIPSE_H
