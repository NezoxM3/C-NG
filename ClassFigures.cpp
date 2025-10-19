#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <limits>

using namespace std;

class Shape {
public:
    virtual void print() const = 0;
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual string serialize() const = 0;
    virtual ~Shape() {}
};

class Rectangle : public Shape {
private:
    double x1, y1, x2, y2;
public:
    Rectangle(double _x1, double _y1, double _x2, double _y2)
        : x1(_x1), y1(_y1), x2(_x2), y2(_y2) {}

    void print() const override {
        cout << "Rectangle: (" << x1 << ", " << y1 << ") to (" << x2 << ", " << y2 << ")"
             << " | area = " << area() << ", perimeter = " << perimeter() << '\n';
    }

    double area() const override {
        return fabs((x2 - x1) * (y2 - y1));
    }

    double perimeter() const override {
        return 2.0 * (fabs(x2 - x1) + fabs(y2 - y1));
    }

    string serialize() const override {
        ostringstream oss;
        oss << "Rectangle " << x1 << ' ' << y1 << ' ' << x2 << ' ' << y2;
        return oss.str();
    }
};

class Square : public Shape {
private:
    double x, y, side; // top-left corner (or any reference) + side length
public:
    Square(double _x, double _y, double _side)
        : x(_x), y(_y), side(_side) {}

    void print() const override {
        cout << "Square: top-left (" << x << ", " << y << "), side = " << side
             << " | area = " << area() << ", perimeter = " << perimeter() << '\n';
    }

    double area() const override {
        return fabs(side * side);
    }

    double perimeter() const override {
        return 4.0 * fabs(side);
    }

    string serialize() const override {
        ostringstream oss;
        oss << "Square " << x << ' ' << y << ' ' << side;
        return oss.str();
    }
};

class Circle : public Shape {
private:
    double x, y, r;
public:
    Circle(double _x, double _y, double _r)
        : x(_x), y(_y), r(_r) {}

    void print() const override {
        cout << "Circle: center (" << x << ", " << y << "), radius = " << r
             << " | area = " << area() << ", perimeter = " << perimeter() << '\n';
    }

    double area() const override {
        return M_PI * r * r;
    }

    double perimeter() const override {
        return 2.0 * M_PI * r;
    }

    string serialize() const override {
        ostringstream oss;
        oss << "Circle " << x << ' ' << y << ' ' << r;
        return oss.str();
    }
};

class ShapeStorage {
private:
    Shape** arr;
    int size;
    int capacity;

    void ensureCapacity() {
        if (size < capacity) return;
        int newCapacity = (capacity == 0) ? 2 : capacity * 2;
        Shape** newArr = new Shape*[newCapacity];
        for (int i = 0; i < size; ++i) newArr[i] = arr[i];
        delete[] arr;
        arr = newArr;
        capacity = newCapacity;
    }

    void clearAllShapes() {
        for (int i = 0; i < size; ++i) {
            delete arr[i];
            arr[i] = nullptr;
        }
        size = 0;
    }

public:
    ShapeStorage()
        : arr(nullptr), size(0), capacity(0) {}

    ~ShapeStorage() {
        clearAllShapes();
        delete[] arr;
        arr = nullptr;
        capacity = 0;
    }

    void addShape(Shape* shape) {
        if (!shape) return;
        ensureCapacity();
        arr[size++] = shape;
    }

    void removeAt(int index) {
        if (index < 0 || index >= size) {
            cout << "removeAt: invalid index " << index << '\n';
            return;
        }
        delete arr[index];
        for (int i = index; i < size - 1; ++i) {
            arr[i] = arr[i + 1];
        }
        arr[size - 1] = nullptr;
        --size;
    }

    void printAll() const {
        if (size == 0) {
            cout << "No shapes stored.\n";
            return;
        }
        cout << "Stored shapes (" << size << "):\n";
        for (int i = 0; i < size; ++i) {
            cout << i << ": ";
            arr[i]->print();
        }
    }

    Shape* maxArea() const {
        if (size == 0) return nullptr;
        int idx = 0;
        double best = arr[0]->area();
        for (int i = 1; i < size; ++i) {
            double a = arr[i]->area();
            if (a > best) {
                best = a;
                idx = i;
            }
        }
        return arr[idx];
    }

    Shape* minArea() const {
        if (size == 0) return nullptr;
        int idx = 0;
        double best = arr[0]->area();
        for (int i = 1; i < size; ++i) {
            double a = arr[i]->area();
            if (a < best) {
                best = a;
                idx = i;
            }
        }
        return arr[idx];
    }

    Shape* maxPerimeter() const {
        if (size == 0) return nullptr;
        int idx = 0;
        double best = arr[0]->perimeter();
        for (int i = 1; i < size; ++i) {
            double p = arr[i]->perimeter();
            if (p > best) {
                best = p;
                idx = i;
            }
        }
        return arr[idx];
    }

    void saveToFile(const string& filename) const {
        ofstream ofs(filename);
        if (!ofs) {
            cout << "Cannot open file for writing: " << filename << '\n';
            return;
        }
        for (int i = 0; i < size; ++i) {
            ofs << arr[i]->serialize() << '\n';
        }
        ofs.close();
        cout << "Saved " << size << " shapes to " << filename << '\n';
    }

    void loadFromFile(const string& filename) {
        ifstream ifs(filename);
        if (!ifs) {
            cout << "Cannot open file for reading: " << filename << '\n';
            return;
        }

        clearAllShapes();
        delete[] arr;
        arr = nullptr;
        capacity = 0;

        string line;
        int loaded = 0;
        while (getline(ifs, line)) {
            if (line.empty()) continue;
            istringstream iss(line);
            string type;
            iss >> type;
            if (type == "Rectangle") {
                double x1, y1, x2, y2;
                if (!(iss >> x1 >> y1 >> x2 >> y2)) {
                    cout << "Invalid Rectangle line: " << line << '\n';
                    continue;
                }
                addShape(new Rectangle(x1, y1, x2, y2));
                ++loaded;
            } else if (type == "Square") {
                double x, y, side;
                if (!(iss >> x >> y >> side)) {
                    cout << "Invalid Square line: " << line << '\n';
                    continue;
                }
                addShape(new Square(x, y, side));
                ++loaded;
            } else if (type == "Circle") {
                double x, y, r;
                if (!(iss >> x >> y >> r)) {
                    cout << "Invalid Circle line: " << line << '\n';
                    continue;
                }
                addShape(new Circle(x, y, r));
                ++loaded;
            } else {
                cout << "Unknown shape type in line: " << line << '\n';
            }
        }

        ifs.close();
        cout << "Loaded " << loaded << " shapes from " << filename << '\n';
    }
};

int main() {
    ShapeStorage storage;

    storage.addShape(new Rectangle(0.0, 0.0, 4.0, 3.0)); // area 12
    storage.addShape(new Circle(1.0, 1.0, 2.0));        // area ~12.5664
    storage.addShape(new Square(5.0, 5.0, 2.0));        // area 4

    cout << "After initial additions:\n";
    storage.printAll();
    cout << '\n';

    // Пошуки
    Shape* sMaxArea = storage.maxArea();
    if (sMaxArea) {
        cout << "Shape with max area:\n";
        sMaxArea->print();
    }

    Shape* sMinArea = storage.minArea();
    if (sMinArea) {
        cout << "Shape with min area:\n";
        sMinArea->print();
    }

    Shape* sMaxPerim = storage.maxPerimeter();
    if (sMaxPerim) {
        cout << "Shape with max perimeter:\n";
        sMaxPerim->print();
    }
    cout << '\n';

    cout << "Removing shape at index 1...\n";
    storage.removeAt(1);
    storage.printAll();
    cout << '\n';

    const string filename = "shapes.txt";
    storage.saveToFile(filename);
    cout << '\n';

    cout << "Loading shapes from file (will replace current storage)...\n";
    storage.loadFromFile(filename);
    storage.printAll();
    cout << '\n';

    cout << "Clearing all shapes by removing indices until empty...\n";
    // remove from end to start to be safe
    for (;;) {
        // We'll attempt to remove index 0 until empty
        Shape* maybe = storage.maxArea(); // just to check if empty quickly
        if (!maybe) break;
        storage.removeAt(0);
    }
    storage.printAll();

    cout << "Program finished.\n";
    return 0;
}