#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <numeric>
using namespace std;


class Figure {
public:
    virtual double area() const = 0;
    virtual void print() const = 0;
    virtual ~Figure() = default;

    bool operator<(const Figure& other) const { return area() < other.area(); }
    bool operator>(const Figure& other) const { return area() > other.area(); }
    bool operator<=(const Figure& other) const { return area() <= other.area(); }
    bool operator>=(const Figure& other) const { return area() >= other.area(); }
    bool operator==(const Figure& other) const { return area() == other.area(); }
    bool operator!=(const Figure& other) const { return area() != other.area(); }
};

ostream& operator<<(ostream& os, const Figure& f) {
    f.print();
    return os;
}

class Rectangle : public Figure {
private:
    double x1, y1, x2, y2;
public:
    Rectangle(double a, double b, double c, double d) : x1(a), y1(b), x2(c), y2(d) {}

    double area() const override {
        return abs((x2 - x1) * (y2 - y1));
    }

    void print() const override {
        cout << "Rectangle: (" << x1 << ", " << y1 << ") to (" << x2 << ", " << y2
             << "), area = " << area();
    }
};

class Square : public Figure {
private:
    double x, y, side;
public:
    Square(double a, double b, double s) : x(a), y(b), side(s) {}

    double area() const override {
        return side * side;
    }

    void print() const override {
        cout << "Square: (" << x << ", " << y << "), side = " << side
             << ", area = " << area();
    }
};

class Circle : public Figure {
private:
    double x, y, r;
public:
    Circle(double a, double b, double rad) : x(a), y(b), r(rad) {}

    double area() const override {
        return 3.141592653589793 * r * r;
    }

    void print() const override {
        cout << "Circle: center (" << x << ", " << y
             << "), radius = " << r << ", area = " << area();
    }
};


class Drib {
private:
    long long num;
    long long den;

    void normalize() {
        if (den < 0) { den = -den; num = -num; }
        long long g = gcd(llabs(num), llabs(den));
        num /= g;
        den /= g;
    }

public:
    Drib(long long n = 0, long long d = 1) : num(n), den(d) {
        normalize();
    }

    Drib(long long whole, long long n, long long d) {
        num = whole * d + n;
        den = d;
        normalize();
    }

    operator float() const {
        return float(num) / float(den);
    }

    Drib operator+(const Drib& o) const {
        return Drib(num * o.den + o.num * den, den * o.den);
    }
    Drib operator-(const Drib& o) const {
        return Drib(num * o.den - o.num * den, den * o.den);
    }
    Drib operator*(const Drib& o) const {
        return Drib(num * o.num, den * o.den);
    }
    Drib operator/(const Drib& o) const {
        return Drib(num * o.den, den * o.num);
    }

    Drib& operator+=(float f) {
        Drib temp((long long)(f * den), den);
        *this = *this + temp;
        normalize();
        return *this;
    }

    bool operator<(const Drib& o) const { return num * o.den < o.num * den; }
    bool operator>(const Drib& o) const { return num * o.den > o.num * den; }
    bool operator<=(const Drib& o) const { return !(*this > o); }
    bool operator>=(const Drib& o) const { return !(*this < o); }
    bool operator==(const Drib& o) const { return num == o.num && den == o.den; }
    bool operator!=(const Drib& o) const { return !(*this == o); }

    friend ostream& operator<<(ostream& os, const Drib& d) {
        long long whole = d.num / d.den;
        long long rem = llabs(d.num % d.den);

        if (rem == 0)
            os << whole;
        else if (whole != 0)
            os << whole << "_" << rem << "/" << d.den;
        else
            os << d.num << "/" << d.den;

        return os;
    }
};


int main() {
    cout << "=== TASK 1: FIGURES ===\n";

    vector<unique_ptr<Figure>> figures;
    figures.push_back(make_unique<Rectangle>(0, 0, 4, 3));
    figures.push_back(make_unique<Square>(0, 0, 5));
    figures.push_back(make_unique<Circle>(0, 0, 3));

    cout << "Figures:\n";
    for (auto& f : figures)
        cout << *f << endl;

    cout << "\nCompare first two:\n";
    cout << (*figures[0] < *figures[1]) << endl;
    cout << (*figures[0] > *figures[1]) << endl;
    cout << (*figures[0] == *figures[1]) << endl;


    cout << "\n=== TASK 2: DRIB ===\n";

    Drib a(1, 3);
    Drib b(5, 1, 3);

    Drib res = a + b;
    cout << "res = " << res << endl;

    cout << a << "<" << b << " : " << (a < b) << endl;
    cout << a << ">" << b << " : " << (a > b) << endl;
    cout << a << "==" << b << " : " << (a == b) << endl;
    cout << a << "!=" << b << " : " << (a != b) << endl;

    float f = (float)res;
    cout << "float(res) = " << f << endl;

    a += 0.5f;
    cout << "a after += 0.5 = " << a << endl;

    return 0;
}