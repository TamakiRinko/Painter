#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QColorDialog>
#include <QOpenGLWidget>
#include <QBoxLayout>
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



protected:

private slots:
    void on_LineSegmentButton_clicked();
    void on_CircleButton_clicked();
    void on_EllipseButton_clicked();
    void on_ColorButton_clicked();
    void on_ModeCheckBox_clicked();
    void on_EraserButton_clicked();
};

#endif // MAINWINDOW_H
