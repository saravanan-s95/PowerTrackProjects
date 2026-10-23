#include "database.h"
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace std;

int main()
{
    Database database;
    int choice = 0;

    do
    {
        cout << "\n========== INVERTED SEARCH =========="
                  << "\n1. Create database from text files"
                  << "\n2. Update database with a text file"
                  << "\n3. Search for a word"
                  << "\n4. Display complete database"
                  << "\n5. Save database"
                  << "\n6. Load database"
                  << "\n7. Exit"
                  << "\nEnter your choice: ";

        if (!(cin >> choice))
        {
            cout << "Invalid input. Exiting.\n";
            break;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice)
        {
        case 1:
        {
            int count;
            cout << "How many input files? ";
            if (!(cin >> count) || count <= 0)
            {
                cout << "Enter a positive number of files.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            vector<string> filenames;
            filenames.reserve(static_cast<size_t>(count));
            for (int i = 0; i < count; ++i)
            {
                string filename;
                cout << "Enter filename " << i + 1 << ": ";
                getline(cin, filename);
                filenames.push_back(filename);
            }
            database.create(filenames);
            cout << "Database creation completed.\n";
            break;
        }
        case 2:
        {
            string filename;
            cout << "Enter filename to add: ";
            getline(cin, filename);
            
            database.updateDatabase(filename);
            break;
        }
        case 3:
        {
            string word;
            cout << "Enter word to search (case-sensitive): ";
            getline(cin, word);
            database.search(word);
            break;
        }
        case 4:
            database.display();
            break;
        case 5:
        {
            string filename;
            cout << "Enter database output filename: ";
            getline(cin, filename);
            database.save(filename);
            break;
        }
        case 6:
        {
            string filename;
            cout << "Enter saved database filename: ";
            getline(cin, filename);
            database.load(filename);
            break;
        }
        case 7:
            cout << "Exiting Inverted Search.\n";
            break;
        default:
            cout << "Invalid choice. Please choose 1-7.\n";
        }
    } while (choice != 7);

    return 0;
}
