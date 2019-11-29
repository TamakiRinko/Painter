#include "graphics.h"

Graphics::Graphics(int id, QColor c, int w): id(id), color(c), width(w), isErased(false){
    oldColor = color;
}

Graphics::Graphics(const Graphics& g){
    //id暂不考虑，QT版本不考虑id
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

int Graphics::getId(){
    return id;
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
    int px = point.x();
    int py = point.y();
    for(int i = 0; i < points.size(); ++i){

//        if(px == x && py == y)  return true;
//        if(px == x && py == y + 1)  return true;
//        if(px == x && py == y - 1)  return true;
//        if(px == x - 1 && py == y)  return true;
//        if(px == x - 1 && py == y + 1)  return true;
//        if(px == x - 1 && py == y - 1)  return true;
//        if(px == x + 1 && py == y)  return true;
//        if(px == x + 1 && py == y + 1)  return true;
//        if(px == x + 1 && py == y - 1)  return true;
        for(int j = width * -1 + 1; j <= width - 1; ++j){
            for(int k = width * -1 + 1; k <= width - 1; ++k){
                int x = points[i]->x() + j;
                int y = points[i]->y() + k;
                if(px == x && py == y){
                    return true;
                }
            }
        }
    }
    return false;
}

bool Graphics::pointIsInBlock(QPoint startPoint, QPoint endPoint){
    int xMin = startPoint.x() > endPoint.x() ? endPoint.x() : startPoint.x();
    int xMax = startPoint.x() < endPoint.x() ? endPoint.x() : startPoint.x();
    int yMin = startPoint.y() > endPoint.y() ? endPoint.y() : startPoint.y();
    int yMax = startPoint.y() < endPoint.y() ? endPoint.y() : startPoint.y();
    for(int i = 0; i < points.size(); ++i){
        if(points[i]->x() >= xMin && points[i]->x() <= xMax
                && points[i]->y() >= yMin && points[i]->y() <= yMax){
            return true;
        }
    }
    return false;
}

void Graphics::pointRotation(QPoint* movePoint, const QPoint* basePoint, int degree){
    double de = degree * PI / 180;

    int moveX = movePoint->x() - basePoint->x();
    int moveY = movePoint->y() - basePoint->y();
    int mX = round(moveX * cos(de) - moveY * sin(de));                //四舍五入！
    int mY = round(moveX * sin(de) + moveY * cos(de));
    movePoint->setX(mX + basePoint->x());
    movePoint->setY(mY + basePoint->y());
}

void Graphics::pointScale(QPoint* scalePoint, const QPoint* basePoint, double times){
    int scaleX = scalePoint->x() - basePoint->x();
    int scaleY = scalePoint->y() - basePoint->y();
    int sX = round(scaleX * times);
    int sY = round(scaleY * times);
    scalePoint->setX(sX + basePoint->x());
    scalePoint->setY(sY + basePoint->y());
}

int Graphics::regionCode(int x, int y){
    int xCode;
    int yCode;
    if(x < xMin){
        xCode = LEFT;
    }else if(x > xMax){
        xCode = RIGHT;
    }else{
        xCode = MIDDLE;
    }
    if(y < yMin){                           //y方向相反!
        yCode = UP;
    }else if(y > yMax){
        yCode = DOWN;
    }else{
        yCode = MIDDLE;
    }
    return xCode | yCode;
}
