#include <iostream>
using namespace std;

template<typename T>
class PriorityQueue {
private:
    struct Node {
        T data;
        int priority;
        Node* next;
        Node(const T& d, int p) : data(d), priority(p), next(nullptr) {}
    };

    Node* head;
    int count;

public:
    PriorityQueue() : head(nullptr), count(0) {}

    ~PriorityQueue() {
        clear();
    }

    bool empty() const {
        return head == nullptr;
    }

    int size() const {
        return count;
    }

    void push(const T& item, int priority) {
        Node* node = new Node(item, priority);

        if (!head || priority > head->priority) {
            node->next = head;
            head = node;
        } else {
            Node* cur = head;
            while (cur->next && cur->next->priority >= priority)
                cur = cur->next;
            node->next = cur->next;
            cur->next = node;
        }

        count++;
    }

    T top() const {
        if (!head) throw runtime_error("Queue is empty");
        return head->data;
    }

    T pop() {
        if (!head) throw runtime_error("Queue is empty");
        Node* temp = head;
        T value = temp->data;
        head = head->next;
        delete temp;
        count--;
        return value;
    }

    void clear() {
        while (!empty()) pop();
    }

    void removeWithPriority(int priority) {
        while (head && head->priority == priority) {
            Node* temp = head;
            head = head->next;
            delete temp;
            count--;
        }

        Node* cur = head;

        while (cur && cur->next) {
            if (cur->next->priority == priority) {
                Node* temp = cur->next;
                cur->next = temp->next;
                delete temp;
                count--;
            } else {
                cur = cur->next;
            }
        }
    }
};

int main() {
    PriorityQueue<string> pq;

    pq.push("Low", 1);
    pq.push("Medium", 5);
    pq.push("High", 10);
    pq.push("Urgent", 10);
    pq.push("Normal", 3);

    cout << "Top: " << pq.top() << endl;

    cout << "Попробуємо видалити всі з пріоритетом 10\n";
    pq.removeWithPriority(10);

    while (!pq.empty()) {
        cout << pq.pop() << endl;
    }

    return 0;
}
