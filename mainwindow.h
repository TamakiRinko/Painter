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
    QAction* newWindowAction;           //新建窗口
    QAction* saveFileAction;            //保存文件
    QString fileName;                   //保存的文件名



    void setAction();                   //设置Action



protected:
    void closeEvent(QCloseEvent* event);
    void resizeEvent(QResizeEvent *event);

private slots:
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

    //Action
    void newWindow();
    bool saveFile();
    void on_RandomLineButton_clicked();
};

#endif // MAINWINDOW_H
