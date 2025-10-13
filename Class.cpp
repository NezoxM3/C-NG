#include <iostream>
#include <cmath>
#include <fstream>
using namespace std;

class Point {
private:
    int x;
    int y;

public:
    Point() : x(0), y(0) {}

    Point(int x, int y) : x(x), y(y) {}

    int getX() const { return x; }
    int getY() const { return y; }

    void setX(int x) { this->x = x; }
    void setY(int y) { this->y = y; }

    double distanceTo(const Point &other) const {
        int dx = x - other.x;
        int dy = y - other.y;
        return sqrt(dx * dx + dy * dy);
    }

    void saveToFile(const string &filename) const {
        ofstream fout(filename);
        if (fout.is_open()) {
            fout << x << " " << y;
            fout.close();
        } else {
            cout << "Помилка відкриття файлу для запису!\n";
        }
    }

    void loadFromFile(const string &filename) {
        ifstream fin(filename);
        if (fin.is_open()) {
            fin >> x >> y;
            fin.close();
        } else {
            cout << "Помилка відкриття файлу для читання!\n";
        }
    }
};

class Rectangle {
private:
    Point bottomLeft;
    Point topRight;

public:
    Rectangle() : bottomLeft(0, 0), topRight(0, 0) {}

    Rectangle(const Point &bl, const Point &tr) : bottomLeft(bl), topRight(tr) {}

    int area() const {
        int width = topRight.getX() - bottomLeft.getX();
        int height = topRight.getY() - bottomLeft.getY();
        return abs(width * height);
    }

    int perimeter() const {
        int width = abs(topRight.getX() - bottomLeft.getX());
        int height = abs(topRight.getY() - bottomLeft.getY());
        return 2 * (width + height);
    }
};

int main() {
    Point p1(3, 4);
    Point p2(7, 1);

    cout << "Відстань між точками: " << p1.distanceTo(p2) << endl;

    p1.saveToFile("point.txt");

    Point p3;
    p3.loadFromFile("point.txt");
    cout << "Завантажена точка: (" << p3.getX() << ", " << p3.getY() << ")\n";

    Rectangle rect(Point(0, 0), Point(5, 3));
    cout << "Площа прямокутника: " << rect.area() << endl;
    cout << "Периметр прямокутника: " << rect.perimeter() << endl;

    return 0;
}