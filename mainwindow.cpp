#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);
    resize(800, 600);

    setAction();

    paint2DWidget = new Paint2DWidget;
    paint2DWidget->setMode(NONE);
    ui->Canvas->addWidget(paint2DWidget);
    paint3DWidget = new QOpenGLWidget;
    ui->Canvas->addWidget(paint3DWidget);
    paint3DWidget->hide();

    ui->ModeLine->setText("None");

    fileName = nullptr;
}

void MainWindow::setAction(){
    fileMenu = ui->menuBar->addMenu("文件");

    //新建窗口
    newWindowAction = new QAction("新建");
    newWindowAction->setShortcut(QKeySequence::New);
    connect(newWindowAction, SIGNAL(triggered(bool)), this, SLOT(newWindow()));

    //保存文件
    saveFileAction = new QAction("保存");
    saveFileAction->setShortcut(QKeySequence::Save);
    connect(saveFileAction, SIGNAL(triggered(bool)), this, SLOT(saveFile()));







    fileMenu->addAction(newWindowAction);
    fileMenu->addAction(saveFileAction);
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

void MainWindow::on_PaintModeCheckBox_clicked(){
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

/**
 * @brief MainWindow::newOneFile
 * 新建一个窗口
 */
void MainWindow::newWindow(){
    MainWindow* newWindow = new MainWindow;
    newWindow->show();
}

bool MainWindow::saveFile(){
    if(fileName == nullptr){
        fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "newPic", tr("Image (*.bmp)"));
    }
    if(fileName == "")  return false;
    fileName = fileName.toUtf8();
    paint2DWidget->saveTo(fileName);
    return true;
}

void MainWindow::closeEvent(QCloseEvent* event){
    if (paint2DWidget->getIsModified()){
        int temp = QMessageBox::information(this, "Message", "文件未保存，是否保存？", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (temp == QMessageBox::Yes){              //保存
            if(saveFile()){
                event->accept();
            }else{
                event->ignore();
            }
        }
        else if(temp == QMessageBox::No){           //不保存
            event->accept();
        }else{                                      //取消关闭
            event->ignore();
        }
    }
    else{
        event->accept();
    }
}
