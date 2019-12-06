#ifndef PAINTWINDOW_H
#define PAINTWINDOW_H

#include <QMainWindow>
//#include "graphics.h"
#include "linesegment.h"
#include "circle.h"
#include "eraser.h"
#include "ellipse.h"
#include "polygon.h"
#include "randomline.h"
#include "curve.h"
#include "command.h"
#include <QWidget>
#include <QPen>
#include <QBrush>
#include <QMouseEvent>
#include <QDebug>
#include <QAction>
#include <unordered_map>
#include <time.h>

#define NN 100000


class Paint2DWidget : public QWidget{
    Q_OBJECT

public:
    explicit Paint2DWidget(const char* fileName = nullptr, QWidget *parent = nullptr);
    void setMode(Mode mode);
    void setColor(QColor color);
    void setWidth(int width);
    void setLineAlgorithm(LineAlgorithm alg);
    void setCropAlgorithm(CropAlgorithm alg);
    void setCurveAlgorithm(CurveAlgorithm alg);
    void setK(int k);
    bool getIsModified();

    ~Paint2DWidget();

    void withDraw();                            //撤回
    void saveTo(QString fileName, const char* format = "bmp");            //保存为指定格式
    void graphicsCopy();
    void graphicsPaste();
    void reset();

private:
    int curId;                                  //全局Id序列
    Mode curMode;                               //当前的模式
    QColor curColor;                            //当前选择的颜色
    int curWidth;                               //当前的宽度
    LineAlgorithm curLineAlg;                   //当前线段算法，默认BresenHam
    CropAlgorithm curCropAlg;                   //当前裁剪算法，默认为CS
    CurveAlgorithm curCurveAlg;                 //当前曲线算法，默认为BEZIER
    int curK;                                   //当前B样条次数，默认为3
    QVector<Graphics* > graphicsList;           //界面上所有的图形
    Graphics* curGraphics;                      //当前正在画的图形
    Eraser* eraser;                             //橡皮擦
    bool isModified;                            //画板是否已经被修改

    QPoint pressPoint;                          //鼠标按下时的坐标
    QVector<Graphics* > transformGraphicsList;  //需要进行转换的图元列表
    QVector<Graphics* > copyGraphicsList;       //复制的图元列表
    bool hasSelected;                           //已经选中

    QPoint* rotatePoint;                        //旋转中心点
    QPoint* scalePoint;                         //缩放中心点

    QPoint blockStartPoint;                     //区域左上点
    QPoint blockEndPoint;                       //区域右下点
    QPoint cropStartPoint;                      //裁剪左上点
    QPoint cropEndPoint;                        //裁剪右下点
    QVector<Graphics* > cropList;               //完全去除的线段

    QVector<QPoint* > rectangleList;            //矩形框

    QVector<QPoint* > curveList;                //曲线控制点
    int curControlPointIndex;                   //当前选中的控制点下标

//    QImage* image;

    void drawGraphics(QPainter& painter, Graphics* graphics);           //画出一个图形
    void drawPoint(QPainter& painter, QPoint* point);                   //画出一个点
    void drawList(QPainter& painter, QVector<QPoint* >* list, int width);//画出一个点的列表

    void eraseGraphics();                           //擦除橡皮擦覆盖到的图形
    void translation(QPoint start, QPoint end);     //平移
    void clearList(QVector<Graphics* >* list);      //清理
    void setListColor(QVector<Graphics* >* list);   //上色
    void rectangleCalculate(QPoint& startPoint, QPoint& endPoint);             //把选取矩的形边框画出来
    bool selectControlPoint(QPoint point);          //是否选中控制点，选中则可以改变控制点
    void translationPoint(QPoint* point, QPoint start, QPoint end);            //平移指定点


    ofstream fout;                              //debug输出
    ifstream fin;                               //读入指令
    unordered_map<string, void (Paint2DWidget::*)()> commandMap;
    unordered_map<string, LineAlgorithm> LineAlgorithmMap;
    unordered_map<string, CropAlgorithm> CropAlgorithmMap;
    unordered_map<string, CurveAlgorithm> CurveAlgorithmMap;
    Graphics* curTransformGraphics;             //命令行时当前处理的图元

    void insertMap();
    int findGraphics(int id);
    void resetCanvasCommand();
    void saveCanvasCommand();
    void setColorCommand();
    void drawLineCommand();
    void drawEllipseCommand();
    void drawPolygonCommand();
    void drawCurveCommand();
    void translateCommand();
    void scaleCommand();
    void clipCommand();
    void rotateCommand();

protected:
    void paintEvent(QPaintEvent* e);
    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void wheelEvent(QWheelEvent* e);
};


#endif // PAINTWINDOW_H
