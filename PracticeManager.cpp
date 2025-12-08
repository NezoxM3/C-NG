#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

class Answer {
public:
    string text;
    Answer(const string& t) : text(t) {}
};

class Question {
public:
    string text;
    vector<shared_ptr<Answer>> answers;

    Question(const string& t) : text(t) {}

    void addAnswer(const string& a) {
        answers.push_back(make_shared<Answer>(a));
    }
};

class Test {
public:
    string name;
    vector<shared_ptr<Question>> questions;

    Test(const string& n = "Untitled") : name(n) {}

    void addQuestion(const string& q) {
        questions.push_back(make_shared<Question>(q));
    }

    // ---------- SAVE TEST ----------
    void saveToFile(const string& filename) {
        ofstream out(filename);
        out << name << "\n";
        out << questions.size() << "\n";
        for (auto& q : questions) {
            out << q->text << "\n";
            out << q->answers.size() << "\n";
            for (auto& ans : q->answers)
                out << ans->text << "\n";
        }
        out.close();
    }

    // ---------- LOAD TEST ----------
    static shared_ptr<Test> loadFromFile(const string& filename) {
        ifstream in(filename);
        if (!in.is_open()) return nullptr;

        string name;
        getline(in, name);

        auto test = make_shared<Test>(name);
        int qCount;
        in >> qCount;
        in.ignore();

        for (int i = 0; i < qCount; i++) {
            string qtext;
            getline(in, qtext);
            auto q = make_shared<Question>(qtext);

            int aCount;
            in >> aCount;
            in.ignore();

            for (int j = 0; j < aCount; j++) {
                string ans;
                getline(in, ans);
                q->addAnswer(ans);
            }
            test->questions.push_back(q);
        }
        return test;
    }
};


class Editor {
public:
    static void editTest(shared_ptr<Test> test) {
        while (true) {
            cout << "\n--- Test Editor: " << test->name << " ---\n";
            cout << "1. Change test name\n";
            cout << "2. Add question\n";
            cout << "3. Edit question\n";
            cout << "4. Save test\n";
            cout << "0. Back\n";

            int c;
            cin >> c;
            cin.ignore();

            if (c == 0) break;

            if (c == 1) {
                cout << "Enter new name: ";
                getline(cin, test->name);
            }
            else if (c == 2) {
                cout << "Enter question text: ";
                string q;
                getline(cin, q);
                test->addQuestion(q);
            }
            else if (c == 3) {
                editQuestion(test);
            }
            else if (c == 4) {
                cout << "Enter filename to save (example: test1.txt): ";
                string f;
                getline(cin, f);
                test->saveToFile(f);
                cout << "Saved.\n";
            }
        }
    }

    static void editQuestion(shared_ptr<Test> test) {
        if (test->questions.empty()) {
            cout << "No questions.\n";
            return;
        }

        cout << "Questions:\n";
        for (int i = 0; i < test->questions.size(); i++) {
            cout << i + 1 << ". " << test->questions[i]->text << "\n";
        }

        cout << "Select question: ";
        int idx;
        cin >> idx;
        cin.ignore();

        idx--;
        if (idx < 0 || idx >= test->questions.size()) return;

        auto q = test->questions[idx];

        while (true) {
            cout << "\nEditing Question: " << q->text << "\n";
            cout << "1. Change text\n";
            cout << "2. Add answer\n";
            cout << "3. Edit answer\n";
            cout << "0. Back\n";

            int c;
            cin >> c;
            cin.ignore();

            if (c == 0) break;

            if (c == 1) {
                cout << "New question text: ";
                getline(cin, q->text);
            }
            else if (c == 2) {
                cout << "Enter answer text: ";
                string a;
                getline(cin, a);
                q->addAnswer(a);
            }
            else if (c == 3) {
                editAnswer(q);
            }
        }
    }

    static void editAnswer(shared_ptr<Question> q) {
        if (q->answers.empty()) {
            cout << "No answers.\n";
            return;
        }

        for (int i = 0; i < q->answers.size(); i++) {
            cout << i + 1 << ". " << q->answers[i]->text << "\n";
        }

        cout << "Select answer: ";
        int idx;
        cin >> idx;
        cin.ignore();

        idx--;
        if (idx < 0 || idx >= q->answers.size()) return;

        cout << "New answer text: ";
        getline(cin, q->answers[idx]->text);
    }
};


class TestManager {
public:

    void run() {
        while (true) {
            cout << "\n--- MAIN MENU ---\n";
            cout << "1. Create test\n";
            cout << "2. Load test\n";
            cout << "0. Exit\n";

            int c;
            cin >> c;
            cin.ignore();

            if (c == 0) break;
            if (c == 1) createTest();
            if (c == 2) loadTest();
        }
    }

    void createTest() {
        cout << "Enter test name: ";
        string n;
        getline(cin, n);

        auto test = make_shared<Test>(n);
        Editor::editTest(test);
    }

    void loadTest() {
        cout << "\nAvailable .txt files:\n";

        vector<string> files;
        for (auto& p : fs::directory_iterator(".")) {
            if (p.path().extension() == ".txt") {
                files.push_back(p.path().filename().string());
            }
        }

        if (files.empty()) {
            cout << "No .txt files found.\n";
            return;
        }

        for (int i = 0; i < files.size(); i++) {
            cout << i + 1 << ". " << files[i] << "\n";
        }

        cout << "Select file: ";
        int idx;
        cin >> idx;
        cin.ignore();

        idx--;
        if (idx < 0 || idx >= files.size()) return;

        auto test = Test::loadFromFile(files[idx]);
        if (!test) {
            cout << "Failed to load file.\n";
            return;
        }

        Editor::editTest(test);
    }
};


int main() {
    TestManager manager;
    manager.run();
    return 0;
}
