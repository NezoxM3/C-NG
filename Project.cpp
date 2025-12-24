#include <iostream>
#include <filesystem>
#include <fstream>
#include <regex>

namespace fs = std::filesystem;
using namespace std;

class FileSystemObject {
protected:
    fs::path path;

public:
    FileSystemObject(const fs::path& p) : path(p) {}
    virtual ~FileSystemObject() = default;

    virtual uintmax_t getSize() const = 0;
    fs::path getPath() const { return path; }
};

class File : public FileSystemObject {
public:
    File(const fs::path& p) : FileSystemObject(p) {}

    uintmax_t getSize() const override {
        if (fs::exists(path) && fs::is_regular_file(path))
            return fs::file_size(path);
        return 0;
    }
};

class Directory : public FileSystemObject {
public:
    Directory(const fs::path& p) : FileSystemObject(p) {}

    uintmax_t getSize() const override {
        uintmax_t size = 0;
        if (!fs::exists(path)) return 0;

        for (const auto& entry : fs::recursive_directory_iterator(path)) {
            if (fs::is_regular_file(entry))
                size += fs::file_size(entry);
        }
        return size;
    }
};

class FileManager {
public:
    void showContent(const fs::path& p) {
        if (!fs::exists(p)) {
            cout << "Path does not exist\n";
            return;
        }

        for (const auto& entry : fs::directory_iterator(p)) {
            cout << entry.path().filename();
            if (fs::is_directory(entry))
                cout << " [DIR]";
            cout << endl;
        }
    }

    void createFile(const fs::path& p) {
        ofstream file(p);
        file.close();
    }

    void createDirectory(const fs::path& p) {
        fs::create_directory(p);
    }

    void remove(const fs::path& p) {
        fs::remove_all(p);
    }

    void rename(const fs::path& oldName, const fs::path& newName) {
        fs::rename(oldName, newName);
    }

    void copy(const fs::path& from, const fs::path& to) {
        fs::copy(from, to, fs::copy_options::recursive);
    }

    void move(const fs::path& from, const fs::path& to) {
        fs::rename(from, to);
    }

    uintmax_t getSize(const fs::path& p) {
        if (fs::is_regular_file(p))
            return File(p).getSize();
        if (fs::is_directory(p))
            return Directory(p).getSize();
        return 0;
    }

    void searchByMask(const fs::path& root, const string& mask) {
        if (!fs::exists(root)) return;

        regex pattern(mask);

        for (const auto& entry : fs::recursive_directory_iterator(root)) {
            if (regex_match(entry.path().filename().string(), pattern)) {
                cout << entry.path() << endl;
            }
        }
    }
};

int main() {
    FileManager fm;
    fs::path current = fs::current_path();

    cout << "Current directory:\n";
    fm.showContent(current);

    cout << "\nCreating directory TestDir...\n";
    fm.createDirectory("TestDir");

    cout << "Creating file TestDir/file.txt...\n";
    fm.createFile("TestDir/file.txt");

    cout << "\nSize of TestDir: "
         << fm.getSize("TestDir") << " bytes\n";

    cout << "\nSearching for .txt files:\n";
    fm.searchByMask(current, ".*\\.txt");

    cout << "\nCopying TestDir to TestDirCopy...\n";
    fm.copy("TestDir", "TestDirCopy");

    cout << "Moving TestDirCopy to MovedDir...\n";
    fm.move("TestDirCopy", "MovedDir");

    cout << "\nRemoving directories...\n";
    fm.remove("TestDir");
    fm.remove("MovedDir");

    cout << "\nDone.\n";
    return 0;
}