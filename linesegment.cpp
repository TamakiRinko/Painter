#include "linesegment.h"

LineSegment::LineSegment(int id): Graphics(id, DEFAULT_COLOR, DEFAULT_WIDTH){
    mode = LINESEGMENT;
}

LineSegment::LineSegment(int id, QPoint startPoint, QColor color, int width, LineAlgorithm alg): Graphics(id, color, width){
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
LineSegment::LineSegment(int id, QPoint startPoint, QPoint endPoint, QColor color, int width, LineAlgorithm alg): Graphics(id, color, width){
    mode = LINESEGMENT;
    this->startPoint = startPoint;
    this->endPoint = endPoint;
    this->alg = alg;
    if(!isNotGraphics()){
        drawLogic();
    }
}

LineSegment::LineSegment(const LineSegment& l): Graphics (l){
    this->startPoint = l.startPoint;
    this->endPoint = l.endPoint;
    this->alg = l.alg;
    //重新指向startPoint和endPoint
    delete points[0];
    points[0] = &startPoint;
    delete points[points.size() - 1];
    points[points.size() - 1] = &endPoint;
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
//    startPoint.setX(startPoint.x()+ xOffset);
//    startPoint.setY(startPoint.y()+ yOffset);
//    endPoint.setX(endPoint.x() + xOffset);
//    endPoint.setY(endPoint.y() + yOffset);
}

void LineSegment::rotation(const QPoint* point, int degree){
    pointRotation(&startPoint, point, degree);
    pointRotation(&endPoint, point, degree);
    drawLogic();
}

void LineSegment::scale(const QPoint* point, double times){
    pointScale(&startPoint, point, times);
    pointScale(&endPoint, point, times);
    drawLogic();
}

bool LineSegment::crop(int xMin, int xMax, int yMin, int yMax, CropAlgorithm curAlg){
    this->xMin = xMin;
    this->xMax = xMax;
    this->yMin = yMin;
    this->yMax = yMax;
    if(curAlg == CropAlgorithm::CS){
        return CS();
    }else if(curAlg == CropAlgorithm::LB){
        return LB();
    }
    return true;
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
    points.push_back(&endPoint);
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

bool LineSegment::CS(){
    int x1 = startPoint.x();
    int y1 = startPoint.y();
    int x2 = endPoint.x();
    int y2 = endPoint.y();
    double k = x1 == x2 ? 0 : (y2 - y1) * 1.0 / (x2 - x1);      //斜率
    bool isVertical = (x1 == x2);
    int code1 = regionCode(x1, y1);                             //startPoint的区域码
    int code2 = regionCode(x2, y2);                             //endPoint的区域码

    while(true){
        if(code1 == MIDDLE && code2 == MIDDLE){                 //全部在区域内部
            break;
        }else if((code1 & code2) != 0){                         //全部在区域外，需要将图元删除，直接返回
            return false;
        }

        int code = code1 == MIDDLE ? code2 : code1;
        int xCrop = 0;                                          //裁剪后的坐标
        int yCrop = 0;
        if((code & LEFT) != 0){
            xCrop = xMin;
            yCrop = round((xCrop - x1) * k) + y1;
        }else if((code & RIGHT) != 0){
            xCrop = xMax;
            yCrop = round((xCrop - x1) * k) + y1;
        }else if((code & DOWN) != 0){
            yCrop = yMax;
            xCrop = isVertical ? x1 : round((yCrop - y1) / k) + x1;
        }else if((code & UP) != 0){
            yCrop = yMin;
            xCrop = isVertical ? x1 : round((yCrop - y1) / k) + x1;
        }

        if(code == code1){                                      //裁剪前部分
            x1 = xCrop;
            y1 = yCrop;
            code1 = regionCode(x1, y1);
        }else{                                                  //裁剪后部分
            x2 = xCrop;
            y2 = yCrop;
            code2 = regionCode(x2, y2);
        }
    }

    startPoint.setX(x1);
    startPoint.setY(y1);
    endPoint.setX(x2);
    endPoint.setY(y2);
    drawLogic();
    return true;
}

bool LineSegment::LB(){
    int x1 = startPoint.x();
    int y1 = startPoint.y();
    int x2 = endPoint.x();
    int y2 = endPoint.y();
    int p[5] = {0};
    int q[5] = {0};
    p[1] = x1 - x2;
    p[2] = x2 - x1;
    p[3] = y1 - y2;
    p[4] = y2 - y1;
    q[1] = x1 - xMin;
    q[2] = xMax - x1;
    q[3] = y1 - yMin;
    q[4] = yMax - y1;

    double ub = 0;
    double ue = 1;

    for(int i = 1; i < 5; ++i){
        if(p[i] == 0 && q[i] < 0){
            return false;
        }else if(p[i] == 0){
            continue;
        }
        double ui = q[i] * 1.0 / p[i];
        if(p[i] < 0){
            ub = ub > ui ? ub : ui;
            if(ub > ue){
                return false;
            }
        }else if(p[i] > 0){
            ue = ue > ui ? ui : ue;
            if(ub > ue){
                return false;
            }
        }
    }
    startPoint.setX(x1 + round(ub * (x2 - x1)));
    startPoint.setY(y1 + round(ub * (y2 - y1)));
    endPoint.setX(x1 + round(ue * (x2 - x1)));
    endPoint.setY(y1 + round(ue * (y2 - y1)));
    drawLogic();
    return true;
}
