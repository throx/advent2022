#include <iostream>
#include <string>
#include <regex>
#include <list>
#include <vector>
#include <conio.h>

using namespace std;

const regex INPUT_R("move (\\d+) from (\\d) to (\\d)");

int main()
{
    vector<list<char>> stacks;
    stacks.resize(9);

    string s;
    while (getline(cin, s)) {
        if (s.empty()) {
            break;
        }

        for (int pos = 0; pos < 9; ++pos) {
            int p = pos * 4 + 1;
            if (s.length() > p) {
                if (isalpha(s[p])) {
                    stacks[pos].push_back(s[p]);
                }
            }
        }
    }

    auto stacks2 = stacks;

    while (getline(cin, s)) {
        smatch sm;
        if (regex_match(s.cbegin(), s.cend(), sm, INPUT_R)) {
            int num = stoi(sm[1]);
            int from = stoi(sm[2]) - 1;
            int to = stoi(sm[3]) - 1;

            for (int i = 0; i < num; ++i) {
                stacks[to].push_front((stacks[from].front()));
                stacks[from].pop_front();
            }

            list<char> crane;

            for (int i = 0; i < num; ++i) {
                crane.push_back(stacks2[from].front());
                stacks2[from].pop_front();
            }

            while (!crane.empty()) {
                stacks2[to].push_front(crane.back());
                crane.pop_back();
            }
        }
    }

    string part1;
    for (auto& stack : stacks) {
        if (!stack.empty()) {
            part1.push_back(stack.front());
        }
    }

    string part2;
    for (auto& stack : stacks2) {
        if (!stack.empty()) {
            part2.push_back(stack.front());
        }
    }

    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}