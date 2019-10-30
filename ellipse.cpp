#include "ellipse.h"

Ellipse::Ellipse(){
    rx = ry = 0;
}

Ellipse::Ellipse(QPoint center, QColor color, int width): Graphics(color, width){
    this->center = center;
    rx = ry = 0;
}

void Ellipse::setPoint(QPoint point){
    outPoint = point;
    rx = abs(outPoint.x() - center.x());
    ry = abs(outPoint.y() - center.y());
}

bool Ellipse::isPoint(){
    return (center.x() == outPoint.x() && center.y() == outPoint.y());
}

void Ellipse::drawLogic(){
    mid();
}

void Ellipse::mid(){
    int rx2 = rx * rx;
    int ry2 = ry * ry;
    int di = ry2 - rx2 * ry + rx2 / 4;
    int xi = 0;
    int yi = ry;
    int x_off = center.x();
    int y_off = center.y();

    if(yi == 0){                                                        //yi == 0，特殊考虑
        for(int i = 0; i < rx; ++i){
            QPoint* tempPoint = new QPoint(x_off + i, y_off);
            QPoint* tempPoint2 = new QPoint(x_off - i, y_off);
            points.append(tempPoint);
            points.append(tempPoint2);
        }
        return;
    }

    QPoint* startPoint = new QPoint(x_off + xi, y_off + yi);
    points.append(startPoint);
    while(ry2 * xi <= rx2 * yi){
        QPoint* temp1;
        if(di >= 0){
            di = di + 2 * ry2 * xi + 2 * ry2 - 2 * rx2 * yi + 2 * rx2 + ry2;
            xi = xi + 1;
            yi = yi - 1;
            temp1 = new QPoint(x_off + xi, y_off + yi);
        }else{
            di = di + 2 * ry2 * xi + 2 * ry2 + ry2;
            xi = xi + 1;
            temp1 = new QPoint(x_off + xi, y_off + yi);
        }
        //对称的8个点
        QPoint* temp2 = new QPoint(x_off - xi, y_off - yi);
        QPoint* temp3 = new QPoint(x_off + xi, y_off - yi);
        QPoint* temp4 = new QPoint(x_off - xi, y_off + yi);
        points.append(temp1);
        points.append(temp2);
        points.append(temp3);
        points.append(temp4);
    }
    di = ry2 * (xi + 1/2) * (xi + 1/2) + rx2 * (yi - 1) * (yi - 1) - rx2 * ry2;
    while(yi >= 0){
        QPoint* temp1;
        if(di >= 0){
            di = di - 2 * rx2 * yi + 2 * rx2 + rx2;
            yi = yi - 1;
            temp1 = new QPoint(x_off + xi, y_off + yi);
        }else{
            di = di + 2 * ry2 * xi + 2 * ry2 - 2 * rx2 * yi + 2 * rx2 + rx2;
            xi = xi + 1;
            yi = yi - 1;
            temp1 = new QPoint(x_off + xi, y_off + yi);
        }
        //对称的8个点
        QPoint* temp2 = new QPoint(x_off - xi, y_off - yi);
        QPoint* temp3 = new QPoint(x_off + xi, y_off - yi);
        QPoint* temp4 = new QPoint(x_off - xi, y_off + yi);
        points.append(temp1);
        points.append(temp2);
        points.append(temp3);
        points.append(temp4);
    }
}
