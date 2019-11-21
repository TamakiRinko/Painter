#include "graphics.h"

Graphics::Graphics(QColor c, int w): color(c), width(w), isErased(false){
    oldColor = color;
}

Graphics::Graphics(const Graphics& g){
    this->color = g.color;
    this->width = g.width;
    this->isErased = g.isErased;
    this->oldColor = g.oldColor;
    this->mode = g.mode;
    for(int i = 0; i < g.points.size(); ++i){
        QPoint* point = new QPoint(g.points[i]->x(), g.points[i]->y());
        this->points.push_back(point);
    }
}

Graphics::~Graphics(){
    for(int i = 0; i < points.size(); ++i){
        delete points[i];
    }
    points.clear();
}

QColor& Graphics::getColor(){
    return color;
}

void Graphics::setColor(QColor c){
    if(oldColor == color){
        oldColor = color;
        color = c;
    }
}

void Graphics::resetColor(){
    color = oldColor;
}

int Graphics::getWidth(){
    return width;
}

const QVector<QPoint* >& Graphics::getPoints() const{
    return points;
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

//QVector<QPoint* >::iterator Graphics::getBeginIterator(){
//    return points.begin();
//}

//QVector<QPoint* >::iterator Graphics::getEndIterator(){
//    return points.end();
//}

void Graphics::setIsErased(bool b){
    isErased = b;
}

bool Graphics::getIsErased(){
    return isErased;
}

Mode Graphics::getMode(){
    return mode;
}

bool Graphics::pointIsIn(QPoint point){
    for(int i = 0; i < points.size(); ++i){
        if(point.x() == points[i]->x() && point.y() == points[i]->y()){
            return true;
        }
    }
    return false;
}
