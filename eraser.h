#ifndef ERASER_H
#define ERASER_H

#include "graphics.h"

/**
 * @brief The Eraser class
 * 橡皮擦，单例模式
 */
class Eraser{
public:
    static Eraser& getInstance(){
        static Eraser eraser;
        return eraser;
    }
    void append(QPoint* point);
    int getNum();
    void clear();
    bool pointIsIn(int i, QPoint& point);   //point在points[i]所在的正方形范围中
    QPoint& operator[](int i);
private:
    Eraser();
    Eraser(const Eraser&);                  //拷贝构造声明
    Eraser& operator=(const Eraser&);       //赋值重载声明
    QVector<QPoint*> points;                 //橡皮擦包含的点集
};

#endif // ERASER_H
