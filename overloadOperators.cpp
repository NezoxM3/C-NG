#include <iostream>
using namespace std;

class DynArray {
private:
    int* data;
    int size;

public:
    DynArray() : data(nullptr), size(0) {}

    DynArray(int n) : size(n) {
        data = new int[size];
        for (int i = 0; i < size; i++) data[i] = 0;
    }

    DynArray(const DynArray& other) : size(other.size) {
        data = new int[size];
        for (int i = 0; i < size; i++) data[i] = other.data[i];
    }

    ~DynArray() {
        delete[] data;
    }

    void push_back(int value) {
        int* newData = new int[size + 1];
        for (int i = 0; i < size; i++) newData[i] = data[i];
        newData[size] = value;
        delete[] data;
        data = newData;
        size++;
    }

    void pop_back() {
        if (size == 0) return;
        int* newData = (size == 1 ? nullptr : new int[size - 1]);
        for (int i = 0; i < size - 1; i++) newData[i] = data[i];
        delete[] data;
        data = newData;
        size--;
    }

    int getSize() const { return size; }

    void insert(int index, int value) {
        if (index < 0 || index > size) return;
        int* newData = new int[size + 1];
        for (int i = 0; i < index; i++) newData[i] = data[i];
        newData[index] = value;
        for (int i = index; i < size; i++) newData[i + 1] = data[i];
        delete[] data;
        data = newData;
        size++;
    }

    void erase(int index) {
        if (index < 0 || index >= size) return;
        int* newData = (size == 1 ? nullptr : new int[size - 1]);
        for (int i = 0; i < index; i++) newData[i] = data[i];
        for (int i = index + 1; i < size; i++) newData[i - 1] = data[i];
        delete[] data;
        data = newData;
        size--;
    }

    int& operator[](int index) { return data[index]; }

    bool operator==(const DynArray& other) const {
        if (size != other.size) return false;
        for (int i = 0; i < size; i++)
            if (data[i] != other.data[i]) return false;
        return true;
    }

    bool operator!=(const DynArray& other) const { return !(*this == other); }
    bool operator<(const DynArray& other) const { return size < other.size; }
    bool operator>(const DynArray& other) const { return size > other.size; }
    bool operator<=(const DynArray& other) const { return size <= other.size; }
    bool operator>=(const DynArray& other) const { return size >= other.size; }

    DynArray operator+(const DynArray& other) const {
        DynArray res(size + other.size);
        for (int i = 0; i < size; i++) res.data[i] = data[i];
        for (int i = 0; i < other.size; i++) res.data[size + i] = other.data[i];
        return res;
    }

    DynArray operator+(int value) const {
        DynArray res(*this);
        res.push_back(value);
        return res;
    }

    DynArray& operator+=(int value) {
        push_back(value);
        return *this;
    }

    DynArray operator*(int n) const {
        DynArray res;
        for (int i = 0; i < n; i++)
            res = res + *this;
        return res;
    }

    DynArray& operator++() {
        push_back(0);
        return *this;
    }

    DynArray& operator--() {
        pop_back();
        return *this;
    }

    DynArray operator-(const DynArray& other) const {
        DynArray res;
        for (int i = 0; i < size; i++) {
            bool found = false;
            for (int j = 0; j < other.size; j++)
                if (data[i] == other.data[j])
                    found = true;
            if (!found) res.push_back(data[i]);
        }
        return res;
    }

    DynArray& operator-=(const DynArray& other) {
        *this = *this - other;
        return *this;
    }

    friend ostream& operator<<(ostream& os, const DynArray& arr) {
        os << "[ ";
        for (int i = 0; i < arr.size; i++) os << arr.data[i] << " ";
        os << "]";
        return os;
    }
};

int main() {
    DynArray a;
    a.push_back(1);
    a.push_back(2);
    a.push_back(3);
    a.push_back(4);

    DynArray b;
    b.push_back(2);
    b.push_back(4);

    cout << "a = " << a << endl;
    cout << "b = " << b << endl;

    DynArray c = a - b;
    cout << "a - b = " << c << endl;

    a -= b;
    cout << "a after a -= b: " << a << endl;

    return 0;
}