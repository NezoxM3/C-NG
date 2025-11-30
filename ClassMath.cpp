#include <iostream>
#include <string>
#include <stdexcept>
#include <climits>
using namespace std;

class InvalidInput : public runtime_error {
public:
    InvalidInput(const string& msg) : runtime_error(msg) {}
};

class Expression {
private:
    string expr;
    long long a, b;
    char op;

    bool isDigit(char c) { return c >= '0' && c <= '9'; }

    long long toNumber(const string& s) {
        if (s.empty()) throw InvalidInput("empty number");

        long long val = 0;
        for (char c : s) {
            if (!isDigit(c)) throw InvalidInput("invalid number");
            val = val * 10 + (c - '0');
            if (val > INT_MAX) throw InvalidInput("number too large");
        }
        return val;
    }

public:
    Expression(const string& s) : expr(s) {}

    void parse() {
        if (expr.empty()) throw InvalidInput("empty expression");
        if (expr.back() != '=') throw InvalidInput("missing '=' at end");

        string core = expr.substr(0, expr.size() - 1);

        int posOp = -1;
        for (int i = 0; i < core.size(); i++) {
            if (core[i] == '+' || core[i] == '-' || core[i] == '*' || core[i] == '/') {
                posOp = i;
                op = core[i];
                break;
            }
        }

        if (posOp == -1) throw InvalidInput("missing operator");

        string left = core.substr(0, posOp);
        string right = core.substr(posOp + 1);

        if (left.empty() || right.empty())
            throw InvalidInput("invalid format");

        a = toNumber(left);
        b = toNumber(right);
    }

    int evaluate() {
        switch (op) {
            case '+': return a + b;
            case '-': return a - b;
            case '*': return (long long)a * b;
            case '/':
                if (b == 0) throw InvalidInput("division by zero");
                return a / b;
        }
        throw InvalidInput("unknown operator");
    }
};

int main() {
    try {
        string input;
        cout << "Enter expression: ";
        cin >> input;

        Expression e(input);
        e.parse();
        int result = e.evaluate();

        cout << input << result << endl;
    }
    catch (const InvalidInput& ex) {
        cout << "Error: " << ex.what() << endl;
    }
    catch (...) {
        cout << "Unknown error" << endl;
    }

    return 0;
}
