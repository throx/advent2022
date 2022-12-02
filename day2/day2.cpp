#include <iostream>
#include <string>
#include <regex>

using namespace std;

int main()
{
    string s;
    int round = 1;
    int total_1 = 0;
    int total_2 = 0;

    while (getline(cin, s)) {
        int them = s[0] - 'A';
        int us = s[2] - 'X';

        // Part 1
        int win = 0;
        if ((them + 1) % 3 == us) {
            win = 1;
        }
        else if (them != us) {
            win = -1;
        }

        int score_1 = (us + 1) + (win + 1) * 3;
        total_1 += score_1;

        // Part 2
        win = s[2] - 'Y';
        if (win == 0) {
            us = them;
        }
        else if (win == 1) {
            us = (them + 1) % 3;
        }
        else {
            us = (them + 2) % 3;
        }

        int score_2 = (us + 1) + (win + 1) * 3;
        total_2 += score_2;

        cout << "Round: " << round << " Score: " << score_2 << " Total: " << total_2 << endl;
    }


    // RESULTS...

    cout << "Part 1: " << total_1 << endl;
    cout << "Part 2: " << total_2 << endl;
}