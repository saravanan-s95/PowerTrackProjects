#ifndef WORD_ENTRY_H
#define WORD_ENTRY_H

#include <fstream>
#include <string>
#include <unordered_map>

using namespace std;

class WordEntry
{
private:
    string word;
    unordered_map<string, int> occurrences;

public:
    WordEntry() = default;
    explicit WordEntry(const string &word) : word(word) {}

    void updateOccurrence(const string &filename);
    void setOccurrence(const string &filename, int frequency);
    const string &getWord() const;
    void display() const;
    void save(ofstream &fout) const;
};

#endif
