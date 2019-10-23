#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);
    resize(800, 600);

    paint2DWidget = new Paint2DWidget;
    paint2DWidget->setMode(NONE);
    ui->Canvas->addWidget(paint2DWidget);
    paint3DWidget = new QOpenGLWidget;
    ui->Canvas->addWidget(paint3DWidget);
    paint3DWidget->hide();

    ui->ModeLine->setText("None");
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_LineSegmentButton_clicked(){
    paint2DWidget->setMode(LINESEGMENT);
    ui->ModeLine->setText("LineSegment");
}

void MainWindow::on_CircleButton_clicked(){
    paint2DWidget->setMode(CIRCLE);
    ui->ModeLine->setText("Circle");
}

void MainWindow::on_EllipseButton_clicked(){
    paint2DWidget->setMode(ELLIPSE);
    ui->ModeLine->setText("Ellipse");
}

void MainWindow::on_ColorButton_clicked(){
    paint2DWidget->setColor(QColorDialog::getColor());
}

void MainWindow::on_ModeCheckBox_clicked(){
    if(ui->PaintModeCheckBox->isChecked()){
        setWindowTitle("3DPaint");
        paint3DWidget->show();
        paint2DWidget->hide();
        ui->LineSegmentButton->setDisabled(true);
        ui->CircleButton->setDisabled(true);
        ui->EllipseButton->setDisabled(true);
        paint2DWidget->setMode(NONE);
        paint2DWidget->setColor(DEFAULT_COLOR);
    }else{
        setWindowTitle("2DPaint");
        paint3DWidget->hide();
        paint2DWidget->show();
        ui->LineSegmentButton->setEnabled(true);
        ui->CircleButton->setEnabled(true);
        ui->EllipseButton->setEnabled(true);
    }
}

void MainWindow::on_EraserButton_clicked(){
    paint2DWidget->setMode(ERASER);
    ui->ModeLine->setText("Eraser");
}
