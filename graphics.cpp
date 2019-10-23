#include "graphics.h"

QColor& Graphics::getColor(){
    return color;
}

void Graphics::append(QPoint* point){
    points.append(point);
}

void Graphics::clear(){
    points.clear();
}

int Graphics::getNum(){
    return points.size();
}

QPoint& Graphics::operator[](int i){
    return *points[i];
}

QVector<QPoint* >::iterator Graphics::getBeginIterator(){
    return points.begin();
}

QVector<QPoint* >::iterator Graphics::getEndIterator(){
    return points.end();
}

void Graphics::setIsErased(bool b){
    isErased = b;
}

bool Graphics::getIsErased(){
    return isErased;
}
