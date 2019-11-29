#include "polygon.h"

Polygon::Polygon(): Graphics(DEFAULT_COLOR, DEFAULT_WIDTH){
    mode = POLYGON;
}

Polygon::Polygon(const Polygon& p): Graphics (p){
    mode = POLYGON;
    num = p.num;
    alg = p.alg;
    startPoint = new QPoint(p.startPoint->x(), p.startPoint->y());
    for(int i = 0; i < p.vertexList.size(); ++i){
        QPoint* point = new QPoint(p.vertexList[i]->x(), p.vertexList[i]->y());
        vertexList.append(point);
    }
    for(int i = 0; i < p.lineList.size(); ++i){
        LineSegment* line = new LineSegment(*(p.lineList[i]));                //默认拷贝构造即可
        lineList.append(line);
    }
}

Polygon::Polygon(QPoint startPoint, QColor color, int width, LineAlgorithm alg): Graphics(color, width){
    mode = POLYGON;
    this->startPoint = new QPoint(startPoint);
//    this->curPoint = startPoint;
    vertexList.append(this->startPoint);
    num = 1;
    this->alg = alg;
}

Polygon::~Polygon(){
    delete startPoint;
    //顶点移动
    for(int i = 0; i < vertexList.size(); ++i){
        delete vertexList[i];
    }
    vertexList.clear();
    //边移动
    for(int i = 0; i < lineList.size(); ++i){
        delete lineList[i];
    }
    lineList.clear();
}

void Polygon::setNextPoint(QPoint nextPoint){
    if(nextPoint.x() == vertexList[num - 1]->x() && nextPoint.y() == vertexList[num - 1]->y()){
        return;
    }
    QPoint* next = new QPoint(nextPoint);
    vertexList.append(next);
    num++;
    LineSegment* line = new LineSegment(*(vertexList[num - 2]), *(vertexList[num - 1]), color, width, alg);
    lineList.append(line);
}

/**
 * @brief Polygon::withDraw
 * 撤回至上一笔
 */
void Polygon::withDraw(){
    if(!lineList.isEmpty()){
        delete lineList[lineList.size() - 1];
        lineList.pop_back();
    }
    if(num > 1){
        delete vertexList[vertexList.size() - 1];
        vertexList.pop_back();
        num--;
    }
    drawLogic();
}

/**
 * @brief Polygon::drawLogic
 * 只要附上所有的边集的点即可
 */
void Polygon::drawLogic(){
    points.clear();
    for(int i = 0; i < lineList.size(); ++i){
        points.append(lineList[i]->getPoints());
    }
}

/**
 * @brief Polygon::isPoint
 * @return
 * 至少三条边才构成一个多边形
 */
bool Polygon::isNotGraphics(){
    return (lineList.size() <= 2);
}

/**
 * @brief Polygon::complete
 * 首尾相连，结束绘制
 */
void Polygon::complete(){
    LineSegment* line = new LineSegment(*(vertexList[0]), *(vertexList[num - 1]), color, width, alg);
    lineList.append(line);
}

void Polygon::translation(int xOffset, int yOffset){
    startPoint->setX(startPoint->x() + xOffset);
    startPoint->setY(startPoint->y() + yOffset);
    //顶点移动
    for(int i = 0; i < vertexList.size(); ++i){
        vertexList[i]->setX(vertexList[i]->x() + xOffset);
        vertexList[i]->setY(vertexList[i]->y() + yOffset);
    }
    //边移动
    for(int i = 0; i < lineList.size(); ++i){
        lineList[i]->translation(xOffset, yOffset);
    }
}

void Polygon::rotation(const QPoint* point, int degree){
    pointRotation(startPoint, point, degree);
    //顶点旋转
    for(int i = 0; i < vertexList.size(); ++i){
        pointRotation(vertexList[i], point, degree);
    }
    //边旋转
    for(int i = 0; i < lineList.size(); ++i){
        lineList[i]->rotation(point, degree);
    }
    drawLogic();
}

void Polygon::scale(const QPoint* point, double times){
    pointScale(startPoint, point, times);
    //顶点缩放
    for(int i = 0; i < vertexList.size(); ++i){
        pointScale(vertexList[i], point, times);
    }
    //边缩放
    for(int i = 0; i < lineList.size(); ++i){
        lineList[i]->scale(point, times);
    }
    drawLogic();
}

