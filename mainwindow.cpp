#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);
    resize(800, 600);
    setWindowTitle("Painter");
    fileName = nullptr;

    setAction();

    paint2DWidget = new Paint2DWidget;
    paint2DWidget->setMode(NONE);
    ui->Canvas->addWidget(paint2DWidget);
    paint3DWidget = new QOpenGLWidget;
    ui->Canvas->addWidget(paint3DWidget);
    paint3DWidget->hide();

}

void MainWindow::setAction(){
    fileMenu = ui->menuBar->addMenu("File");

    //新建窗口
    newWindowAction = new QAction("New");
    newWindowAction->setShortcut(QKeySequence::New);
    connect(newWindowAction, SIGNAL(triggered(bool)), this, SLOT(newWindow()));
    //保存文件
    saveFileAction = new QAction("Save");
    saveFileAction->setShortcut(QKeySequence::Save);
    connect(saveFileAction, SIGNAL(triggered(bool)), this, SLOT(saveFile()));
    //复制
    copyAction = new QAction("Copy");
    copyAction->setShortcut(QKeySequence::Copy);
    connect(copyAction, SIGNAL(triggered(bool)), this, SLOT(graphicsCopy_triggered()));
    //粘贴
    pasteAction = new QAction("Paste");
    pasteAction->setShortcut(QKeySequence::Paste);
    connect(pasteAction, SIGNAL(triggered(bool)), this, SLOT(graphicsPaste_triggered()));

    fileMenu->addAction(newWindowAction);
    fileMenu->addAction(saveFileAction);
    fileMenu->addAction(copyAction);
    fileMenu->addAction(pasteAction);

    //平移
    transformMenu = ui->menuBar->addMenu("Transform");
    translationAction = new QAction("Translation");
    connect(translationAction, SIGNAL(triggered(bool)), this, SLOT(on_actionTranslation_triggered()));
    transformMenu->addAction(translationAction);
}

MainWindow::~MainWindow(){
    delete ui;
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
        int temp = QMessageBox::information(this, "File Not Saved", "文件未保存，是否保存？", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
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

/**
 * @brief MainWindow::resizeEvent
 * @param event
 * 检测窗口改变，调整大小
 */
void MainWindow::resizeEvent(QResizeEvent*){
    ui->HSpinBox->setValue(this->height());
    ui->WSpinBox->setValue(this->width());
}

void MainWindow::on_LineSegmentButton_clicked(){
    paint2DWidget->setMode(LINESEGMENT);

    QMessageBox box(QMessageBox::Warning, "LineSegmentAlgorithm", "DDA or BresenHam?\n");
    box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    box.setButtonText(QMessageBox::Yes, QString("DDA"));
    box.setButtonText(QMessageBox::No, QString("BresenHam"));
    int button = box.exec();
    if (button == QMessageBox::Yes){              //保存
        paint2DWidget->setLineAlgorithm(DDA);
        ui->ModeLabel->setText("LineSegment, DDA");
    }
    else if(button == QMessageBox::No){           //不保存
        paint2DWidget->setLineAlgorithm(BRESENHAM);
        ui->ModeLabel->setText("LineSegment, BresenHam");
    }
}

void MainWindow::on_RandomLineButton_clicked(){
    paint2DWidget->setMode(RANDOMLINE);
    ui->ModeLabel->setText("RandomLine");
}

void MainWindow::on_PolygonButton_clicked(){
    paint2DWidget->setMode(POLYGON);
//    ui->ModeLabel->setText("Polygon");
    QMessageBox box(QMessageBox::Warning, "PolygonAlgorithm", "DDA or BresenHam?\n");
    box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    box.setButtonText(QMessageBox::Yes, QString("DDA"));
    box.setButtonText(QMessageBox::No, QString("BresenHam"));
    int button = box.exec();
    if (button == QMessageBox::Yes){              //保存
        paint2DWidget->setLineAlgorithm(DDA);
        ui->ModeLabel->setText("Polygon, DDA");
    }
    else if(button == QMessageBox::No){           //不保存
        paint2DWidget->setLineAlgorithm(BRESENHAM);
        ui->ModeLabel->setText("Polygon, BresenHam");
    }
}

void MainWindow::on_CircleButton_clicked(){
    paint2DWidget->setMode(CIRCLE);
    ui->ModeLabel->setText("Circle");
}

void MainWindow::on_EllipseButton_clicked(){
    paint2DWidget->setMode(ELLIPSE);
    ui->ModeLabel->setText("Ellipse");
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
        ui->RandomLineButton->setDisabled(true);
        ui->PolygonButton->setDisabled(true);
        paint2DWidget->setMode(NONE);
        paint2DWidget->setColor(DEFAULT_COLOR);
    }else{
        setWindowTitle("2DPaint");
        paint3DWidget->hide();
        paint2DWidget->show();
        ui->LineSegmentButton->setEnabled(true);
        ui->CircleButton->setEnabled(true);
        ui->EllipseButton->setEnabled(true);
        ui->RandomLineButton->setEnabled(true);
        ui->PolygonButton->setEnabled(true);
    }
}

void MainWindow::on_EraserButton_clicked(){
    paint2DWidget->setMode(ERASER);
    ui->ModeLabel->setText("Eraser");
}

void MainWindow::on_WithDrawButton_clicked(){
    ui->ModeLabel->setText("WithDraw");
    paint2DWidget->withDraw();
}

void MainWindow::on_WidthSpinBox_valueChanged(int arg1){
    paint2DWidget->setWidth(arg1);
}

void MainWindow::on_WSpinBox_valueChanged(int arg1){
    resize(arg1, this->height());
}

void MainWindow::on_HSpinBox_valueChanged(int arg1){
    resize(this->width(), arg1);
}

void MainWindow::on_actionTranslation_triggered(){
    ui->ModeLabel->setText("Translation");
    paint2DWidget->setMode(TRANSLATION);
}

void MainWindow::on_SelectButton_clicked(){
    ui->ModeLabel->setText("Select");
    paint2DWidget->setMode(SELECT);
}

void MainWindow::graphicsCopy_triggered(){
    ui->ModeLabel->setText("Copy");
    paint2DWidget->graphicsCopy();
}

void MainWindow::graphicsPaste_triggered(){
    ui->ModeLabel->setText("Paste");
    paint2DWidget->graphicsPaste();
    ui->ModeLabel->setText("Translation");
}
