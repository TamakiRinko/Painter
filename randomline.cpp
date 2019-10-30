#include "randomline.h"

RandomLine::RandomLine(): Graphics(DEFAULT_COLOR, DEFAULT_WIDTH){
}

RandomLine::RandomLine(QPoint startPoint, QColor color, int width): Graphics(color, width){
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
    line = new LineSegment(curPoint, nextPoint);
    curPoint = nextPoint;
}

void RandomLine::drawLogic(){
    points.append(line->getPoints());
}

bool RandomLine::isNotGraphics(){
    return points.size() < 1;
}
