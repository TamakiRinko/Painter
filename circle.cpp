#include "circle.h"

Circle::Circle(int id): Graphics(id, DEFAULT_COLOR, DEFAULT_WIDTH){
    radius = 0;
    mode = CIRCLE;
}

Circle::Circle(int id, QPoint center, QColor color, int width): Graphics(id, color, width){
    this->center = center;
    this->backCenter = center;
    radius = 0;
    mode = CIRCLE;
}

void Circle::setPoint(QPoint point){
    outPoint = point;
    radius = (int)(sqrtf((outPoint.x() - center.x()) * (outPoint.x() - center.x()) +
                   (outPoint.y() - center.y()) * (outPoint.y() - center.y())));
    backOutPoint = outPoint;
    backRadius = radius;
}

bool Circle::isNotGraphics(){
    return (center.x() == outPoint.x() && center.y() == outPoint.y());
}

void Circle::drawLogic(){
    points.clear();
    mid();
}

void Circle::translation(int xOffset, int yOffset){
    for(int i = 0; i < points.size(); ++i){
        points[i]->setX(points[i]->x() + xOffset);
        points[i]->setY(points[i]->y() + yOffset);
    }
    center.setX(center.x()+ xOffset);
    center.setY(center.y()+ yOffset);
    outPoint.setX(outPoint.x() + xOffset);
    outPoint.setY(outPoint.y() + yOffset);
}

void Circle::rotation(const QPoint* point, int degree){
    pointRotation(&center, point, degree);
    pointRotation(&outPoint, point, degree);
    drawLogic();
}

void Circle::scale(const QPoint* point, double times){
    pointScale(&center, point, times);
    pointScale(&outPoint, point, times);
    setPoint(outPoint);
    drawLogic();
}

void Circle::mid(){
    int di = 3 - 2 * radius;
    int xi = 0;
    int yi = -radius;
    int x_off = center.x();
    int y_off = center.y();

    QPoint* startPoint = new QPoint(x_off + xi, y_off + yi);
    points.append(startPoint);
    while(xi + yi <= 0){
        if(di >= 0){
            di = di + 4 * xi + 4 * yi + 10;
            xi = xi + 1;
            yi = yi + 1;
        }else{
            di = di + 4 * xi + 6;
            xi = xi + 1;
        }
        //对称的8个点
        QPoint* temp1 = new QPoint(x_off + xi, y_off + yi);
        QPoint* temp2 = new QPoint(x_off + yi, y_off + xi);
        QPoint* temp3 = new QPoint(x_off + xi, y_off - yi);
        QPoint* temp4 = new QPoint(x_off + yi, y_off - xi);
        QPoint* temp5 = new QPoint(x_off - xi, y_off - yi);
        QPoint* temp6 = new QPoint(x_off - yi, y_off - xi);
        QPoint* temp7 = new QPoint(x_off - yi, y_off + xi);
        QPoint* temp8 = new QPoint(x_off - xi, y_off + yi);
        points.append(temp1);
        points.append(temp2);
        points.append(temp3);
        points.append(temp4);
        points.append(temp5);
        points.append(temp6);
        points.append(temp7);
        points.append(temp8);
    }
}
