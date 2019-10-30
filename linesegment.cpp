#include "linesegment.h"

LineSegment::LineSegment(QPoint startPoint, QColor color, int width): Graphics(color, width){
    this->startPoint = startPoint;
}

void LineSegment::setEndPoint(QPoint endPoint){
    this->endPoint = endPoint;
}

void LineSegment::drawLogic(){
    bresenHam();
}

bool LineSegment::isPoint(){
    return (startPoint == endPoint);
}

void LineSegment::bresenHam(){
    points.append(&startPoint);

    int flagX = 1;      //x增大or减小
    int flagY = 1;     //y增大or减小
    int dx = endPoint.x() - startPoint.x();
    int dy = endPoint.y() - startPoint.y();

    if(dx == 0){                                            //垂直线
        int direction = (dy > 0)?1:-1;                      //向上/下
        int index = direction;
        int tempX = startPoint.x();
        QPoint* tempPoint;
        while(index != dy){
            tempPoint = new QPoint(tempX, startPoint.y() + index);
            points.append(tempPoint);
            index += direction;
        }
        points.append(&endPoint);
        return;
    }


    QPoint* old = &startPoint;
    if(dx < 0){
        dx = -dx;
        flagX = -1;
    }
    if(dy < 0){         //y往下变大
        dy = -dy;
        flagY = -1;
    }
    if(dx >= dy){
        int di = 2 * dy - dx;
        do{
            QPoint* temp;
            if(di >= 0){
                temp = new QPoint(old->x() + flagX, old->y() + flagY);
                di = di + 2 * dy - 2 * dx;
            }else{
                temp = new QPoint(old->x() + flagX, old->y());
                di = di + 2 * dy;
            }
            points.append(temp);
            old = temp;
        }while(old->x() != endPoint.x() || old->y() != endPoint.y());
    }else if(dx < dy){
        int di = 2 * dx - dy;
        do{
            QPoint* temp;
            if(di >= 0){
                temp = new QPoint(old->x() + flagX, old->y() + flagY);
                di = di + 2 * dx - 2 * dy;
            }else{
                temp = new QPoint(old->x(), old->y() + flagY);
                di = di + 2 * dx;
            }
            points.append(temp);
            old = temp;
        }while(old->x() != endPoint.x()|| old->y() != endPoint.y());
    }
}
