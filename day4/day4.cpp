#include <iostream>
#include <string>
#include <regex>
#include <conio.h>

using namespace std;

const regex INPUT_R("(\\d+)-(\\d+),(\\d+)-(\\d+)");

int main()
{
    int count = 0;
    int count2 = 0;

    string s;
    while (getline(cin, s)) {
        smatch sm;
        if (regex_match(s.cbegin(), s.cend(), sm, INPUT_R)) {
            int start_1 = stoi(sm[1]);
            int end_1 = stoi(sm[2]);
            int start_2 = stoi(sm[3]);
            int end_2 = stoi(sm[4]);

            if ((start_1 <= start_2 && end_1 >= end_2) ||
                (start_2 <= start_1 && end_2 >= end_1)) {

                ++count;
            }

            if (start_2 <= end_1 && start_1 <= end_2) {
                ++count2;
            }
        }
    }


    // RESULTS...

    cout << "Part 1: " << count << endl;
    cout << "Part 2: " << count2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}