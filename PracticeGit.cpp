#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <iomanip>

struct DateTime {
    int year, month, day, hour, minute;
    DateTime(): year(0), month(0), day(0), hour(0), minute(0) {}
};

struct Task {
    std::string title;
    int priority;
    std::string description;
    DateTime dt;
};

const std::string FILENAME = "tasks.txt";
const std::string SEP = "---";

std::tm to_tm(const DateTime &d) {
    std::tm tm = {};
    tm.tm_year = d.year - 1900;
    tm.tm_mon = d.month - 1;
    tm.tm_mday = d.day;
    tm.tm_hour = d.hour;
    tm.tm_min = d.minute;
    tm.tm_sec = 0;
    tm.tm_isdst = -1;
    return tm;
}

time_t to_time_t(const DateTime &d) {
    std::tm tm = to_tm(d);
    return std::mktime(&tm);
}

std::string dt_to_string(const DateTime &d) {
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(4) << d.year << "-"
        << std::setw(2) << d.month << "-"
        << std::setw(2) << d.day << " "
        << std::setw(2) << d.hour << ":"
        << std::setw(2) << d.minute;
    return oss.str();
}

bool readDateTimeFromUser(DateTime &out) {
    std::cout << "Enter date and time (Year Month Day Hour Minute): ";
    if (!(std::cin >> out.year >> out.month >> out.day >> out.hour >> out.minute)) {
        std::cin.clear();
        std::string tmp; std::getline(std::cin, tmp);
        return false;
    }
    if (out.year < 1900 || out.month < 1 || out.month > 12 || out.day < 1 || out.day > 31
        || out.hour < 0 || out.hour > 23 || out.minute < 0 || out.minute > 59) {
        std::cout << "Invalid date/time values.\n";
        return false;
    }
    std::string rest;
    std::getline(std::cin, rest); // clear rest of line
    return true;
}

void saveToFile(const std::vector<Task> &tasks) {
    std::ofstream ofs(FILENAME);
    if (!ofs) {
        std::cout << "Failed to open file for writing.\n";
        return;
    }
    for (const auto &t : tasks) {
        ofs << t.title << "\n";
        ofs << t.priority << "\n";
        ofs << t.description << "\n";
        ofs << t.dt.year << " " << t.dt.month << " " << t.dt.day << " "
            << t.dt.hour << " " << t.dt.minute << "\n";
        ofs << SEP << "\n";
    }
    ofs.close();
}

std::vector<Task> loadFromFile() {
    std::vector<Task> tasks;
    std::ifstream ifs(FILENAME);
    if (!ifs) {
        return tasks;
    }
    std::string line;
    while (true) {
        Task t;
        if (!std::getline(ifs, t.title)) break;
        if (t.title.size() == 0) {
            if (!std::getline(ifs, t.title)) break;
        }
        if (!std::getline(ifs, line)) break; // priority
        {
            std::istringstream prios(line);
            prios >> t.priority;
        }
        if (!std::getline(ifs, t.description)) break; // description
        if (!std::getline(ifs, line)) break; // datetime line
        {
            std::istringstream dtss(line);
            dtss >> t.dt.year >> t.dt.month >> t.dt.day >> t.dt.hour >> t.dt.minute;
        }
        if (!std::getline(ifs, line)) {
        }
        tasks.push_back(t);
    }
    ifs.close();
    return tasks;
}

void printTask(const Task &t, int index = -1) {
    if (index >= 0) std::cout << "[" << index << "] ";
    std::cout << "Title: " << t.title << "\n";
    std::cout << "Priority: " << t.priority << "\n";
    std::cout << "Description: " << t.description << "\n";
    std::cout << "Date/Time: " << dt_to_string(t.dt) << "\n";
    std::cout << "-----------------------------------\n";
}

void addTask(std::vector<Task> &tasks) {
    Task t;
    std::string tmp;
    std::cout << "Enter title: ";
    std::getline(std::cin, t.title);
    if (t.title.empty()) {
        std::cout << "Title cannot be empty.\n";
        return;
    }
    std::cout << "Enter priority (integer, higher means more important): ";
    if (!(std::cin >> t.priority)) {
        std::cin.clear();
        std::getline(std::cin, tmp);
        std::cout << "Invalid priority.\n";
        return;
    }
    std::getline(std::cin, tmp);
    std::cout << "Enter description: ";
    std::getline(std::cin, t.description);
    if (!readDateTimeFromUser(t.dt)) {
        std::cout << "Failed to read date/time. Task addition cancelled.\n";
        return;
    }
    tasks.push_back(t);
    saveToFile(tasks);
    std::cout << "Task added.\n";
}

void listAllTasks(const std::vector<Task> &tasks) {
    if (tasks.empty()) {
        std::cout << "Task list is empty.\n";
        return;
    }
    for (size_t i = 0; i < tasks.size(); ++i) {
        printTask(tasks[i], static_cast<int>(i));
    }
}

void deleteTask(std::vector<Task> &tasks) {
    if (tasks.empty()) {
        std::cout << "No tasks to delete.\n";
        return;
    }
    listAllTasks(tasks);
    std::cout << "Enter task index to delete: ";
    int idx;
    if (!(std::cin >> idx)) {
        std::cin.clear();
        std::string tmp; std::getline(std::cin, tmp);
        std::cout << "Invalid index.\n";
        return;
    }
    std::string rest; std::getline(std::cin, rest);
    if (idx < 0 || idx >= static_cast<int>(tasks.size())) {
        std::cout << "Index out of range.\n";
        return;
    }
    tasks.erase(tasks.begin() + idx);
    saveToFile(tasks);
    std::cout << "Task deleted.\n";
}

void editTask(std::vector<Task> &tasks) {
    if (tasks.empty()) {
        std::cout << "No tasks to edit.\n";
        return;
    }
    listAllTasks(tasks);
    std::cout << "Enter task index to edit: ";
    int idx;
    if (!(std::cin >> idx)) {
        std::cin.clear();
        std::string tmp; std::getline(std::cin, tmp);
        std::cout << "Invalid index.\n";
        return;
    }
    std::string rest; std::getline(std::cin, rest);
    if (idx < 0 || idx >= static_cast<int>(tasks.size())) {
        std::cout << "Index out of range.\n";
        return;
    }
    Task &t = tasks[idx];
    std::cout << "Editing task [" << idx << "]. Press Enter to skip a field.\n";

    std::string input;
    std::cout << "New title (previous: \"" << t.title << "\"): ";
    std::getline(std::cin, input);
    if (!input.empty()) t.title = input;

    std::cout << "New priority (previous: " << t.priority << ") or Enter: ";
    std::getline(std::cin, input);
    if (!input.empty()) {
        std::istringstream iss(input);
        int p;
        if (iss >> p) t.priority = p;
        else std::cout << "Invalid priority — keeping previous.\n";
    }

    std::cout << "New description (previous: \"" << t.description << "\"): ";
    std::getline(std::cin, input);
    if (!input.empty()) t.description = input;

    std::cout << "Edit date/time? (y/n): ";
    std::getline(std::cin, input);
    if (!input.empty() && (input == "y" || input == "Y")) {
        DateTime nd;
        if (readDateTimeFromUser(nd)) {
            t.dt = nd;
        } else {
            std::cout << "Failed to read date/time — keeping previous.\n";
        }
    }
    saveToFile(tasks);
    std::cout << "Task updated.\n";
}

void searchByTitle(const std::vector<Task> &tasks) {
    std::cout << "Enter substring to search in title: ";
    std::string q; std::getline(std::cin, q);
    bool found = false;
    for (size_t i = 0; i < tasks.size(); ++i) {
        if (tasks[i].title.find(q) != std::string::npos) {
            printTask(tasks[i], static_cast<int>(i));
            found = true;
        }
    }
    if (!found) std::cout << "No matches found.\n";
}

void searchByPriority(const std::vector<Task> &tasks) {
    std::cout << "Enter priority to search: ";
    int p;
    if (!(std::cin >> p)) {
        std::cin.clear();
        std::string tmp; std::getline(std::cin, tmp);
        std::cout << "Invalid priority.\n";
        return;
    }
    std::string rest; std::getline(std::cin, rest);
    bool found = false;
    for (size_t i = 0; i < tasks.size(); ++i) {
        if (tasks[i].priority == p) {
            printTask(tasks[i], static_cast<int>(i));
            found = true;
        }
    }
    if (!found) std::cout << "No matches found.\n";
}

void searchByDescription(const std::vector<Task> &tasks) {
    std::cout << "Enter substring to search in description: ";
    std::string q; std::getline(std::cin, q);
    bool found = false;
    for (size_t i = 0; i < tasks.size(); ++i) {
        if (tasks[i].description.find(q) != std::string::npos) {
            printTask(tasks[i], static_cast<int>(i));
            found = true;
        }
    }
    if (!found) std::cout << "No matches found.\n";
}

void searchByDateTime(const std::vector<Task> &tasks) {
    std::cout << "Enter exact date/time to search (Year Month Day Hour Minute):\n";
    DateTime q;
    if (!readDateTimeFromUser(q)) {
        std::cout << "Invalid input.\n";
        return;
    }
    bool found = false;
    for (size_t i = 0; i < tasks.size(); ++i) {
        if (tasks[i].dt.year == q.year && tasks[i].dt.month == q.month &&
            tasks[i].dt.day == q.day && tasks[i].dt.hour == q.hour &&
            tasks[i].dt.minute == q.minute) {
            printTask(tasks[i], static_cast<int>(i));
            found = true;
        }
    }
    if (!found) std::cout << "No matches found.\n";
}

void searchTaskMenu(const std::vector<Task> &tasks) {
    while (true) {
        std::cout << "\nSearch tasks:\n";
        std::cout << "1. By title\n2. By priority\n3. By description\n4. By date/time\n0. Back\n";
        std::cout << "Choice: ";
        int ch;
        if (!(std::cin >> ch)) {
            std::cin.clear();
            std::string tmp; std::getline(std::cin, tmp);
            std::cout << "Invalid input.\n";
            continue;
        }
        std::string rest; std::getline(std::cin, rest);
        if (ch == 0) break;
        switch (ch) {
            case 1: searchByTitle(tasks); break;
            case 2: searchByPriority(tasks); break;
            case 3: searchByDescription(tasks); break;
            case 4: searchByDateTime(tasks); break;
            default: std::cout << "Unknown choice.\n"; break;
        }
    }
}

void showTasksInRange(const std::vector<Task> &tasks, time_t start, time_t end) {
    bool found = false;
    for (size_t i = 0; i < tasks.size(); ++i) {
        time_t t = to_time_t(tasks[i].dt);
        if (t >= start && t <= end) {
            printTask(tasks[i], static_cast<int>(i));
            found = true;
        }
    }
    if (!found) std::cout << "No tasks found in this range.\n";
}

void showTasksByDay(const std::vector<Task> &tasks) {
    std::cout << "Enter day (Year Month Day): ";
    DateTime q;
    if (!(std::cin >> q.year >> q.month >> q.day)) {
        std::cin.clear();
        std::string tmp; std::getline(std::cin, tmp);
        std::cout << "Invalid input.\n";
        return;
    }
    std::string rest; std::getline(std::cin, rest);
    DateTime start = q; start.hour = 0; start.minute = 0;
    DateTime end = q; end.hour = 23; end.minute = 59;
    time_t ts = to_time_t(start);
    time_t te = to_time_t(end);
    showTasksInRange(tasks, ts, te);
}

void showTasksByWeek(const std::vector<Task> &tasks) {
    std::cout << "Enter start day of the week (Year Month Day): ";
    DateTime q;
    if (!(std::cin >> q.year >> q.month >> q.day)) {
        std::cin.clear();
        std::string tmp; std::getline(std::cin, tmp);
        std::cout << "Invalid input.\n";
        return;
    }
    std::string rest; std::getline(std::cin, rest);
    DateTime start = q; start.hour = 0; start.minute = 0;
    DateTime end = q;
    std::tm tm_end = to_tm(end);
    tm_end.tm_mday += 6;
    std::mktime(&tm_end);
    DateTime endNorm;
    endNorm.year = tm_end.tm_year + 1900;
    endNorm.month = tm_end.tm_mon + 1;
    endNorm.day = tm_end.tm_mday;
    endNorm.hour = 23;
    endNorm.minute = 59;
    time_t ts = to_time_t(start);
    time_t te = to_time_t(endNorm);
    showTasksInRange(tasks, ts, te);
}

void showTasksByMonth(const std::vector<Task> &tasks) {
    std::cout << "Enter month (Year Month): ";
    int year, month;
    if (!(std::cin >> year >> month)) {
        std::cin.clear();
        std::string tmp; std::getline(std::cin, tmp);
        std::cout << "Invalid input.\n";
        return;
    }
    std::string rest; std::getline(std::cin, rest);
    DateTime start; start.year = year; start.month = month; start.day = 1; start.hour = 0; start.minute = 0;
    DateTime next = start;
    next.month += 1;
    if (next.month > 12) { next.month = 1; next.year += 1; }
    DateTime nextStart = next; nextStart.hour = 0; nextStart.minute = 0;
    std::tm tm_next = to_tm(nextStart);
    time_t tn = std::mktime(&tm_next);
    tn -= 60;
    time_t ts = to_time_t(start);
    time_t te = tn;
    showTasksInRange(tasks, ts, te);
}

void sortByPriority(std::vector<Task> &tasks) {
    std::sort(tasks.begin(), tasks.end(), [](const Task &a, const Task &b) {
        return a.priority > b.priority;
    });
    saveToFile(tasks);
    std::cout << "Sorted by priority (higher first).\n";
}

void sortByDateTime(std::vector<Task> &tasks) {
    std::sort(tasks.begin(), tasks.end(), [](const Task &a, const Task &b) {
        return to_time_t(a.dt) < to_time_t(b.dt);
    });
    saveToFile(tasks);
    std::cout << "Sorted by execution date and time (ascending).\n";
}

void showMenu() {
    std::cout << "\n===== Task List =====\n";
    std::cout << "1. Add task\n";
    std::cout << "2. Delete task\n";
    std::cout << "3. Edit task\n";
    std::cout << "4. Search tasks\n";
    std::cout << "5. Show tasks for a day\n";
    std::cout << "6. Show tasks for a week\n";
    std::cout << "7. Show tasks for a month\n";
    std::cout << "8. Sort by priority\n";
    std::cout << "9. Sort by date\n";
    std::cout << "10. Show all tasks\n";
    std::cout << "0. Exit\n";
    std::cout << "Choice: ";
}

int main() {
    std::vector<Task> tasks = loadFromFile();
    while (true) {
        showMenu();
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::string tmp; std::getline(std::cin, tmp);
            std::cout << "Invalid input. Try again.\n";
            continue;
        }
        std::string rest; std::getline(std::cin, rest);
        switch (choice) {
            case 1: addTask(tasks); break;
            case 2: deleteTask(tasks); break;
            case 3: editTask(tasks); break;
            case 4: searchTaskMenu(tasks); break;
            case 5: showTasksByDay(tasks); break;
            case 6: showTasksByWeek(tasks); break;
            case 7: showTasksByMonth(tasks); break;
            case 8: sortByPriority(tasks); break;
            case 9: sortByDateTime(tasks); break;
            case 10: listAllTasks(tasks); break;
            case 0:
                std::cout << "Saving and exiting...\n";
                saveToFile(tasks);
                return 0;
            default:
                std::cout << "Invalid choice.\n";
        }
    }
    return 0;
}
