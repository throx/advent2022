#include <iostream>
#include <string>
#include <regex>
#include <set>
#include <conio.h>
#include "../shared/Point.h"

using namespace std;

const regex INPUT_R("(.) (\\d+)");

Point2 move_tail(const Point2& head, const Point2& tail)
{
    Point2 diff = head - tail;
    if (diff[0] >= -1 && diff[0] <= 1 && diff[1] >= -1 && diff[1] <= 1) {
        return tail;
    }
    diff = diff.Clamp(1);
    return tail + diff;
}

int main()
{
    set<Point2> visited[10];
    Point2 pos[10];

    string s;
    while (getline(cin, s)) {
        smatch sm;
        if (regex_match(s.cbegin(), s.cend(), sm, INPUT_R)) {
            Point2 dir = P2::DIRS[((string)sm[1])[0]];
            int num = stoi(sm[2]);

            for (int i = 0; i < num; ++i) {
                pos[0] += dir;
                for (int t = 0; t < 9; ++t) {
                    pos[t + 1] = move_tail(pos[t], pos[t + 1]);
                }
                for (int t = 0; t < 10; ++t) {
                    visited[t].insert(pos[t]);
                }
            }
        }
    }


    // RESULTS...

    cout << "Part 1: " << visited[1].size() << endl;
    cout << "Part 2: " << visited[9].size() << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}