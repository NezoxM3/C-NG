#include <iostream>
#include <vector>
#include <memory>
using namespace std;

class Shape {
public:
    virtual void draw() const = 0;
    virtual ~Shape() {}
};

class Circle : public Shape {
    static int created;
    static int deleted;
public:
    Circle() { created++; }
    ~Circle() override { deleted++; }

    void draw() const override {
        cout << "Drawing Circle\n";
    }

    static int getCreated() { return created; }
    static int getDeleted() { return deleted; }
};
int Circle::created = 0;
int Circle::deleted = 0;

class Rectangle : public Shape {
    static int created;
    static int deleted;
public:
    Rectangle() { created++; }
    ~Rectangle() override { deleted++; }

    void draw() const override {
        cout << "Drawing Rectangle\n";
    }

    static int getCreated() { return created; }
    static int getDeleted() { return deleted; }
};
int Rectangle::created = 0;
int Rectangle::deleted = 0;

class ShapeManager {
    vector<shared_ptr<Shape>> shapes;

public:
    ShapeManager() = default;

    ShapeManager(const ShapeManager& other) {
        shapes = other.shapes;
        cout << "[ShapeManager copied]\n";
    }

    void addShape(shared_ptr<Shape> s) {
        shapes.push_back(s);
    }

    void drawAll() const {
        for (auto& s : shapes)
            s->draw();
    }

    void clear() {
        shapes.clear();
    }

    static void showStats() {
        cout << "\n==============================\n";
        cout << "        Shape Statistics\n";
        cout << "==============================\n";
        cout << "Circles created:    " << Circle::getCreated()
             << " | deleted: " << Circle::getDeleted() << endl;
        cout << "Rectangles created: " << Rectangle::getCreated()
             << " | deleted: " << Rectangle::getDeleted() << endl;
        cout << "==============================\n\n";
    }
};


int main() {
    cout << "=== DEMO: Creating manager m1 ===\n";
    ShapeManager m1;
    m1.addShape(make_shared<Circle>());
    m1.addShape(make_shared<Rectangle>());
    m1.addShape(make_shared<Circle>());

    cout << "\nShapes in m1:\n";
    m1.drawAll();

    ShapeManager::showStats();

    cout << "=== Copying manager to m2 ===\n";
    ShapeManager m2 = m1;

    cout << "\nShapes in m2 (copy):\n";
    m2.drawAll();

    cout << "\nSwitching between managers...\n";
    cout << "Manager m1:\n"; m1.drawAll();
    cout << "Manager m2:\n"; m2.drawAll();

    ShapeManager::showStats();

    cout << "=== Clearing m1 and m2 ===\n";
    m1.clear();
    m2.clear();

    ShapeManager::showStats();

    cout << "=== End of program ===\n";
    return 0;
}