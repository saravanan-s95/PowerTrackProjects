#include "database.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

void Database::create(const vector<string> &filenames)
{
    wordIndex.clear();
    for (const auto &filename : filenames)
        readFile(filename);
}

void Database::readFile(const string &filename)
{
    ifstream fin(filename);
    if (!fin)
    {
        cerr << "Error: could not open input file '" << filename << "'.\n";
        return;
    }

    string word;
    while (fin >> word)
    {
        auto it = wordIndex.find(word);
        if (it != wordIndex.end())
            it->second.updateOccurrence(filename);
        else
        {
            WordEntry entry(word);
            entry.updateOccurrence(filename);
            wordIndex.emplace(word, entry);
        }
    }
}

void Database::updateDatabase(const string &filename)
{
    readFile(filename);
}

void Database::search(const string &word) const
{
    auto it = wordIndex.find(word);
    if (it != wordIndex.end())
        it->second.display();
    else
        cout << "Word '" << word << "' not found.\n";
}

void Database::display() const
{
    if (wordIndex.empty())
    {
        cout << "Database is empty. Create or load an index first.\n";
        return;
    }
    for (const auto &entry : wordIndex)
        entry.second.display();
}

void Database::save(const string &filename) const
{
    ofstream fout(filename);
    if (!fout)
    {
        cerr << "Error: could not open output file '" << filename << "'.\n";
        return;
    }
    for (const auto &entry : wordIndex)
        entry.second.save(fout);
    cout << "Database saved to '" << filename << "'.\n";
}

void Database::load(const string &filename)
{
    ifstream fin(filename);
    if (!fin)
    {
        cerr << "Error: could not open database file '" << filename << "'.\n";
        return;
    }

    wordIndex.clear();
    string line;
    while (getline(fin, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);
        string word;
        if (!getline(ss, word, '|') || word.empty())
            continue;

        WordEntry entry(word);
        string token;
        while (getline(ss, token, '|'))
        {
            stringstream part(token);
            string filenamePart, frequency;
            if (getline(part, filenamePart, ':') &&
                getline(part, frequency) && !filenamePart.empty())
            {
                try
                {
                    entry.setOccurrence(filenamePart, stoi(frequency));
                }
                catch (const exception &)
                {
                    cerr << "Warning: skipped invalid frequency in database file.\n";
                }
            }
        }
        wordIndex[word] = entry;
    }
    cout << "Database loaded from '" << filename << "'.\n";
}
