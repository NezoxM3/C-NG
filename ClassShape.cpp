#include <iostream>
#include <vector>
using namespace std;

class Figure {
public:
    virtual void print() const = 0;
    virtual ~Figure() {}
};

class Rectangle : public Figure {
private:
    double x1, y1, x2, y2;
public:
    Rectangle(double _x1, double _y1, double _x2, double _y2)
        : x1(_x1), y1(_y1), x2(_x2), y2(_y2) {}

    void print() const override {
        cout << "Rectangle: (" << x1 << ", " << y1
             << ") to (" << x2 << ", " << y2 << ")\n";
    }
};

class Square : public Figure {
private:
    double x, y, side;
public:
    Square(double _x, double _y, double _side)
        : x(_x), y(_y), side(_side) {}

    void print() const override {
        cout << "Square: top-left (" << x << ", " << y
             << "), side = " << side << "\n";
    }
};

class Circle : public Figure {
private:
    double x, y, r;
public:
    Circle(double _x, double _y, double _r)
        : x(_x), y(_y), r(_r) {}

    void print() const override {
        cout << "Circle: center (" << x << ", " << y
             << "), radius = " << r << "\n";
    }
};

int main() {
    vector<Figure*> figures;
    int choice;

    while (true) {
        cout << "1 - Rectangle, 2 - Square, 3 - Circle, 0 - Exit & Print: ";
        cin >> choice;

        if (choice == 0) break;

        switch (choice) {
            case 1: {
                double x1, y1, x2, y2;
                cout << "Enter x1 y1 x2 y2: ";
                cin >> x1 >> y1 >> x2 >> y2;
                figures.push_back(new Rectangle(x1, y1, x2, y2));
                break;
            }
            case 2: {
                double x, y, side;
                cout << "Enter x y side: ";
                cin >> x >> y >> side;
                figures.push_back(new Square(x, y, side));
                break;
            }
            case 3: {
                double x, y, r;
                cout << "Enter x y radius: ";
                cin >> x >> y >> r;
                figures.push_back(new Circle(x, y, r));
                break;
            }
            default:
                cout << "Invalid choice!" << endl;
        }
    }

    cout << "\nAll figures:\n";
    for (Figure* f : figures) {
        f->print();
    }

    for (Figure* f : figures) {
        delete f;
    }

    return 0;
}