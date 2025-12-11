#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
#include <functional>
#include <random>
#include "consoleCommands.h"
#include <conio.h>
#include <string>
#include <fstream>

using namespace std;

class User {};

class QuestionOption {
private:
    string optionText;
    bool isCorrect;
public:
    QuestionOption() : optionText(""), isCorrect(false) {}
    QuestionOption(const string& text, bool correct) : optionText(text), isCorrect(correct) {}

    void setOptionText(const string& text) { optionText = text; }
    string getOptionText() const { return optionText; }
    void setIsCorrect(bool correct) { isCorrect = correct; }
    bool getIsCorrect() const { return isCorrect; }

    friend ostream& operator<<(ostream& os, const QuestionOption& option) {
        os << option.optionText << endl << (option.isCorrect ? 1 : 0) << endl;
        return os;
    }
    friend istream& operator>>(istream& is, QuestionOption& option) {
        getline(is, option.optionText);
        int val; is >> val;
        option.isCorrect = (val != 0);
        is.ignore();
        return is;
    }
};

class Question {
private:
    string questionText;
    vector<shared_ptr<QuestionOption>> options;
public:
    Question() : questionText("") {}
    Question(const string& text) : questionText(text) {}

    void setQuestionText(const string& text) { questionText = text; }
    string getQuestionText() const { return questionText; }

    void addOption(const shared_ptr<QuestionOption>& option) { options.push_back(option); }
    void deleteOption(size_t index) { if (index < options.size()) options.erase(options.begin() + index); }

    shared_ptr<QuestionOption> operator[](size_t index) const { return options[index]; }
    size_t getOptionsCount() const { return options.size(); }

    friend ostream& operator<<(ostream& os, const Question& question) {
        os << question.questionText << endl;
        os << question.options.size() << endl;
        for (const auto& option : question.options) os << *option;
        return os;
    }
    friend istream& operator>>(istream& is, Question& question) {
        getline(is, question.questionText);
        size_t count; is >> count; is.ignore();
        for (size_t i = 0; i < count; i++) {
            auto opt = make_shared<QuestionOption>();
            is >> *opt;
            question.options.push_back(opt);
        }
        return is;
    }
};

class Test {
private:
    string testName;
    vector<shared_ptr<Question>> questions;
public:
    Test() : testName("") {}
    Test(const string& name) : testName(name) {}

    void setTestName(const string& name) { testName = name; }
    string getTestName() const { return testName; }

    void addQuestion(const shared_ptr<Question>& q) { questions.push_back(q); }
    void removeQuestion(size_t index) { if (index < questions.size()) questions.erase(questions.begin() + index); }

    shared_ptr<Question> getQuestion(size_t index) const { return questions[index]; }
    size_t getQuestionsCount() const { return questions.size(); }

    friend ostream& operator<<(ostream& os, const Test& test) {
        os << test.testName << endl;
        os << test.questions.size() << endl;
        for (auto& q : test.questions) os << *q;
        return os;
    }
    friend istream& operator>>(istream& is, Test& test) {
        getline(is, test.testName);
        size_t count; is >> count; is.ignore();
        for (size_t i = 0; i < count; i++) {
            auto q = make_shared<Question>();
            is >> *q;
            test.questions.push_back(q);
        }
        return is;
    }
};

shared_ptr<Test> loadTestFromFile(const string& filename) {
    ifstream in(filename);
    if (!in.is_open()) throw runtime_error("Cannot open file");
    auto t = make_shared<Test>();
    in >> *t;
    return t;
}

void saveTestToFile(const shared_ptr<Test>& test, const string& filename) {
    ofstream out(filename);
    if (!out.is_open()) throw runtime_error("Cannot open file");
    out << *test;
}

class Menu {
private:
    string title = "";
    vector<string> options;
    int selectedIndex;
    void draw() {
        int w, h;
        GetConsoleSize(w, h);
        int x = (w - 20) / 2;
        int y = (h - options.size()) / 2;
        SetCursorPosition(x, y - 2);
        if (!title.empty()) cout << title << endl;
        drawConsoleFrameAtPosition(x, y, 20, options.size() + 2, WHITE, DARK_BLUE);
        for (size_t i = 0; i < options.size(); i++) {
            SetCursorPosition(x + 2, y + i + 1);
            if (i == selectedIndex) { SetColor(BLACK, WHITE); cout << options[i]; SetColor(WHITE, DARK_BLUE); }
            else cout << options[i];
        }
        SetColor(WHITE, BLACK);
    }
public:
    Menu(const vector<string>& opts) { options = opts; selectedIndex = 0; }
    void addOption(const string& opt) { options.push_back(opt); }
    void setTitle(const string& t) { title = t; }

    void run() {
        system("cls");
        while (true) {
            draw();
            int ch = _getch();
            if (ch == 224) {
                ch = _getch();
                if (ch == 72) selectedIndex = (selectedIndex - 1 + options.size()) % options.size();
                else if (ch == 80) selectedIndex = (selectedIndex + 1) % options.size();
            }
            else if (ch == 13) break;
        }
        system("cls");
    }
    int getSelectedIndex() const { return selectedIndex; }
    string getSelectedValue() const { return options[selectedIndex]; }
};

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    string filename = "test.txt";
    auto test = loadTestFromFile(filename);

    while (true) {
        auto m = make_shared<Menu>(Menu({
            "Редагувати назву",
            "Додати питання",
            "Видалити питання",
            "Редагувати питання",
            "Зберегти у файл",
            "Вийти"
        }));

        m->setTitle("Test Editor - " + test->getTestName());
        m->run();

        switch (m->getSelectedIndex()) {
        case 0: {
            cout << "Enter new title: ";
            string s; getline(cin, s);
            test->setTestName(s);
            break;
        }
        case 1: {
            cout << "Введіть питання: ";
            string q; getline(cin, q);
            auto question = make_shared<Question>(q);

            while (true) {
                cout << "Варіант (або 'стоп'): ";
                string t; getline(cin, t);
                if (t == "стоп") break;
                cout << "Правильний? (т/н): ";
                char c; cin >> c; cin.ignore();
                question->addOption(make_shared<QuestionOption>(t, (c == 'т' || c == 'Т')));
            }

            test->addQuestion(question);
            break;
        }
        case 2: {
            auto dm = make_shared<Menu>(Menu({}));
            dm->setTitle("Select question:");
            for (size_t i = 0; i < test->getQuestionsCount(); i++)
                dm->addOption(test->getQuestion(i)->getQuestionText());
            dm->addOption("Назад");
            dm->run();
            if (dm->getSelectedValue() != "Назад")
                test->removeQuestion(dm->getSelectedIndex());
            break;
        }
        case 3: {
            auto qm = make_shared<Menu>(Menu({}));
            qm->setTitle("Select question:");
            for (size_t i = 0; i < test->getQuestionsCount(); i++)
                qm->addOption(test->getQuestion(i)->getQuestionText());
            qm->addOption("Назад");
            qm->run();
            if (qm->getSelectedValue() == "Назад") break;

            size_t qi = qm->getSelectedIndex();
            auto q = test->getQuestion(qi);

            auto em = make_shared<Menu>(Menu({
                "Змінити текст питання",
                "Додати варіант",
                "Видалити варіант",
                "Редагувати варіант",
                "Назад"
            }));
            em->setTitle("Редагування питання");
            em->run();

            switch (em->getSelectedIndex()) {
            case 0: {
                cout << "Новий текст: ";
                string nt; getline(cin, nt);
                q->setQuestionText(nt);
                break;
            }
            case 1: {
                cout << "Текст: ";
                string t; getline(cin, t);
                cout << "Правильний? (т/н): ";
                char c; cin >> c; cin.ignore();
                q->addOption(make_shared<QuestionOption>(t, c == 'т' || c == 'Т'));
                break;
            }
            case 2: {
                auto om = make_shared<Menu>(Menu({}));
                for (size_t i = 0; i < q->getOptionsCount(); i++)
                    om->addOption(q->operator[](i)->getOptionText());
                om->addOption("Назад");
                om->run();
                if (om->getSelectedValue() != "Назад")
                    q->deleteOption(om->getSelectedIndex());
                break;
            }
            case 3: {
                auto om = make_shared<Menu>(Menu({}));
                for (size_t i = 0; i < q->getOptionsCount(); i++)
                    om->addOption(q->operator[](i)->getOptionText());
                om->addOption("Назад");
                om->run();
                if (om->getSelectedValue() == "Назад") break;

                size_t oi = om->getSelectedIndex();
                auto opt = q->operator[](oi);

                cout << "Новий текст: ";
                string nt; getline(cin, nt);
                opt->setOptionText(nt);

                cout << "Правильний? (т/н): ";
                char c; cin >> c; cin.ignore();
                opt->setIsCorrect(c == 'т' || c == 'Т');
                break;
            }
            }
            break;
        }
        case 4:
            saveTestToFile(test, filename);
            break;
        case 5:
            return 0;
        }
    }
}
