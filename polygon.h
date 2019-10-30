#ifndef POLYGON_H
#define POLYGON_H

#include "graphics.h"
#include "linesegment.h"

class Polygon: public Graphics{
public:
    Polygon();
    Polygon(QPoint startPoint, QColor color = DEFAULT_COLOR, int width = DEFAULT_WIDTH);
    void setNextPoint(QPoint nextPoint);
    void complete();
    virtual ~Polygon(){}
    void drawLogic();
    bool isNotGraphics();
private:
    QPoint* startPoint;
    QVector<QPoint*> vertexList;                    //多边形顶点集
    int num;                                        //当前有多少顶点
    QVector<LineSegment*> lineList;                 //多边形边集
};

#endif // POLYGON_H
