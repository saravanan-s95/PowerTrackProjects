#include "wordEntry.h"
#include <iostream>

using namespace std;

void WordEntry::updateOccurrence(const string &filename)
{
    auto it = occurrences.find(filename);
    if (it != occurrences.end())
        ++it->second;
    else
        occurrences[filename] = 1;
}

void WordEntry::setOccurrence(const string &filename, int frequency)
{
    occurrences[filename] = frequency;
}

const string &WordEntry::getWord() const
{
    return word;
}

void WordEntry::display() const
{
    cout << word << '\n';
    for (const auto &entry : occurrences)
        cout << "  " << entry.first << " : " << entry.second << '\n';
}

void WordEntry::save(ofstream &fout) const
{
    fout << word;
    for (const auto &entry : occurrences)
        fout << '|' << entry.first << ':' << entry.second;
    fout << '\n';
}
