#include <iostream>
#include <fstream>
#include <unordered_map>
#include <map>
#include <string>
#include <random>

using namespace std;

unordered_map<string, string> dictAtoB;
unordered_map<string, string> dictBtoA;
const string filename = "dictionary.txt";

void loadDictionary() {
    ifstream file(filename);
    if (!file.is_open()) return;

    string a, b;
    while (file >> a >> b) {
        dictAtoB[a] = b;
        dictBtoA[b] = a;
    }
    file.close();
}

void saveDictionary() {
    ofstream file(filename);
    for (auto &p : dictAtoB)
        file << p.first << " " << p.second << "\n";
    file.close();
}

void addWordPair() {
    string a, b;
    cout << "Введіть слово мовою A: ";
    cin >> a;
    cout << "Введіть переклад мовою B: ";
    cin >> b;

    dictAtoB[a] = b;
    dictBtoA[b] = a;

    saveDictionary();
    cout << "Пару додано!\n";
}

void searchTranslation() {
    string w;
    cout << "Введіть слово для перекладу: ";
    cin >> w;

    if (dictAtoB.count(w)) {
        cout << "Переклад (A→B): " << dictAtoB[w] << "\n";
    } else if (dictBtoA.count(w)) {
        cout << "Переклад (B→A): " << dictBtoA[w] << "\n";
    } else {
        cout << "Слово не знайдено!\n";
    }
}

void deleteWordPair() {
    string w;
    cout << "Введіть слово для видалення: ";
    cin >> w;

    if (dictAtoB.count(w)) {
        string b = dictAtoB[w];
        dictAtoB.erase(w);
        dictBtoA.erase(b);
        saveDictionary();
        cout << "Видалено!\n";
    }
    else if (dictBtoA.count(w)) {
        string a = dictBtoA[w];
        dictBtoA.erase(w);
        dictAtoB.erase(a);
        saveDictionary();
        cout << "Видалено!\n";
    }
    else {
        cout << "Слова немає!\n";
    }
}

void dictionaryMenu() {
    int choice;
    do {
        cout << "\n--- МЕНЮ ПЕРЕКЛАДАЧА ---\n";
        cout << "1. Додати пару слів\n";
        cout << "2. Пошук перекладу\n";
        cout << "3. Видалити пару слів\n";
        cout << "0. Повернутися назад\n";
        cout << "Ваш вибір: ";
        cin >> choice;

        switch (choice) {
            case 1: addWordPair(); break;
            case 2: searchTranslation(); break;
            case 3: deleteWordPair(); break;
            case 0: break;
            default: cout << "Невірний вибір!\n";
        }
    } while (choice != 0);
}

void randomStatistics() {
    map<int, int> freq;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, 100);

    for (int i = 0; i < 1000; i++) {
        int x = dist(gen);
        freq[x]++;
    }

    cout << "\nЧисла, що зустрічаються лише 1 раз:\n";
    for (auto &p : freq)
        if (p.second == 1)
            cout << p.first << " ";
    cout << "\n";

    cout << "\nЧастота появи всіх чисел:\n";
    for (auto &p : freq)
        cout << p.first << " → " << p.second << " раз(и)\n";
}

int main() {
    loadDictionary();

    int choice;
    do {
        cout << "\n====== ГОЛОВНЕ МЕНЮ ======\n";
        cout << "1. Перекладач (словник)\n";
        cout << "2. Аналіз випадкових чисел\n";
        cout << "0. Вихід\n";
        cout << "Ваш вибір: ";
        cin >> choice;

        switch (choice) {
            case 1: dictionaryMenu(); break;
            case 2: randomStatistics(); break;
            case 0: cout << "Вихід...\n"; break;
            default: cout << "Невірний вибір!\n";
        }

    } while (choice != 0);

    return 0;
}
