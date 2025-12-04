#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>

using namespace std;

class Student {
public:
    string lastName;
    string firstName;

    Student(string ln, string fn) : lastName(ln), firstName(fn) {}
};


void task1() {
    vector<int> nums = {1, 2, 3, 4, 5, 6, 8, 11, 22};
    vector<int> evenNums;

    copy_if(nums.begin(), nums.end(), back_inserter(evenNums),
            [](int x){ return x % 2 == 0; });

    cout << "Парні числа: ";
    for (int x : evenNums) cout << x << " ";
    cout << "\n";
}

void task2() {
    vector<string> words = {"apple", "Banana", "Apricot", "avocado", "cat"};

    int countA = count_if(words.begin(), words.end(),
                          [](const string& w){
                              return !w.empty() && tolower(w[0]) == 'a';
                          });

    cout << "Слів на 'a': " << countA << "\n";
}

void task3() {
    vector<int> nums = {1, 2, 3, 4, 5};
    vector<int> squares(nums.size());

    transform(nums.begin(), nums.end(), squares.begin(),
             [](int x){ return x * x; });

    cout << "Квадрати: ";
    for (int x : squares) cout << x << " ";
    cout << "\n";
}

void task4() {
    vector<int> nums = {-5, 3, -1, 8, -2, 10};

    nums.erase(remove_if(nums.begin(), nums.end(),
                         [](int x){ return x < 0; }),
               nums.end());

    cout << "Без від’ємних: ";
    for (int x : nums) cout << x << " ";
    cout << "\n";
}

void task5() {
    vector<int> nums = {1, 2, 3, 4, 5};

    bool allPositive = all_of(nums.begin(), nums.end(),
                              [](int x){ return x > 0; });

    cout << (allPositive ? "Всі додатні" : "Не всі додатні") << "\n";
}

void task6() {
    vector<Student> students = {
        {"Ivanov", "Petro"},
        {"Sudakov", "Georgiy"},
        {"Bondarenko", "Oleh"},
        {"Adamuyk", "Viktor"}
    };

    sort(students.begin(), students.end(),
         [](const Student& a, const Student& b){
             return a.lastName < b.lastName;
         });

    cout << "Відсортовані студенти:\n";
    for (auto& s : students)
        cout << s.lastName << " " << s.firstName << "\n";
}

void task7() {
    vector<int> nums = {10, 50, 99, 120, 150};

    auto it = find_if(nums.begin(), nums.end(),
                      [](int x){ return x > 100; });

    if (it != nums.end())
        cout << "Перше >100: " << *it << "\n";
    else
        cout << "Немає таких чисел\n";
}

int main() {
    int choice;

    do {
        cout << "\n===== МЕНЮ =====\n";
        cout << "1. Вибрати парні числа\n";
        cout << "2. Порахувати слова, що починаються на 'a'\n";
        cout << "3. Перетворити числа на квадрати\n";
        cout << "4. Видалити від’ємні числа\n";
        cout << "5. Перевірити, чи всі числа додатні\n";
        cout << "6. Відсортувати студентів за прізвищем\n";
        cout << "7. Знайти перше число >100\n";
        cout << "0. Вихід\n";
        cout << "Ваш вибір: ";
        cin >> choice;

        switch (choice) {
            case 1: task1(); break;
            case 2: task2(); break;
            case 3: task3(); break;
            case 4: task4(); break;
            case 5: task5(); break;
            case 6: task6(); break;
            case 7: task7(); break;
            case 0: cout << "Вихід...\n"; break;
            default: cout << "Невірний вибір!\n";
        }

    } while (choice != 0);

    return 0;
}
