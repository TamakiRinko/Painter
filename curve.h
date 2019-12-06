#ifndef CURVE_H
#define CURVE_H

#include "graphics.h"
#include "linesegment.h"

class Curve: public Graphics{
public:
    Curve(int id, int k = 3);
    Curve(int id, QPoint point, QColor color = DEFAULT_COLOR, int width = DEFAULT_WIDTH, CurveAlgorithm alg = CurveAlgorithm::BEZIER, int k = 3);
    Curve(const Curve& p);
    ~Curve();

    void setCurK(int i);
    void setNextPoint(QPoint point);
    void withDraw();
    void translationPoint(int index, QPoint start, QPoint end);

    void drawLogic();
    bool isNotGraphics();
    //TODO:
    void translation(int xOffset, int yOffset);
    void rotation(const QPoint* point, int degree);
    void scale(const QPoint* point, double times);
    bool crop(int xMin, int xMax, int yMin, int yMax, CropAlgorithm curAlg);

private:
    int vertexNum;                                          //点的个数
    QVector<QPoint*> vertexList;                            //曲线顶点集
    CurveAlgorithm alg;                                     //算法
    int curK;                                               //k次B样条

    void bezier();
    void bspline();
    double curK1(int i, double u);
    double curK2(int i, double u);
    double curK3(int i, double u);
};

#endif // CURVE_H
