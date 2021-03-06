#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QColorDialog>
#include <QOpenGLWidget>
#include <QBoxLayout>
#include <QFileDialog>
#include <QSpinBox>
#include "paint2dwidget.h"

namespace Ui {
class MainWindow; 
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const char* file = nullptr, QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Paint2DWidget* paint2DWidget;
//    QOpenGLWidget* paint3DWidget;

    QMenu* fileMenu;                    //文件菜单
    QMenu* transformMenu;               //操作菜单
    QAction* newWindowAction;           //新建窗口
    QAction* saveFileAction;            //保存文件
    QAction* copyAction;                //复制
    QAction* pasteAction;               //粘贴

    QAction* DDAAction;                 //选择DDA算法
    QAction* BresenhamAction;           //选择Bresenham算法
    QAction* CSAction;                  //选择Cohen-Sutherland算法
    QAction* LBAction;                  //选择Liang-Barsky算法
    QAction* BezierAction;              //选择Bezier算法
    QAction* BsplineAction;             //选择B-Spline算法

    QAction* translationAction;         //平移
    QAction* rotateAction;              //旋转
    QAction* scaleAction;               //缩放
    QAction* cropAction;                //裁剪

    QString fileName;                   //保存的文件名


    void setAction();                   //设置Action



protected:
    void closeEvent(QCloseEvent* event);
    void resizeEvent(QResizeEvent *event);

private slots:

    //Action
    void newWindow();
    bool saveFile();

    void on_actionDDA_triggered();
    void on_actionBresenHam_triggered();
    void on_actionCS_triggered();
    void on_actionLB_triggered();
    void on_actionBezier_triggered();
    void on_actionBspline_triggered();

    void on_LineSegmentButton_clicked();
    void on_RandomLineButton_clicked();
    void on_PolygonButton_clicked();
    void on_CircleButton_clicked();
    void on_EllipseButton_clicked();
    void on_CurveButton_clicked();
    void on_ColorButton_clicked();
//    void on_PaintModeCheckBox_clicked();
    void on_EraserButton_clicked();
    void on_WithDrawButton_clicked();
    void on_ReDoButton_clicked();
    void on_WidthSpinBox_valueChanged(int arg1);
    void on_WSpinBox_valueChanged(int arg1);
    void on_HSpinBox_valueChanged(int arg1);
    void on_actionTranslation_triggered();
    void on_actionRotation_triggered();
    void on_actionScale_triggered();
    void on_actionCrop_triggered();

    void on_SelectButton_clicked();
    void on_SelectBlockButton_clicked();

    void graphicsCopy_triggered();
    void graphicsPaste_triggered();
    void on_ResetButton_clicked();
    void on_KSpinBox_valueChanged(int arg1);
};

#endif // MAINWINDOW_H
