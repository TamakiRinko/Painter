#include "eraser.h"

Eraser::Eraser(){

}

void Eraser::append(QPoint* point){
    points.append(point);
}

int Eraser::getNum(){
    return points.size();
}

QPoint& Eraser::operator[](int i){
    return *(points[i]);
}

void Eraser::clear(){
    points.clear();
}

bool Eraser::pointIsIn(int i, QPoint& point){
    int x = points[i]->x();
    int y = points[i]->y();
    int px = point.x();
    int py = point.y();
    if(px == x && py == y)  return true;
    if(px == x && py == y + 1)  return true;
    if(px == x && py == y - 1)  return true;
    if(px == x - 1 && py == y)  return true;
    if(px == x - 1 && py == y + 1)  return true;
    if(px == x - 1 && py == y - 1)  return true;
    if(px == x + 1 && py == y)  return true;
    if(px == x + 1 && py == y + 1)  return true;
    if(px == x + 1 && py == y - 1)  return true;
    return false;
}
