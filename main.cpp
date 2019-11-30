#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    if(argc == 2){
        MainWindow w(argv[1]);
        a.exit();
        return 0;
    }else{
        MainWindow w;
        return a.exec();
    }
}
