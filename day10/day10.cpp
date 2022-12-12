#include <iostream>
#include <string>
#include <sstream>
#include <regex>
#include <conio.h>

#include "Cpu.h"

using namespace std;

const regex INPUT_R("(\\d+) (\\d+)");

int main()
{
    vector<string> opcodes;

    string s;
    while (getline(cin, s)) {
        opcodes.push_back(s);
    }

    Cpu cpu(opcodes);

    __int64 part1 = 0;
    for (int c = 20; c <= 220; c += 40) {
        cpu.RunTo(c);
        part1 += c * cpu.x();
    }

    cpu.Reset();

    stringstream part2;
    for (int c = 1; c <= 240; ++c) {
        cpu.RunTo(c);
        int pos = (c - 1) % 40;

        if (abs(cpu.x() - pos) <= 1) {
            part2 << "#";
        }
        else {
            part2 << " ";
        }

        if (pos == 39) {
            part2 << endl;
        }
    }
    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << endl << part2.str() << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}