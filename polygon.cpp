#include "polygon.h"

Polygon::Polygon(): Graphics(DEFAULT_COLOR, DEFAULT_WIDTH){

}

Polygon::Polygon(QPoint startPoint, QColor color, int width): Graphics(color, width){
    this->startPoint = new QPoint(startPoint);
//    this->curPoint = startPoint;
    vertexList.append(this->startPoint);
    num = 1;
}

void Polygon::setNextPoint(QPoint nextPoint){
    if(nextPoint.x() == vertexList[num - 1]->x() && nextPoint.y() == vertexList[num - 1]->y()){
        return;
    }
    QPoint* next = new QPoint(nextPoint);
    vertexList.append(next);
    num++;
    LineSegment* line = new LineSegment(*(vertexList[num - 2]), *(vertexList[num - 1]));
    lineList.append(line);
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
    LineSegment* line = new LineSegment(*(vertexList[0]), *(vertexList[num - 1]));
    lineList.append(line);
}
