#include <iostream>
#include <fstream>
#include <string>
#include <cctype> // for isspace()
#include <algorithm>

using namespace std;

int main() {
  string filename;
  ifstream file;
  char choice;

    do {
        LOOP:
        cout << endl << "Filename: ";
        cin >> filename;
        cout << "\n";

        file.open(filename + ".cpp");

        if (file.fail()) {
            cout << "File Failed To Open." << endl;
            goto LOOP;
        }

        string line;
        int lines = 0;

        while (getline(file, line)) {
            // Check for comment start (single-line comments)
            size_t commentStart = line.find("//");

            if (commentStart != string::npos) {
                // Remove everything from the comment start onwards
                line.erase(commentStart);
            }

            // Check if the line is empty (only whitespace characters) after comment removal
            bool isEmpty = all_of(line.begin(), line.end(), ::isspace);

            if (!isEmpty) {
                lines++;
                // Optionally, uncomment this line to print non-empty lines:
                cout << line << endl;
            }
        }

        file.close();
        cout << "\n";
        cout << "Total Lines with Content: " << lines << endl;
        cout << "Do You Want To Count Lines Of Another File?: ";
        cin >> choice;
    } while (choice == 'y' || choice == 'Y');

    return 0;
}