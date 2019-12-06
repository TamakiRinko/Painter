#include "paint2dwidget.h"

long long combine[maxCurveNum][maxCurveNum];
double powerU[stepsNum][maxCurveNum];

Paint2DWidget::Paint2DWidget(const char* fileName, QWidget *parent) :
    QWidget(parent){
    curId = 0;
    curMode = NONE;
    curColor = DEFAULT_COLOR;
    curWidth = DEFAULT_WIDTH;
    curLineAlg = BRESENHAM;
    curCropAlg = CS;
    curCurveAlg = BEZIER;
    curK = 3;
    curGraphics = nullptr;
    curTransformGraphics = nullptr;
    rotatePoint = nullptr;
    scalePoint = nullptr;
    curControlPointIndex = -1;
    eraser = &Eraser::getInstance();
    isModified = false;
    hasSelected = false;

    //预处理组合数
    for(int i = 0; i < maxCurveNum; ++i){
        combine[0][i] = 0;
        combine[i][0] = 1;
    }
    for(int i = 1; i < maxCurveNum; ++i){
        for(int j = 1; j < maxCurveNum; ++j){
            combine[i][j] = combine[i-1][j] + combine[i-1][j-1];
        }
    }
//    clock_t startTime,endTime;
//    startTime = clock();
    //预处理幂
    double step = 1.0 / stepsNum;
    for(int i = 0; i < stepsNum; ++i){
        double u = i * step;
        for(int j = 0; j < maxCurveNum; ++j){
            powerU[i][j] = pow(u, j);
        }
    }
//    endTime = clock();
//    qDebug() << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s";

//    for(int i = 0; i < maxCurveNum; ++i){
//        for(int j = 0; j < maxCurveNum; ++j){
//            cout << combine[i][j] << ", ";
//        }
//        cout << endl;
//    }

    if(fileName != nullptr){
        insertMap();
        fout.open("debug.txt", ios::out);                   //调试用
        fin.open(fileName, ios::in);
        if(!fin.is_open() || !fout.is_open()){
            return;
        }
        string order;
        fin >> order;
        while(!fin.fail()){
            (this->*(commandMap[order]))();
            fin >> order;
        }
    }
}

Paint2DWidget::~Paint2DWidget(){

}

void Paint2DWidget::setMode(Mode mode){
    if(curMode == POLYGON && curGraphics != nullptr){                                 //多边形未画完便改变Mode
        delete curGraphics;
        curGraphics = nullptr;
    }else if(curMode == CURVE && curGraphics != nullptr){
        delete curGraphics;
        curGraphics = nullptr;
        curveList.clear();
        curControlPointIndex = -1;
    }
    this->curMode = mode;
    if(curMode != TRANSLATION && curMode != ROTATION && curMode != SELECT
            && curMode != SCALE && curMode != SELECTBOLCK && curMode != CROP){
        clearList(&transformGraphicsList);
    }
    if(curMode != ROTATION && rotatePoint != nullptr){          //旋转基准点
        delete rotatePoint;
        rotatePoint = nullptr;
    }
    if(curMode != SCALE && scalePoint != nullptr){              //缩放基准点
        delete scalePoint;
        scalePoint = nullptr;
    }

    update();
}

void Paint2DWidget::setColor(QColor color){
    this->curColor = color;
}

void Paint2DWidget::setWidth(int width){
    this->curWidth = width;
}

void Paint2DWidget::setLineAlgorithm(LineAlgorithm alg){
    curLineAlg = alg;
}

void Paint2DWidget::setCropAlgorithm(CropAlgorithm alg){
    curCropAlg = alg;
}

void Paint2DWidget::setCurveAlgorithm(CurveAlgorithm alg){
    curCurveAlg = alg;
}

void Paint2DWidget::setK(int k){
    curK = k;
    if(dynamic_cast<Curve* >(curGraphics)){
        Curve* curCurve = (Curve* )curGraphics;
        curCurve->setCurK(k);
    }
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

void Paint2DWidget::drawPoint(QPainter& painter, QPoint* point){
    QPen pen;
    if(point == nullptr){
        return;
    }
    pen.setColor(Qt::red);
    pen.setWidth(5);
    painter.setPen(pen);
    painter.drawPoint(*point);
}

void Paint2DWidget::drawList(QPainter& painter, QVector<QPoint* >* list, int width){
    if(list->empty()){
        return;
    }
    QPen pen;
    pen.setColor(Qt::blue);
    pen.setWidth(width);
    painter.setPen(pen);
    for(int i = 0; i < list->size(); ++i){
        painter.drawPoint(*(*list)[i]);
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
        qDebug() << "删除的图形下标:" << i;
        delete graphicsList[i - offset];                            //释放空间
        graphicsList.erase(graphicsList.begin() + i - offset);      //移除该图形
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
    }else if(curMode == CURVE && curGraphics != nullptr){
        Curve* curCurve = (Curve* )curGraphics;
        curCurve->withDraw();
        if(curveList.size() > 1){
            delete curveList[curveList.size() - 1];
            curveList.pop_back();
        }
    }else{
        if(!graphicsList.isEmpty()){
            delete graphicsList[graphicsList.size() - 1];
            graphicsList.pop_back();
        }
    }
    update();
}

void Paint2DWidget::translation(QPoint start, QPoint end){
    int xOffset = end.x() - start.x();
    int yOffset = end.y() - start.y();
//    qDebug() << "xOff: " << xOffset << ", yOff: " << yOffset << endl;
    for(int i = 0; i < transformGraphicsList.size(); ++i){
        transformGraphicsList[i]->translation(xOffset, yOffset);
    }
}

void Paint2DWidget::clearList(QVector<Graphics* >* list){
    for(int i = 0; i < list->size(); ++i){
        (*list)[i]->resetColor();
    }
    list->clear();
}

void Paint2DWidget::setListColor(QVector<Graphics* >* list){
    for(int i = 0; i < list->size(); ++i){
        if((*list)[i]->getColor() == Qt::red){
            (*list)[i]->setColor(Qt::blue);
        }else{
            (*list)[i]->setColor(Qt::red);
        }
    }
}

void Paint2DWidget::rectangleCalculate(QPoint& startPoint, QPoint& endPoint){
    rectangleList.clear();
    int xMin = startPoint.x() > endPoint.x() ? endPoint.x() : startPoint.x();
    int xMax = startPoint.x() < endPoint.x() ? endPoint.x() : startPoint.x();
    int yMin = startPoint.y() > endPoint.y() ? endPoint.y() : startPoint.y();
    int yMax = startPoint.y() < endPoint.y() ? endPoint.y() : startPoint.y();
    for(int j = yMin; j < yMax; ++j){
        QPoint* p = new QPoint(xMin, j);
        rectangleList.push_back(p);
    }
    for(int j = yMin; j < yMax; ++j){
        QPoint* p = new QPoint(xMax, j);
        rectangleList.push_back(p);
    }
    for(int j = xMin; j < xMax; ++j){
        QPoint* p = new QPoint(j, yMin);
        rectangleList.push_back(p);
    }
    for(int j = xMin; j < xMax; ++j){
        QPoint* p = new QPoint(j, yMax);
        rectangleList.push_back(p);
    }
}

/**
 * @brief Paint2DWidget::selectControlPoint
 * @param point
 * 找到对应的控制点，用于后续调整
 */
bool Paint2DWidget::selectControlPoint(QPoint point){
    int px = point.x();
    int py = point.y();

    for(int i = 0; i < curveList.size(); ++i){
        for(int j = 5 * -1 + 1; j <= 5 - 1; ++j){
            for(int k = 5 * -1 + 1; k <= 5 - 1; ++k){
                int x = curveList[i]->x() + j;
                int y = curveList[i]->y() + k;
                if(px == x && py == y){
                    curControlPointIndex = i;
                    return true;
                }
            }
        }
    }
    curControlPointIndex = -1;
    return false;
}

void Paint2DWidget::translationPoint(QPoint* point, QPoint start, QPoint end){
    int xOffset = end.x() - start.x();
    int yOffset = end.y() - start.y();
    point->setX(point->x() + xOffset);
    point->setY(point->y() + yOffset);
}

void Paint2DWidget::graphicsCopy(){
    if(transformGraphicsList.size() != 0 && (curMode == SELECT || curMode == SELECTBOLCK) && hasSelected){
        copyGraphicsList.clear();
        for(int i = 0; i < transformGraphicsList.size(); ++i){
            Graphics* copy;
            switch (transformGraphicsList[i]->getMode()) {
                case LINESEGMENT:{
                    copy = new LineSegment(*(LineSegment*)transformGraphicsList[i]);
                    break;
                }
                case RANDOMLINE:{
                    copy = new RandomLine(*(RandomLine*)transformGraphicsList[i]);
                    break;
                }
                case CIRCLE:{
                    copy = new Circle(*(Circle*)transformGraphicsList[i]);
                    break;
                }
                case ELLIPSE:{
                    copy = new Ellipse(*(Ellipse*)transformGraphicsList[i]);
                    break;
                }
                case POLYGON:{
                    copy = new Polygon(*(Polygon*)transformGraphicsList[i]);
                    break;
                }
                default: copy = nullptr; break;
            }
            if(copy != nullptr){
                copyGraphicsList.push_back(copy);
            }
        }
    }
}

void Paint2DWidget::graphicsPaste(){
    if(copyGraphicsList.size() != 0 && (curMode == SELECT || curMode == SELECTBOLCK)){
        clearList(&transformGraphicsList);
//        setListColor(&copyGraphicsList);
        transformGraphicsList = copyGraphicsList;
        QPoint p(0, 0);
        QPoint q(10, 10);
        translation(p, q);          //平移三格，用以区分
        for(int i = 0; i < copyGraphicsList.size(); ++i){
            graphicsList.push_back(copyGraphicsList[i]);
        }
        curMode = TRANSLATION;
        update();
    }
}

void Paint2DWidget::reset(){
    curMode = NONE;
    curColor = DEFAULT_COLOR;
    curWidth = DEFAULT_WIDTH;
    curLineAlg = BRESENHAM;
    curCropAlg = CS;
    clearList(&graphicsList);
    clearList(&transformGraphicsList);
    clearList(&copyGraphicsList);
    clearList(&cropList);
    if(curGraphics != nullptr){
        delete curGraphics;
        curGraphics = nullptr;
    }
    rotatePoint = nullptr;
    scalePoint = nullptr;
    isModified = true;                      //重置也是一种改变
    hasSelected = false;
    update();
}

void Paint2DWidget::paintEvent(QPaintEvent*){
    QPainter painter(this);
//    painter.drawImage(0, 0, *image);
    drawGraphics(painter, curGraphics);
    for(int i = 0; i < graphicsList.size(); ++i){
        drawGraphics(painter, graphicsList[i]);
    }
    drawPoint(painter, rotatePoint);
    drawPoint(painter, scalePoint);
    drawList(painter, &rectangleList, 1);
    drawList(painter, &curveList, 5);
    if(curControlPointIndex != -1){
        drawPoint(painter, curveList[curControlPointIndex]);
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
            curGraphics = new LineSegment(curId++, point, curColor, curWidth, curLineAlg);     //当前为线段
            break;
        }
        case RANDOMLINE:{
            curGraphics = new RandomLine(curId++, point, curColor, curWidth);      //当前为随机线
            break;
        }
        case CIRCLE:{
            curGraphics = new Circle(curId++, point, curColor, curWidth);          //当前为圆
            break;
        }
        case ELLIPSE:{
            curGraphics = new Ellipse(curId++, point, curColor, curWidth);         //当前为椭圆
            break;
        }
        case CURVE: {
            if(curControlPointIndex != -1){
                pressPoint = point;
            }
            break;
        }
        case ERASER:{
            QPoint* p = new QPoint(point.x(), point.y());                 //当前为橡皮擦
            eraser->append(p);
            break;
        }
        case TRANSLATION:{                                                //当前需要平移
            pressPoint = point;
            this->setCursor(Qt::OpenHandCursor);
            break;
        }
        case SELECTBOLCK:{
            blockStartPoint = point;
            break;
        }
        case CROP:{
            cropStartPoint = point;
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
            if(curGraphics == nullptr && e->button() == Qt::RightButton){
                break;
            }
            if(curGraphics == nullptr){
                curGraphics = new Polygon(curId++, point, curColor, curWidth, curLineAlg);   //新建多边形
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
        case CURVE:{
            this->setCursor(Qt::ArrowCursor);
            if(selectControlPoint(point)){                                              //选中了控制点
                break;
            }
            if(curGraphics == nullptr && e->button() == Qt::RightButton){               //初始按下右键，不作为
                break;
            }
            if(curveList.size() == maxCurveNum - 1 && e->button() == Qt::LeftButton){   //大于最大控制点数，不作为
                break;
            }
            QPoint* p = new QPoint(point.x(), point.y());
            curveList.push_back(p);
            if(curGraphics == nullptr){
                curGraphics = new Curve(curId++, point, curColor, curWidth, curCurveAlg, curK);//当前为曲线
            }else{
                Curve* curCurve = (Curve* )curGraphics;
                curCurve->setNextPoint(point);
                if(e->button() == Qt::RightButton){                     //右键点击，结束绘制
                    if(!curGraphics->isNotGraphics()){
                        curGraphics->drawLogic();
                        graphicsList.append(curGraphics);
                    }
                    curGraphics = nullptr;
                    curveList.clear();
                }
            }
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
        case SELECT:{                       //选择一个点，只考虑释放
            //还原之前选中的图元
            clearList(&transformGraphicsList);
            clearList(&copyGraphicsList);
            //获得选中的图元
            hasSelected = false;
            for(int i = 0; i < graphicsList.size(); ++i){
                if(graphicsList[i]->pointIsIn(point)){
                    hasSelected = true;
                    transformGraphicsList.push_back(graphicsList[i]);
                }
            }
            setListColor(&transformGraphicsList);
            break;
        }
        case TRANSLATION:{
            translation(pressPoint, point);
            pressPoint = point;
            this->setCursor(Qt::ArrowCursor);
            break;
        }
        case ROTATION:{
            if(rotatePoint != nullptr){
                delete rotatePoint;
                rotatePoint = nullptr;
            }
            rotatePoint = new QPoint(point.x(), point.y());
            update();
            break;
        }
        case SCALE:{
            if(scalePoint != nullptr){
                delete scalePoint;
                scalePoint = nullptr;
            }
            scalePoint = new QPoint(point.x(), point.y());
            update();
            break;
        }
        case SELECTBOLCK:{
            blockEndPoint = point;
            //还原之前选中的图元
            clearList(&transformGraphicsList);
            clearList(&copyGraphicsList);
            //获得选中的图元
            hasSelected = false;
            for(int i = 0; i < graphicsList.size(); ++i){
                if(graphicsList[i]->pointIsInBlock(blockStartPoint, blockEndPoint)){
                    hasSelected = true;
                    transformGraphicsList.push_back(graphicsList[i]);
                }
            }
            setListColor(&transformGraphicsList);
            rectangleList.clear();
            break;
        }
        case CROP:{
            cropList.clear();
            cropEndPoint = point;
            int xMin = cropStartPoint.x() > cropEndPoint.x() ? cropEndPoint.x() : cropStartPoint.x();
            int xMax = cropStartPoint.x() < cropEndPoint.x() ? cropEndPoint.x() : cropStartPoint.x();
            int yMin = cropStartPoint.y() > cropEndPoint.y() ? cropEndPoint.y() : cropStartPoint.y();
            int yMax = cropStartPoint.y() < cropEndPoint.y() ? cropEndPoint.y() : cropStartPoint.y();
            rectangleList.clear();
            for(int i = 0; i < transformGraphicsList.size(); ++i){
                if(transformGraphicsList[i]->crop(xMin, xMax, yMin, yMax, curCropAlg) == false){
                    cropList.push_back(transformGraphicsList[i]);
                }
            }
            int k = 0;
            for(QVector<Graphics* >::iterator it = graphicsList.begin(); it != graphicsList.end() && k < cropList.size();){
                if((*it) == cropList[k]){
                    delete *it;
                    graphicsList.erase(it);
                    k++;
                }else{
                    it++;
                }
            }
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
        case CURVE: {
            if(curControlPointIndex != -1){
                this->setCursor(Qt::OpenHandCursor);
                translationPoint(curveList[curControlPointIndex], pressPoint, point);
                Curve* curCurve = (Curve* )curGraphics;
                curCurve->translationPoint(curControlPointIndex, pressPoint, point);
                pressPoint = point;
                update();
            }
            break;
        }
        case ERASER:{
            QPoint* p = new QPoint(point.x(), point.y());
            eraser->append(p);
            break;
        }
        case TRANSLATION:{
            translation(pressPoint, point);
            pressPoint = point;
            update();
            break;
        }
        case SELECTBOLCK:{                                  //选择矩形块时同步画出矩形虚线框
            rectangleCalculate(blockStartPoint, point);
            update();
            break;
        }
        case CROP:{
            rectangleCalculate(cropStartPoint, point);
            update();
            break;
        }
        default: break;
    }
    isModified = true;
}

void Paint2DWidget::wheelEvent(QWheelEvent* e){
    int delta = e->delta();

    switch(curMode){
        case ROTATION:{
            if(hasSelected && rotatePoint != nullptr){
                for(int i = 0; i < transformGraphicsList.size(); ++i){
                    transformGraphicsList[i]->rotation(rotatePoint, delta / 120);
                }
                update();
            }
            break;
        }
        case SCALE:{
            if(hasSelected && scalePoint != nullptr){
                for(int i = 0; i < transformGraphicsList.size(); ++i){
                    transformGraphicsList[i]->scale(scalePoint, 1 + (delta * 0.1 / 120));
                }
                update();
            }
            break;
        }
        default: break;
    }

}

void Paint2DWidget::insertMap(){
    commandMap.insert(pair<string, void (Paint2DWidget::*)()>("resetCanvas", &Paint2DWidget::resetCanvasCommand));
    commandMap.insert(pair<string, void (Paint2DWidget::*)()>("setColor", &Paint2DWidget::setColorCommand));
    commandMap.insert(pair<string, void (Paint2DWidget::*)()>("drawLine", &Paint2DWidget::drawLineCommand));
    commandMap.insert(pair<string, void (Paint2DWidget::*)()>("saveCanvas", &Paint2DWidget::saveCanvasCommand));
    commandMap.insert(pair<string, void (Paint2DWidget::*)()>("clip", &Paint2DWidget::clipCommand));
    commandMap.insert(pair<string, void (Paint2DWidget::*)()>("rotate", &Paint2DWidget::rotateCommand));
    commandMap.insert(pair<string, void (Paint2DWidget::*)()>("drawEllipse", &Paint2DWidget::drawEllipseCommand));
    commandMap.insert(pair<string, void (Paint2DWidget::*)()>("drawPolygon", &Paint2DWidget::drawPolygonCommand));
    commandMap.insert(pair<string, void (Paint2DWidget::*)()>("drawCurve", &Paint2DWidget::drawCurveCommand));
    commandMap.insert(pair<string, void (Paint2DWidget::*)()>("translate", &Paint2DWidget::translateCommand));
    commandMap.insert(pair<string, void (Paint2DWidget::*)()>("scale", &Paint2DWidget::scaleCommand));
    LineAlgorithmMap.insert(pair<string, LineAlgorithm>("DDA", DDA));
    LineAlgorithmMap.insert(pair<string, LineAlgorithm>("Bresenham", BRESENHAM));
    CropAlgorithmMap.insert(pair<string, CropAlgorithm>("Cohen-Sutherland", CS));
    CropAlgorithmMap.insert(pair<string, CropAlgorithm>("Liang-Barsky", LB));
    CurveAlgorithmMap.insert(pair<string, CurveAlgorithm>("Bezier", BEZIER));
    CurveAlgorithmMap.insert(pair<string, CurveAlgorithm>("Bspline", BSPLINE));
}

int Paint2DWidget::findGraphics(int id){
    int index = 0;
    bool flag = false;
    for(int i = 0; i < graphicsList.size(); ++i){
        if(graphicsList[i]->getId() == id){
            curTransformGraphics = graphicsList[i];
            index = i;
            flag = true;
            break;
        }
    }
    if(flag){
        return index;
    }
    return -1;
}

void Paint2DWidget::resetCanvasCommand(){
    struct::ResetCanvas r;
    fin >> r;
    reset();
    resize(r.width, r.height);
//    fout << r.width << " " << r.height << endl;
}

void Paint2DWidget::saveCanvasCommand(){
    struct::SaveCanvas s;
    fin >> s;
    QString name((s.fileName + ".bmp").c_str());
    saveTo(name);
}

void Paint2DWidget::setColorCommand(){
    struct::SetColor s;
    fin >> s;
    QColor q(s.R, s.G, s.B);
    setColor(q);
//    fout << s.R << " " << s.G << " " << s.B << endl;
}

void Paint2DWidget::drawLineCommand(){
    struct::DrawLine d;
    fin >> d;

    QPoint startPoint(d.x1, d.y1);
    QPoint endPoint(d.x2, d.y2);
    curGraphics = new LineSegment(d.id, startPoint, curColor, curWidth, LineAlgorithmMap[d.alg]);
    LineSegment* curLine = (LineSegment* )curGraphics;
    curLine->setEndPoint(endPoint);
    if(!curLine->isNotGraphics()){                                      //可能只是一个点
        curGraphics->drawLogic();
        graphicsList.append(curGraphics);                               //加入已有图形列表
    }
    curGraphics = nullptr;

//    for(int i = 0; i < NN; ++i){
//        QPoint startPoint(d.x1, d.y1);
//        QPoint endPoint(d.x2, d.y2);
//        curGraphics = new LineSegment(d.id + i, startPoint, curColor, curWidth, LineAlgorithmMap[d.alg]);
//        LineSegment* curLine = (LineSegment* )curGraphics;
//        curLine->setEndPoint(endPoint);
//        if(!curLine->isNotGraphics()){                                      //可能只是一个点
//            curGraphics->drawLogic();
//            graphicsList.append(curGraphics);                               //加入已有图形列表
//        }
//        curGraphics = nullptr;
//    }
}

void Paint2DWidget::drawEllipseCommand(){
    struct::DrawEllipse d;
    fin >> d;
    QPoint point(d.x, d.y);
    curGraphics = new Ellipse(d.id, point, curColor, curWidth);
    Ellipse* curEllipse = (Ellipse* )curGraphics;
    curEllipse->setR(d.rx, d.ry);
    if(!curEllipse->isNotGraphics()){
        curGraphics->drawLogic();
        graphicsList.append(curGraphics);
    }
    curGraphics = nullptr;
}

void Paint2DWidget::drawPolygonCommand(){
    struct::DrawPolygon d;
    fin >> d;

    QPoint p(d.points[0].first, d.points[0].second);
    curGraphics = new Polygon(d.id, p, curColor, curWidth, LineAlgorithmMap[d.alg]);
    Polygon* curPolygon = (Polygon* )curGraphics;
    for(int i = 1; i < d.n; ++i){
        QPoint point(d.points[i].first, d.points[i].second);
        curPolygon->setNextPoint(point);
    }
    curPolygon->complete();
    if(!curGraphics->isNotGraphics()){
        curGraphics->drawLogic();
        graphicsList.append(curGraphics);
    }
    curGraphics = nullptr;
}

void Paint2DWidget::drawCurveCommand(){
    struct::DrawCurve d;
    fin >> d;

    QPoint p(d.points[0].first, d.points[0].second);
    curGraphics = new Curve(d.id, p, curColor, curWidth, CurveAlgorithmMap[d.alg]);
    Curve* curCurve = (Curve* )curGraphics;
    for(int i = 1; i < d.n; ++i){
        QPoint point(d.points[i].first, d.points[i].second);
        curCurve->setNextPoint(point);
    }
    if(!curGraphics->isNotGraphics()){
        curGraphics->drawLogic();
        graphicsList.append(curGraphics);
    }
    curGraphics = nullptr;
}

void Paint2DWidget::rotateCommand(){
    struct::Rotate r;
    fin >> r;
    if(rotatePoint != nullptr){
        delete rotatePoint;
        rotatePoint = nullptr;
    }
    rotatePoint = new QPoint(r.x, r.y);
    int index = findGraphics(r.id);
    if(curTransformGraphics != nullptr && index != -1){
        curTransformGraphics->rotation(rotatePoint, r.degree);
    }
    curTransformGraphics = nullptr;
}

void Paint2DWidget::translateCommand(){
    struct::Translate t;
    fin >> t;
    int index = findGraphics(t.id);
    if(curTransformGraphics != nullptr && index != -1){
        curTransformGraphics->translation(t.xOffset, t.yOffset);
    }
    curTransformGraphics = nullptr;
}

void Paint2DWidget::scaleCommand(){
    struct::Scale s;
    fin >> s;
    if(scalePoint != nullptr){
        delete scalePoint;
        scalePoint = nullptr;
    }
    scalePoint = new QPoint(s.x, s.y);
    int index = findGraphics(s.id);
    if(curTransformGraphics != nullptr && index != -1){
        curTransformGraphics->scale(scalePoint, s.s);
    }
    curTransformGraphics = nullptr;
}

void Paint2DWidget::clipCommand(){
    struct::Clip c;
    fin >> c;

    int xMin = c.x1;
    int xMax = c.x2;
    int yMin = c.y1;
    int yMax = c.y2;

    int index = findGraphics(c.id);

    if(curTransformGraphics != nullptr && index != -1){
        if(curTransformGraphics->crop(xMin, xMax, yMin, yMax, CropAlgorithmMap[c.alg]) == false){
            delete curTransformGraphics;
            graphicsList.erase(graphicsList.begin() + index);
        }
    }
    curTransformGraphics = nullptr;

//    clock_t startTime,endTime;
//    startTime = clock();
//    for(int i = 0; i < 100000; ++i){
//        int xMin = c.x1;
//        int xMax = c.x2;
//        int yMin = c.y1;
//        int yMax = c.y2;

//        int index = findGraphics(c.id + i);

//        if(curTransformGraphics != nullptr && index != -1){
//            if(curTransformGraphics->crop(xMin, xMax, yMin, yMax, CropAlgorithmMap[c.alg]) == false){
//                delete curTransformGraphics;
//                graphicsList.erase(graphicsList.begin() + index);
//            }
//        }
//        curTransformGraphics = nullptr;
//    }
//    endTime = clock();
//    fout << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
}
