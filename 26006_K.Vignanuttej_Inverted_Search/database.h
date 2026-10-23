#ifndef DATABASE_H
#define DATABASE_H

#include "wordEntry.h"
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class Database
{
private:
    unordered_map<string, WordEntry> wordIndex;
    void readFile(const string &filename);

public:
    void create(const vector<string> &filenames);
    void updateDatabase(const string &filename);
    void search(const string &word) const;
    void display() const;
    void save(const string &filename) const;
    void load(const string &filename);
};

#endif
