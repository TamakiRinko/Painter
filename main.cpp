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
//    w.show();

//    return a.exec();
//    if(argc == 1){
//        QApplication a(argc, argv);
//        MainWindow w;
//        w.show();

//        return a.exec();
//    }else{
//        ofstream fout;
//        fout.open("debug.txt", ios::out);
//        ifstream fin;
//        fin.open(argv[1], ios::in);
//        string order;
//        fin >> order;
//        while(!fin.fail()){
//            if (order == "resetCanvas"){
//                resetCanvas r;
//                fin >> r;

//            } else if (order == "setColor"){
//                setColor s;
//                fin >> s;
//            }
//            fin >> order;
//        }
//        QApplication a(argc, argv);
//        MainWindow w(argv[1]);
//        w.show();

//        return a.exec();
//    }

}
