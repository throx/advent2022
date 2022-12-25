#include <iostream>
#include <string>
#include <conio.h>

using namespace std;

__int64 from_snafu(const string& s)
{
    __int64 r = 0;
    for (int x = 0; x < s.length(); ++x) {
        r *= 5;
        switch (s[x]) {
        case '2':
            r += 2;
            break;
        case '1':
            r += 1;
            break;
        case '0':
            break;
        case '-':
            r -= 1;
            break;
        case '=':
            r -= 2;
            break;
        default:
            throw new exception("BOOM");
        }
    }
    return r;
}

string to_snafu(__int64 v)
{
    string s;
    while (v != 0) {
        switch ((v % 5 + 5) % 5) {
        case 2:
            s = "2"s + s;
            v -= 2;
            break;
        case 1:
            s = "1"s + s;
            v -= 1;
            break;
        case 0:
            s = "0"s + s;
            break;
        case 4:
            s = "-"s + s;
            v += 1;
            break;
        case 3:
            s = "="s + s;
            v += 2;
            break;
        }

        v /= 5;
    }

    return s;
}

int main()
{
    __int64 f = 0;
    while (!cin.eof()) {
        string s;
        cin >> s;

        f += from_snafu(s);
    }

    cout << f << endl;

    cout << "Part 1: " << to_snafu(f) << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}