#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QColorDialog>
#include <QOpenGLWidget>
#include <QBoxLayout>
#include <QFileDialog>
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

private slots:
    void on_LineSegmentButton_clicked();
    void on_CircleButton_clicked();
    void on_EllipseButton_clicked();
    void on_ColorButton_clicked();
    void on_PaintModeCheckBox_clicked();
    void on_EraserButton_clicked();

    //Action
    void newWindow();
    bool saveFile();
};

#endif // MAINWINDOW_H
