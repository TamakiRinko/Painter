#ifndef COMMANDSTRUCTURE_H
#define COMMANDSTRUCTURE_H
#include <fstream>
#include <string>
#include <vector>
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

struct ResetCanvas{
    int width;
    int height;
    friend ifstream& operator>>(ifstream& fin, ResetCanvas& r){
        fin >> r.width >> r.height;
        return fin;
    }
};

struct SetColor{
    int R;
    int G;
    int B;
    friend ifstream& operator>>(ifstream& fin, SetColor& s) {
        fin >> s.R >> s.G >> s.B;
        return fin;
    }
};

struct DrawLine{
    int id;
    int x1;
    int y1;
    int x2;
    int y2;
    string alg;
    friend ifstream& operator>>(ifstream& in, DrawLine& d) {
        in >> d.id >> d.x1 >> d.y1 >> d.x2 >> d.y2 >> d.alg;
        return in;
    }
    friend ofstream& operator<<(ofstream& out, DrawLine& d) {
        out << d.id << " " << d.x1 << " " << d.y1 << " " << d.x2 << " " << d.y2 << " " << d.alg;
        return out;
    }
};

struct SaveCanvas{
    string fileName;
    friend ifstream& operator>>(ifstream& in, SaveCanvas& s) {
        in >> s.fileName;
        return in;
    }
};

struct Clip{
    int id;
    int x1;
    int y1;
    int x2;
    int y2;
    string alg;
    friend ifstream& operator>>(ifstream& in, Clip& c) {
        in >> c.id >> c.x1 >> c.y1 >> c.x2 >> c.y2 >> c.alg;
        return in;
    }
};

struct Rotate{
    int id;
    int x;
    int y;
    int degree;
    friend ifstream& operator>>(ifstream& in, Rotate& r) {
        in >> r.id >> r.x >> r.y >> r.degree;
        return in;
    }
};

struct DrawEllipse{
    int id;
    int x;
    int y;
    int rx;
    int ry;
    friend ifstream& operator>>(ifstream& in, DrawEllipse& d) {
        in >> d.id >> d.x >> d.y >> d.rx >> d.ry;
        return in;
    }
};

struct DrawPolygon{
    int id;
    int n;
    string alg;
    vector< pair<int, int> > points;
    friend ifstream& operator>>(ifstream& in, DrawPolygon& d) {
        in >> d.id >> d.n >> d.alg;
        int x, y;
        for(int i = 0; i < d.n; ++i){
            in >> x >> y;
            d.points.push_back(pair<int, int>(x, y));
        }
        return in;
    }
};

struct Translate{
    int id;
    int xOffset;
    int yOffset;
    friend ifstream& operator>>(ifstream& in, Translate& t) {
        in >> t.id >> t.xOffset >> t.yOffset;
        return in;
    }
};

struct Scale{
    int id;
    int x;
    int y;
    float s;
    friend ifstream& operator>>(ifstream& in, Scale& s) {
        in >> s.id >> s.x >> s.y >> s.s;
        return in;
    }
};

#endif // COMMANDSTRUCTURE_H
