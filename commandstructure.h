#ifndef COMMANDSTRUCTURE_H
#define COMMANDSTRUCTURE_H
#include <fstream>
#include <string>
using namespace std;

//class command{

//};

//class resetCanvas : public command{
//private:
//    int width;
//    int height;
//public:
//    friend ifstream& operator>>(ifstream& fin, resetCanvas& r){
//        fin >> r.width >> r.height;
//        return fin;
//    }
//};

//class setColor: public command{
//private:
//    int R;
//    int G;
//    int B;
//public:
//    friend ifstream& operator>>(ifstream& fin, setColor& s) {
//        fin >> s.R >> s.G >> s.B;
//        return fin;
//    }
//};

struct resetCanvas{
    int width;
    int height;
    friend ifstream& operator>>(ifstream& fin, resetCanvas& r){
        fin >> r.width >> r.height;
        return fin;
    }
};

struct setColor{
    int R;
    int G;
    int B;
    friend ifstream& operator>>(ifstream& fin, setColor& s) {
        fin >> s.R >> s.G >> s.B;
        return fin;
    }
};

struct drawLine{
    int id;
    int x1;
    int y1;
    int x2;
    int y2;
    string alg;
    friend ifstream& operator>>(ifstream& in, drawLine& d) {
        in >> d.id >> d.x1 >> d.y1 >> d.x2 >> d.y2 >> d.alg;
        return in;
    }
    friend ofstream& operator<<(ofstream& out, drawLine& d) {
        out << d.id << " " << d.x1 << " " << d.y1 << " " << d.x2 << " " << d.y2 << " " << d.alg;
        return out;
    }
};

struct saveCanvas{
    string fileName;
    friend ifstream& operator>>(ifstream& in, saveCanvas& s) {
        in >> s.fileName;
        return in;
    }
};

#endif // COMMANDSTRUCTURE_H
