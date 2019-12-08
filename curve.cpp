#include "curve.h"

extern long long combine[maxCurveNum][maxCurveNum];
extern double powerU[stepsNum][maxCurveNum];

Curve::Curve(int id, int k): Graphics(id, DEFAULT_COLOR, DEFAULT_WIDTH), curK(k){
    mode = CURVE;
}

Curve::Curve(int id, QPoint point, QColor color, int width, CurveAlgorithm alg, int k): Graphics(id, color, width), curK(k){
    mode = CURVE;
    this->alg = alg;
    QPoint* startPoint = new QPoint(point.x(), point.y());
    vertexList.push_back(startPoint);
    vertexNum = 1;
}

Curve::Curve(const Curve& p): Graphics (p){
    mode = CURVE;
    alg = p.alg;
    vertexNum = p.vertexNum;
    curK = p.curK;
    vertexList.clear();
    for(int i = 0; i < p.vertexList.size(); ++i){
        QPoint* point = new QPoint(p.vertexList[i]->x(), p.vertexList[i]->y());
        vertexList.push_back(point);
    }
}

Curve::~Curve(){
    for(int i = 0; i < vertexList.size(); ++i){
        delete vertexList[i];
    }
    vertexList.clear();
    vertexNum = 0;
}

void Curve::setCurK(int i){
    curK = i;
//    drawLogic();
}

void Curve::setNextPoint(QPoint point){
    QPoint* nextPoint = new QPoint(point.x(), point.y());
    vertexList.push_back(nextPoint);
    vertexNum++;
    drawLogic();
}

void Curve::withDraw(){
    if(vertexList.size() > 1){
        delete vertexList[vertexList.size() - 1];
        vertexList.pop_back();
    }
    if(vertexNum > 1){
        vertexNum--;
    }
    drawLogic();
}

void Curve::translationPoint(int index, QPoint start, QPoint end){
    int xOffset = end.x() - start.x();
    int yOffset = end.y() - start.y();
    vertexList[index]->setX(vertexList[index]->x() + xOffset);
    vertexList[index]->setY(vertexList[index]->y() + yOffset);
    drawLogic();
}

void Curve::drawLogic(){
    points.clear();
//    if(vertexNum == 2){                         //两点，为直线
//        LineSegment* line = new LineSegment(id, *(vertexList[0]), *(vertexList[1]), color, width, BRESENHAM);
//        points.append(line->getPoints());
//    }else
    if(alg == BEZIER){
        bezier();
    }else{
        bspline();
    }
}

bool Curve::isNotGraphics(){
    return (vertexNum < 2 || (vertexNum == 2 && vertexList[0]->x() == vertexList[1]->x() && vertexList[0]->y() == vertexList[1]->y()));
}

void Curve::translation(int xOffset, int yOffset){
    for(int i = 0; i < vertexList.size(); ++i){
        vertexList[i]->setX(vertexList[i]->x() + xOffset);
        vertexList[i]->setY(vertexList[i]->y() + yOffset);
    }
    for(int i = 0; i < points.size(); ++i){
        points[i]->setX(points[i]->x() + xOffset);
        points[i]->setY(points[i]->y() + yOffset);
    }
}

void Curve::rotation(const QPoint* point, int degree){
    //控制点旋转
    for(int i = 0; i < vertexList.size(); ++i){
        pointRotation(vertexList[i], point, degree);
    }
    drawLogic();
}

void Curve::scale(const QPoint* point, double times){
    //控制点缩放
    for(int i = 0; i < vertexList.size(); ++i){
        pointScale(vertexList[i], point, times);
    }
    drawLogic();
}

bool Curve::crop(int xMin, int xMax, int yMin, int yMax, CropAlgorithm curAlg){
    return true;
}

void Curve::bezier(){
    int n = vertexNum - 1;                              //n + 1个控制点
    for(int u = 0; u < stepsNum; ++u){
        double pointX = 0;
        double pointY = 0;
        for(int i = 0; i <= n; ++i){
            pointX = pointX + combine[n][i] * powerU[u][i] * powerU[stepsNum - u][n - i] * vertexList[i]->x();
            pointY = pointY + combine[n][i] * powerU[u][i] * powerU[stepsNum - u][n - i] * vertexList[i]->y();
        }
        QPoint* p = new QPoint((int)(round(pointX)), (int)(round(pointY)));
        points.push_back(p);
    }
}

void Curve::bspline(){
    for(double u = curK; u < vertexNum; u += 0.001){
        QPointF tmp(0,0);
        for(int i = 0; i < vertexNum; i++){
            QPointF t(vertexList[i]->x(), vertexList[i]->y());
            switch(curK){
                case 1:{
                    t = t * curK1(i, u);
                    break;
                }
                case 2:{
                    t = t * curK2(i, u);
                    break;
                }
                case 3:{
                    t = t * curK3(i, u);
                    break;
                }
                default: break;
            }
            tmp += t;
        }
        QPoint* p = new QPoint(round(tmp.x()), round(tmp.y()));
        points.push_back(p);
    }
}

double Curve::curK1(int i, double u){
    double t = u - i;

    if (0 <= t && t < 1)
        return t;
    if (1 <= t && t < 2)
        return 2 - t;
    return 0;
}

double Curve::curK2(int i, double u){
    double t = u - i;

    if (0 <= t && t < 1)
        return 0.5 * t * t;
    if (1 <= t && t < 2)
        return 3 * t - t * t - 1.5;
    if (2 <= t && t < 3)
        return 0.5 * pow(3 - t, 2);
    return 0;
}

double Curve::curK3(int i, double u){
    double t = u - i;
    double a = 1.0 / 6;

    if (0 <= t && t < 1)
        return a * t * t * t;
    if (1 <= t && t < 2)
        return a * (-3 * pow(t - 1, 3) + 3 * pow(t - 1, 2) + 3 * (t - 1) + 1);
    if (2 <= t && t < 3)
        return a * (3 * pow(t - 2, 3) - 6 * pow(t - 2, 2) + 4);
    if (3 <= t && t < 4)
        return a * pow(4 - t, 3);
    return 0;
}
