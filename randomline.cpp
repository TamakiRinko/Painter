#include "randomline.h"

RandomLine::RandomLine(int id): Graphics(id, DEFAULT_COLOR, DEFAULT_WIDTH){
    mode = RANDOMLINE;
}

RandomLine::RandomLine(int id, QPoint startPoint, QColor color, int width): Graphics(id, color, width){
    mode = RANDOMLINE;
    QPoint* p = new QPoint(startPoint);
    points.append(p);
    this->curPoint = startPoint;
}

/**
 * @brief RandomLine::setPoint
 * @param nextPoint
 * 每两点间画线
 */
void RandomLine::setPoint(QPoint nextPoint){
    line = new LineSegment(id, curPoint, nextPoint);
    curPoint = nextPoint;
}

void RandomLine::drawLogic(){
    points.append(line->getPoints());
}

bool RandomLine::isNotGraphics(){
    return points.size() < 1;
}

void RandomLine::translation(int xOffset, int yOffset){
    curPoint.setX(curPoint.x() + xOffset);
    curPoint.setY(curPoint.y() + yOffset);
    for(int i = 0; i < points.size(); ++i){
        points[i]->setX(points[i]->x() + xOffset);
        points[i]->setY(points[i]->y() + yOffset);
    }
}

void RandomLine::rotation(const QPoint* point, int degree){

}

void RandomLine::scale(const QPoint* point, double times){

}
