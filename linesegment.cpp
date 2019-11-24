#include "linesegment.h"

LineSegment::LineSegment(): Graphics(DEFAULT_COLOR, DEFAULT_WIDTH){
    mode = LINESEGMENT;
}

LineSegment::LineSegment(QPoint startPoint, QColor color, int width, LineAlgorithm alg): Graphics(color, width){
    mode = LINESEGMENT;
    this->startPoint = startPoint;
    this->alg = alg;
}

/**
 * @brief LineSegment::LineSegment
 * @param startPoint
 * @param endPoint
 * @param color
 * @param width
 * 两个点已经给出，点集直接计算
 */
LineSegment::LineSegment(QPoint startPoint, QPoint endPoint, QColor color, int width, LineAlgorithm alg): Graphics(color, width){
    mode = LINESEGMENT;
    this->startPoint = startPoint;
    this->endPoint = endPoint;
    this->alg = alg;
    if(!isNotGraphics()){
        drawLogic();
    }
}

void LineSegment::setEndPoint(QPoint endPoint){
    this->endPoint = endPoint;
}

void LineSegment::drawLogic(){
    points.clear();
    if(alg == BRESENHAM){
        bresenHam();
    }else{
        DDA();
    }
}

bool LineSegment::isNotGraphics(){
    return (startPoint == endPoint);
}

void LineSegment::translation(int xOffset, int yOffset){
    for(int i = 0; i < points.size(); ++i){
        points[i]->setX(points[i]->x() + xOffset);
        points[i]->setY(points[i]->y() + yOffset);
    }
    startPoint.setX(startPoint.x()+ xOffset);
    startPoint.setY(startPoint.y()+ yOffset);
    endPoint.setX(endPoint.x() + xOffset);
    endPoint.setY(endPoint.y() + yOffset);
}

void LineSegment::rotation(const QPoint* point, int degree){
    pointRotation(&startPoint, point, degree);
    pointRotation(&endPoint, point, degree);
    drawLogic();
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

void LineSegment::DDA(){
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

    if(dx < 0){
        flagX = -1;
        dx = -dx;
    }
    if(dy < 0){         //y往下变大
        flagY = -1;
        dy = -dy;
    }

    double m = (double)dy / (double)dx;
    double k = 0;
    QPoint* temp = &startPoint;
    if(m > 1){                                          //Δy = 1，Δx = 1/m
        m = m * flagX;
        m = 1 / m;
        while(temp->y() != endPoint.y()){
            k += m;
            temp = new QPoint(int(startPoint.x() + k), temp->y() + flagY);
            points.append(temp);
        }
    }else{                                              //Δy = m，Δx = 1
        m = m * flagY;
        while(temp->x() != endPoint.x()){
            k += m;
            temp = new QPoint(temp->x() + flagX, int(startPoint.y() + k));
            points.append(temp);
        }
    }
    points.append(&endPoint);
    return;
}
