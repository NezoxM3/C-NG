#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

class StringTools {
private:
    string value;

public:
    StringTools() {}
    StringTools(const string& s) : value(s) {}

    void set(const string& s) { value = s; }
    string get() const { return value; }

    string concat(const string& other) {
        return value + " " + other;
    }

    int length() const {
        return value.length();
    }

    bool containsA() const {
        return value.find('a') != string::npos;
    }

    string replaceOnce(const string& from, const string& to) {
        string temp = value;
        size_t pos = temp.find(from);
        if (pos != string::npos)
            temp.replace(pos, from.length(), to);
        return temp;
    }

    string toUpper() const {
        string temp = value;
        transform(temp.begin(), temp.end(), temp.begin(), ::toupper);
        return temp;
    }

    string toLower() const {
        string temp = value;
        transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
        return temp;
    }

    int countVowels() const {
        string vowels = "aeiouяаеиі";
        int count = 0;
        for (char c : value)
            if (vowels.find(tolower(c)) != string::npos)
                count++;
        return count;
    }

    string removeSpaces() const {
        string temp = value;
        temp.erase(remove(temp.begin(), temp.end(), ' '), temp.end());
        return temp;
    }

    bool isPalindrome() const {
        string t = "";
        for (char c : value)
            if (c != ' ') t += tolower(c);

        string r = t;
        reverse(r.begin(), r.end());
        return t == r;
    }
};

int main() {
    StringTools st, helper;
    string s1, s2, s, from, to;

    cout << "Завдання 1: Конкатенація\n";
    cout << "Введіть перше слово: ";
    cin >> s1;
    cout << "Введіть друге слово: ";
    cin >> s2;
    st.set(s1);
    cout << "Результат: " << st.concat(s2) << "\n\n";
    cin.ignore();

    cout << "Завдання 2: Довжина + 'a'\n";
    cout << "Введіть рядок: ";
    getline(cin, s);
    st.set(s);
    cout << "Довжина: " << st.length() << endl;
    cout << "Містить 'a': " << (st.containsA() ? "так" : "ні") << "\n\n";

    cout << "Завдання 3: Заміна\n";
    cout << "Введіть текст: ";
    getline(cin, s);
    st.set(s);
    cout << "Що замінити: ";
    getline(cin, from);
    cout << "На що замінити: ";
    getline(cin, to);
    cout << "Початковий: " << st.get() << endl;
    cout << "Після заміни: " << st.replaceOnce(from, to) << "\n\n";

    cout << "Завдання 4: Регістр\n";
    cout << "Введіть рядок: ";
    getline(cin, s);
    st.set(s);
    cout << "Великі: " << st.toUpper() << endl;
    cout << "Малі: " << st.toLower() << "\n\n";

    cout << "Завдання 5: Голосні\n";
    cout << "Введіть рядок: ";
    getline(cin, s);
    st.set(s);
    cout << "Голосних: " << st.countVowels() << "\n\n";

    cout << "Завдання 6: Видалення пробілів\n";
    cout << "Введіть рядок: ";
    getline(cin, s);
    st.set(s);
    cout << "Без пробілів: " << st.removeSpaces() << "\n\n";

    cout << "Завдання 7: Паліндром\n";
    cout << "Введіть рядок: ";
    getline(cin, s);
    st.set(s);
    cout << "Паліндром: " << (st.isPalindrome() ? "так" : "ні") << "\n\n";

    return 0;
}