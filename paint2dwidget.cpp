#include "paint2dwidget.h"

Paint2DWidget::Paint2DWidget(QWidget *parent) :
    QWidget(parent){
    curMode = NONE;
    curColor = DEFAULT_COLOR;
    curWidth = DEFAULT_WIDTH;
    curGraphics = nullptr;
    eraser = &Eraser::getInstance();
    isModified = false;
//    image = new QImage(this->size(), QImage::Format_ARGB32);
//    image->load(tr("D:/Myfolder/Desktop/11.bmp"));
}

Paint2DWidget::~Paint2DWidget(){

}

void Paint2DWidget::setMode(Mode mode){
    this->curMode = mode;
}

void Paint2DWidget::setColor(QColor color){
    this->curColor = color;
}

void Paint2DWidget::setWidth(int width){
    this->curWidth = width;
}

void Paint2DWidget::drawGraphics(QPainter& painter, Graphics* graphics){
    QPen pen;
    if(graphics == nullptr){
        return;
    }
//    graphics->clear();
//    graphics->drawLogic();
    pen.setColor(graphics->getColor());
    pen.setWidth(graphics->getWidth());
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
    sort(listTmp.begin(), listTmp.end());

    int offset = 0;
    for(int i: listTmp){
//        qDebug() << "删除的图形下标:" << i;
        graphicsList.erase(graphicsList.begin() + i - offset);      //移除该图形，已经调用析构函数
//        delete graphicsList[i - offset];                            //释放空间
        offset++;
    }
}

void Paint2DWidget::saveTo(QString fileName, const char* format){
    QImage image = QImage(this->width(),this->height(),QImage::Format_ARGB32);
    image.fill("white");
    QPainter painter(&image);
    for(int i = 0; i < graphicsList.size(); ++i){
        drawGraphics(painter, graphicsList[i]);
    }
    image.save(fileName, format);
    isModified = false;
}

bool Paint2DWidget::getIsModified(){
    return isModified;
}

/**
 * @brief Paint2DWidget::withDraw
 * 撤回，当为多边形时只撤回上一笔
 */
void Paint2DWidget::withDraw(){
    if(curMode == POLYGON && curGraphics != nullptr){       //可能该多边形恰巧画完
        Polygon* curPolygon = (Polygon* )curGraphics;
        curPolygon->withDraw();
    }else{
        if(!graphicsList.isEmpty()){
            graphicsList.pop_back();
        }
    }
    update();
}


void Paint2DWidget::paintEvent(QPaintEvent*){
    QPainter painter(this);
//    painter.drawImage(0, 0, *image);
    drawGraphics(painter, curGraphics);
    for(int i = 0; i < graphicsList.size(); ++i){
        drawGraphics(painter, graphicsList[i]);
    }
}

/**
 * @brief PaintWindow::mousePressEvent
 * @param e
 * 鼠标按下，新建对应图形，画出当前点
 */
void Paint2DWidget::mousePressEvent(QMouseEvent* e){
    QPoint point(e->x(), e->y());
    switch (curMode) {
        case LINESEGMENT:{
            curGraphics = new LineSegment(point, curColor, curWidth);     //当前为线段
            break;
        }
        case RANDOMLINE:{
            curGraphics = new RandomLine(point, curColor, curWidth);      //当前为随机线
            break;
        }
        case CIRCLE:{
            curGraphics = new Circle(point, curColor, curWidth);          //当前为圆
            break;
        }
        case ELLIPSE:{
            curGraphics = new Ellipse(point, curColor, curWidth);         //当前为椭圆
            break;
        }
        case ERASER:{
            QPoint* p = new QPoint(point.x(), point.y());                 //当前为橡皮擦
            eraser->append(p);
            break;
        }
        default: break;
    }
    isModified = true;
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
            if(!curLine->isNotGraphics()){                                      //可能只是一个点
                curGraphics->drawLogic();
                graphicsList.append(curGraphics);                               //加入已有图形列表
            }
            curGraphics = nullptr;
            break;
        }
        case RANDOMLINE:{
            RandomLine* curRandomLine = (RandomLine* )curGraphics;
            curRandomLine->setPoint(point);
            if(!curRandomLine->isNotGraphics()){
                curGraphics->drawLogic();
                graphicsList.append(curGraphics);
            }
            curGraphics = nullptr;
            break;
        }
        case POLYGON:{      //对于多边形，只考虑鼠标释放
            if(curGraphics == nullptr){
                curGraphics = new Polygon(point, curColor, curWidth);   //新建多边形
            }else{
                Polygon* curPolygon = (Polygon* )curGraphics;
                curPolygon->setNextPoint(point);
                if(e->button() == Qt::RightButton){                     //右键点击，结束绘制
                    curPolygon->complete();
                    if(!curGraphics->isNotGraphics()){
                        curGraphics->drawLogic();
                        graphicsList.append(curGraphics);
                    }
                    curGraphics = nullptr;
                }else{                                                  //左键点击，继续绘制
                    curGraphics->drawLogic();
                }
            }
            break;
        }
        case CIRCLE:{
            Circle* curCircle = (Circle* )curGraphics;
            curCircle->setPoint(point);
            if(!curCircle->isNotGraphics()){
                curGraphics->drawLogic();
                graphicsList.append(curGraphics);
            }
            curGraphics = nullptr;
            break;
        }
        case ELLIPSE:{
            Ellipse* curEllipse = (Ellipse* )curGraphics;
            curEllipse->setPoint(point);
            if(!curEllipse->isNotGraphics()){
                curGraphics->drawLogic();
                graphicsList.append(curGraphics);
            }
            curGraphics = nullptr;
            break;
        }
        case ERASER:{                   //释放时删除
            QPoint* p = new QPoint(point.x(), point.y());
            eraser->append(p);
            eraseGraphics();
            eraser->clear();
            curGraphics = nullptr;
            break;
        }
        default: break;
    }
    isModified = true;
    update();
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
            curGraphics->drawLogic();
            update();
            break;
        }
        case RANDOMLINE:{
            RandomLine* curRandomLine = (RandomLine* )curGraphics;
            curRandomLine->setPoint(point);
            curGraphics->drawLogic();
            update();
            break;
        }
        case CIRCLE:{
            Circle* curCircle = (Circle* )curGraphics;
            curCircle->setPoint(point);
            curGraphics->drawLogic();
            update();
            break;
        }
        case ELLIPSE:{
            Ellipse* curEllipse = (Ellipse* )curGraphics;
            curEllipse->setPoint(point);
            curGraphics->drawLogic();
            update();
            break;
        }
        case ERASER:{
            QPoint* p = new QPoint(point.x(), point.y());
            eraser->append(p);
            break;
        }
        default: break;
    }
    isModified = true;
}
