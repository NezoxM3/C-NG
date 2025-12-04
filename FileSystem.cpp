#include <ncurses.h>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

namespace fs = std::filesystem;

struct Entry {
    fs::directory_entry de;
    std::string name;
    uintmax_t size;
    std::string type;
};

static const std::vector<std::string> TEXT_EXT = {".txt", ".cpp", ".h", ".hpp", ".log", ".md", ".ini", ".conf"};

bool is_text_file(const fs::path &p) {
    auto ext = p.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return std::find(TEXT_EXT.begin(), TEXT_EXT.end(), ext) != TEXT_EXT.end();
}

std::vector<Entry> read_dir(const fs::path &p) {
    std::vector<Entry> out;
    try {
        for (auto &d : fs::directory_iterator(p)) {
            Entry e;
            e.de = d;
            e.name = d.path().filename().string();
            std::error_code ec;
            if (d.is_regular_file(ec)) {
                e.type = "file";
                e.size = d.file_size(ec);
            } else if (d.is_directory(ec)) {
                e.type = "dir";
                e.size = 0;
            } else {
                e.type = "other";
                e.size = 0;
            }
            out.push_back(std::move(e));
        }
        std::sort(out.begin(), out.end(), [](auto &a, auto &b) {
            if (a.type != b.type) return a.type > b.type;
            if (a.type == "dir" && b.type != "dir") return true;
            if (a.type != "dir" && b.type == "dir") return false;
            return a.name < b.name;
        });
    } catch (...) {
    }
    return out;
}

void draw_list(WINDOW *win, const std::vector<Entry> &entries, int highlight, const fs::path &cwd, int start_idx) {
    werase(win);
    int h, w;
    getmaxyx(win, h, w);
    mvwprintw(win, 0, 0, "Current: %s", cwd.string().c_str());
    mvwhline(win, 1, 0, 0, w);
    int line = 2;
    int max_lines = h - 3;
    for (int i = start_idx; i < (int)entries.size() && (line - 2) < max_lines; ++i, ++line) {
        auto &e = entries[i];
        if (i == highlight) {
            wattron(win, A_REVERSE);
        }
        std::string type = (e.type == "dir" ? "<DIR>" : (e.type=="file"?"<FILE>":"<OTHR>"));
        std::ostringstream ss;
        if (e.type == "file") {
            ss << std::setw(10) << e.size;
        } else {
            ss << std::setw(10) << "-";
        }
        std::string size_s = ss.str();
        int name_space = w - 30;
        std::string name_display = e.name;
        if ((int)name_display.size() > name_space && name_space > 4) {
            name_display = name_display.substr(0, name_space - 3) + "...";
        }
        mvwprintw(win, line, 1, "%-3d %-*s %10s %6s", i+1, name_space, name_display.c_str(), size_s.c_str(), type.c_str());
        if (i == highlight) {
            wattroff(win, A_REVERSE);
        }
    }
    mvwprintw(win, h-1, 0, "Up/Down: move  Enter: open  Backspace: up  v:view  d:delete  n:new dir  q:quit");
    wrefresh(win);
}

std::string prompt_input(WINDOW *win, const std::string &prompt) {
    echo();
    nocbreak();
    curs_set(1);
    int h, w;
    getmaxyx(win, h, w);
    mvwprintw(win, h-2, 0, "%s", std::string(w, ' ').c_str());
    mvwprintw(win, h-2, 0, "%s", prompt.c_str());
    char buf[1024];
    wgetnstr(win, buf, 1000);
    noecho();
    cbreak();
    curs_set(0);
    return std::string(buf);
}

bool confirm_prompt(WINDOW *win, const std::string &msg) {
    int h, w;
    getmaxyx(win, h, w);
    mvwprintw(win, h-2, 0, "%s (y/n): ", msg.c_str());
    wrefresh(win);
    int ch;
    while ((ch = wgetch(win))) {
        if (ch == 'y' || ch == 'Y') return true;
        if (ch == 'n' || ch == 'N') return false;
    }
    return false;
}

void view_text_file(WINDOW *win, const fs::path &p) {
    std::ifstream ifs(p);
    if (!ifs.is_open()) {
        int h, w;
        getmaxyx(win, h, w);
        mvwprintw(win, h-2, 0, "Cannot open file for reading. Press any key.");
        wgetch(win);
        return;
    }
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(ifs, line)) lines.push_back(line);
    int top = 0;
    int h, w;
    getmaxyx(win, h, w);
    int content_h = h - 4;
    keypad(win, TRUE);
    while (1) {
        werase(win);
        mvwprintw(win, 0, 0, "Viewing: %s", p.filename().string().c_str());
        mvwhline(win, 1, 0, 0, w);
        for (int i = 0; i < content_h; ++i) {
            int idx = top + i;
            if (idx >= (int)lines.size()) break;
            std::string s = lines[idx];
            if ((int)s.size() > w - 1) s = s.substr(0, w - 4) + "...";
            mvwprintw(win, 2 + i, 0, "%s", s.c_str());
        }
        mvwprintw(win, h-1, 0, "Up/Down: scroll  PgUp/PgDn: jump  Backspace/q: close");
        wrefresh(win);
        int ch = wgetch(win);
        if (ch == KEY_UP) { if (top > 0) --top; }
        else if (ch == KEY_DOWN) { if (top + content_h < (int)lines.size()) ++top; }
        else if (ch == KEY_NPAGE) { top = std::min((int)lines.size()-content_h, top + content_h); if (top<0) top=0; }
        else if (ch == KEY_PPAGE) { top = std::max(0, top - content_h); }
        else if (ch == 127 || ch == KEY_BACKSPACE || ch == 'q') { break; }
    }
}

int main() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);

    fs::path cwd = fs::current_path();
    std::vector<Entry> entries = read_dir(cwd);
    int highlight = 0;
    int start_idx = 0;

    int h, w;
    getmaxyx(stdscr, h, w);

    WINDOW *win = newwin(h, w, 0, 0);
    keypad(win, TRUE);

    draw_list(win, entries, highlight, cwd, start_idx);

    while (1) {
        int ch = wgetch(win);
        if (ch == KEY_UP) {
            if (highlight > 0) {
                --highlight;
                if (highlight < start_idx) start_idx = highlight;
            }
        } else if (ch == KEY_DOWN) {
            if (highlight + 1 < (int)entries.size()) {
                ++highlight;
                int hlines = h - 3;
                if (highlight - start_idx >= hlines) start_idx = highlight - hlines + 1;
            }
        } else if (ch == '\n' || ch == KEY_ENTER) {
            if (entries.empty()) { /* nothing */ }
            else {
                auto &sel = entries[highlight];
                if (sel.type == "dir") {
                    try {
                        cwd /= sel.name;
                        entries = read_dir(cwd);
                        highlight = 0;
                        start_idx = 0;
                    } catch (...) {
                    }
                } else {
                    if (is_text_file(sel.de.path())) {
                        view_text_file(win, sel.de.path());
                    } else {
                        mvwprintw(win, h-2, 0, "Not a supported text file. Press any key.");
                        wgetch(win);
                    }
                }
            }
        } else if (ch == KEY_BACKSPACE || ch == 127) {
            if (cwd.has_parent_path()) {
                cwd = cwd.parent_path();
                entries = read_dir(cwd);
                highlight = 0;
                start_idx = 0;
            }
        } else if (ch == 'q' || ch == 'Q') {
            break;
        } else if (ch == 'v' || ch == 'V') {
            if (!entries.empty()) {
                auto &sel = entries[highlight];
                if (sel.type == "file" && is_text_file(sel.de.path())) {
                    view_text_file(win, sel.de.path());
                } else {
                    mvwprintw(win, h-2, 0, "Selected is not a supported text file. Press any key.");
                    wgetch(win);
                }
            }
        } else if (ch == 'n' || ch == 'N') {
            std::string name = prompt_input(win, "New directory name: ");
            if (!name.empty()) {
                fs::path newp = cwd / name;
                std::error_code ec;
                if (fs::create_directory(newp, ec)) {
                    entries = read_dir(cwd);
                    highlight = 0;
                    start_idx = 0;
                } else {
                    mvwprintw(win, h-2, 0, "Cannot create directory: %s. Press any key.", ec.message().c_str());
                    wgetch(win);
                }
            }
        } else if (ch == 'd' || ch == 'D') {
            if (!entries.empty()) {
                auto &sel = entries[highlight];
                std::string msg = "Delete " + sel.name + " ?";
                if (confirm_prompt(win, msg)) {
                    std::error_code ec;
                    fs::remove_all(sel.de.path(), ec);
                    if (ec) {
                        mvwprintw(win, h-2, 0, "Delete failed: %s. Press any key.", ec.message().c_str());
                        wgetch(win);
                    }
                    entries = read_dir(cwd);
                    highlight = std::min(highlight, (int)entries.size()-1);
                    if (highlight < 0) highlight = 0;
                    start_idx = 0;
                }
            }
        }
        getmaxyx(stdscr, h, w);
        wresize(win, h, w);
        draw_list(win, entries, highlight, cwd, start_idx);
    }

    delwin(win);
    endwin();
    return 0;
}
