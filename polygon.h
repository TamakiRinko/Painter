#ifndef POLYGON_H
#define POLYGON_H

#include "graphics.h"
#include "linesegment.h"

class Polygon: public Graphics{
public:
    Polygon();
    Polygon(QPoint startPoint, QColor color = DEFAULT_COLOR, int width = DEFAULT_WIDTH, LineAlgorithm alg = LineAlgorithm::DDA);
    void setNextPoint(QPoint nextPoint);
    void complete();
    void withDraw();
    virtual ~Polygon(){}
    void drawLogic();
    bool isNotGraphics();
private:
    QPoint* startPoint;
    QVector<QPoint*> vertexList;                    //多边形顶点集
    QVector<LineSegment*> lineList;                 //多边形边集
    int num;                                        //当前有多少顶点
    LineAlgorithm alg;
};

#endif // POLYGON_H
