#include <iostream>
#include <string>
#include <set>
#include <conio.h>

using namespace std;

size_t get_first_unique(const string& s, size_t x)
{
    for (size_t p = x; p < s.length(); ++p) {
        set<char> test(s.begin() + (p-x), s.begin() + p);
        if (test.size() == x) {
            return p;
        }
    }

    return -1;
}

int main()
{
    while (!cin.eof()) {
        string s;
        cin >> s;

        cout << s.substr(0, 10) << endl;
        cout << " Part 1: " << get_first_unique(s, 4) << endl;
        cout << " Part 2: " << get_first_unique(s, 14) << endl;
        cout << endl;
    }

    cout << "Press a key to continue" << endl;
    _getch();
}