#include <iostream>
#include <string>
using namespace std;

template<typename T>
class Queue {
private:
    struct Node {
        T data;
        Node* next;
        Node(T d) : data(d), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int count;

public:
    Queue() : head(nullptr), tail(nullptr), count(0) {}

    ~Queue() {
        while (!isEmpty()) dequeue();
    }

    void enqueue(const T& value) {
        Node* node = new Node(value);
        if (isEmpty()) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
        count++;
    }

    void dequeue() {
        if (isEmpty()) return;
        Node* tmp = head;
        head = head->next;
        delete tmp;
        count--;
        if (isEmpty()) tail = nullptr;
    }

    T front() const {
        return head->data;
    }

    bool isEmpty() const {
        return count == 0;
    }

    int size() const {
        return count;
    }
};

int main() {
    Queue<string> q;
    int n;

    cout << "Скільки рядків додати? ";
    cin >> n;
    cin.ignore();

    for (int i = 0; i < n; i++) {
        string s;
        cout << "Введіть рядок " << i + 1 << ": ";
        getline(cin, s);
        q.enqueue(s);
    }

    cout << "\nВивід із черги:\n";
    while (!q.isEmpty()) {
        cout << q.front() << endl;
        q.dequeue();
    }

    return 0;
}
