#include "paint2dwidget.h"

Paint2DWidget::Paint2DWidget(QWidget *parent) :
    QWidget(parent){
    curMode = NONE;
    curColor = DEFAULT_COLOR;
    curGraphics = nullptr;
    eraser = &Eraser::getInstance();
}

Paint2DWidget::~Paint2DWidget(){

}

void Paint2DWidget::setMode(Mode mode){
    this->curMode = mode;
}

void Paint2DWidget::setColor(QColor color){
    this->curColor = color;
}

void Paint2DWidget::drawGraphics(Graphics* graphics){
    QPainter painter(this);
    QPen pen;
    if(graphics == nullptr){
        return;
    }
    graphics->clear();
    graphics->drawLogic();
    pen.setColor(graphics->getColor());
    painter.setPen(pen);
    for(int i = 0; i < graphics->getNum(); ++i){
        painter.drawPoint((*graphics)[i]);
    }
}

void Paint2DWidget::eraseGraphics(){
    QSet<int> eraserSet;
    for(int i = 0; i < eraser->getNum(); ++i){
        for(int j = 0; j < graphicsList.size(); ++j){
            if(graphicsList[j]->getIsErased()){                     //已经被删除
                continue;
            }
            for(int k = 0; k < graphicsList[j]->getNum(); ++k){
                if(eraser->pointIsIn(i, (*(graphicsList)[j])[k])){
                    eraserSet.insert(j);
                    graphicsList[j]->setIsErased(true);
                    break;
                }
            }
        }
    }
    //对set由小到大排序
    QList<int> listTmp = QList<int>::fromSet(eraserSet);
    qSort(listTmp);

    int offset = 0;
    for(int i: listTmp){
//        qDebug() << "删除的图形下标:" << i;
        graphicsList.erase(graphicsList.begin() + i - offset);
        offset++;
    }
}


void Paint2DWidget::paintEvent(QPaintEvent*){
    drawGraphics(curGraphics);
    for(int i = 0; i < graphicsList.size(); ++i){
        drawGraphics(graphicsList[i]);
    }
}

//void PaintWindow::keyPressEvent(QKeyEvent* e){
//    switch (curMode) {
//        case LINESEGMENT:{

//            break;
//        }
//        case CIRCLE:{
//            break;
//        }
//        case ELLIPSE:{
//            break;
//        }
//        default: break;
//    }
//}

/**
 * @brief PaintWindow::mousePressEvent
 * @param e
 * 鼠标按下，新建对应图形，画出当前点
 */
void Paint2DWidget::mousePressEvent(QMouseEvent* e){
    QPoint point(e->x(), e->y());
    switch (curMode) {
        case LINESEGMENT:{
            curGraphics = new LineSegment(point, curColor);     //当前为线段
            break;
        }
        case CIRCLE:{
            curGraphics = new Circle(point, curColor);          //当前为圆
            break;
        }
        case ELLIPSE:{
            break;
        }
        case ERASER:{
            QPoint* p = new QPoint(point.x(), point.y());
            eraser->append(p);
            break;
        }
        default: break;
    }
//    //画出该点
//    pen.setColor(curColor);
//    painter.setPen(pen);
//    painter.drawPoint(point);
}

/**
 * @brief PaintWindow::mouseReleaseEvent
 * @param e
 * 鼠标释放，对象新建结束，将对象保存起来
 */
void Paint2DWidget::mouseReleaseEvent(QMouseEvent* e){
    QPoint point(e->x(), e->y());
    switch (curMode) {
        case LINESEGMENT:{
            LineSegment* curLine = (LineSegment* )curGraphics;
            curLine->setEndPoint(point);
            if(!curLine->isPoint()){                                    //可能只是一个点
                graphicsList.append(curGraphics);                       //加入已有图形列表
            }
            break;
        }
        case CIRCLE:{
            Circle* curCircle = (Circle* )curGraphics;
            curCircle->setPoint(point);
            if(!curCircle->isPoint()){
                graphicsList.append(curGraphics);
            }
            break;
        }
        case ELLIPSE:{
            break;
        }
        case ERASER:{                   //释放时删除
            QPoint* p = new QPoint(point.x(), point.y());
            eraser->append(p);
            eraseGraphics();
            eraser->clear();
            update();
            break;
        }
        default: break;
    }
    curGraphics = nullptr;
}

/**
 * @brief PaintWindow::mouseMoveEvent
 * @param e
 * 鼠标移动，动态变化对象，实时画出
 */
void Paint2DWidget::mouseMoveEvent(QMouseEvent* e){
    QPoint point(e->x(), e->y());
    switch (curMode) {
        case LINESEGMENT:{
            LineSegment* curLine = (LineSegment* )curGraphics;
            curLine->setEndPoint(point);
            update();
            break;
        }
        case CIRCLE:{
            Circle* curCircle = (Circle* )curGraphics;
            curCircle->setPoint(point);
            update();
            break;
        }
        case ELLIPSE:{
            break;
        }
        case ERASER:{
            QPoint* p = new QPoint(point.x(), point.y());
            eraser->append(p);
            break;
        }
        default: break;
    }
}
