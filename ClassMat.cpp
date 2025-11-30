#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
using namespace std;

const int N = 10;

class Matrix {
private:
    int data[N][N];
public:
    Matrix() { generate(); }

    void generate() {
        vector<int> nums(101);
        for (int i = 0; i < 101; i++) nums[i] = i;
        random_shuffle(nums.begin(), nums.end());
        int idx = 0;
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                data[i][j] = nums[idx++];
    }

    int get(int i, int j) const {
        return data[i][j];
    }

    void print() const {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++)
                cout << data[i][j] << "\t";
            cout << "\n";
        }
        cout << endl;
    }
};

class PathFinder {
private:
    const Matrix &m;
    int dpMin[N][N];
    int dpMax[N][N];
public:
    PathFinder(const Matrix &matrix) : m(matrix) {}

    void compute() {
        dpMin[0][0] = dpMax[0][0] = m.get(0, 0);

        for (int j = 1; j < N; j++) {
            dpMin[0][j] = dpMin[0][j - 1] + m.get(0, j);
            dpMax[0][j] = dpMax[0][j - 1] + m.get(0, j);
        }

        for (int i = 1; i < N; i++) {
            dpMin[i][0] = dpMin[i - 1][0] + m.get(i, 0);
            dpMax[i][0] = dpMax[i - 1][0] + m.get(i, 0);
        }

        for (int i = 1; i < N; i++)
            for (int j = 1; j < N; j++) {
                dpMin[i][j] = min(dpMin[i - 1][j], dpMin[i][j - 1]) + m.get(i, j);
                dpMax[i][j] = max(dpMax[i - 1][j], dpMax[i][j - 1]) + m.get(i, j);
            }
    }

    int getMinSum() const { return dpMin[N - 1][N - 1]; }
    int getMaxSum() const { return dpMax[N - 1][N - 1]; }
};

int main() {
    srand(time(0));
    Matrix matrix;
    matrix.print();
    PathFinder pf(matrix);
    pf.compute();
    cout << "Min sum: " << pf.getMinSum() << endl;
    cout << "Max sum: " << pf.getMaxSum() << endl;
    return 0;
}
