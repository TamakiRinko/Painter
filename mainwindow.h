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
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Paint2DWidget* paint2DWidget;
    QOpenGLWidget* paint3DWidget;

    QMenu* fileMenu;                    //文件菜单
    QMenu* transformMenu;               //操作菜单
    QAction* newWindowAction;           //新建窗口
    QAction* saveFileAction;            //保存文件
    QAction* copyAction;                //复制
    QAction* pasteAction;               //粘贴
    QAction* translationAction;         //平移
    QAction* rotateAction;              //旋转


    QString fileName;                   //保存的文件名




    void setAction();                   //设置Action



protected:
    void closeEvent(QCloseEvent* event);
    void resizeEvent(QResizeEvent *event);

private slots:

    //Action
    void newWindow();
    bool saveFile();

    void on_LineSegmentButton_clicked();
    void on_CircleButton_clicked();
    void on_EllipseButton_clicked();
    void on_ColorButton_clicked();
    void on_PaintModeCheckBox_clicked();
    void on_EraserButton_clicked();
    void on_PolygonButton_clicked();
    void on_WithDrawButton_clicked();
    void on_WidthSpinBox_valueChanged(int arg1);
    void on_WSpinBox_valueChanged(int arg1);
    void on_HSpinBox_valueChanged(int arg1);
    void on_RandomLineButton_clicked();
    void on_actionTranslation_triggered();
    void on_actionRotation_triggered();
    void on_SelectButton_clicked();

    void graphicsCopy_triggered();
    void graphicsPaste_triggered();
};

#endif // MAINWINDOW_H
