#include <iostream>
#include <string>
#include <regex>
#include <list>
#include <conio.h>
#include "../shared/Split.h"

using namespace std;

const regex MONKEY_R("\\s*Monkey \\d+:");
const regex ITEMS_R("\\s*Starting items: (.*)");
const regex OP_R("\\s*Operation: new = old (.) (.*)");
const regex DIV_R("\\s*Test: divisible by (\\d+)");
const regex TRUE_R("\\s*If true: throw to monkey (\\d+)");
const regex FALSE_R("\\s*If false: throw to monkey (\\d+)");

struct Monkey {
    list<int> items;
    string op;
    string operand;
    int div_test;
    int true_throw;
    int false_throw;
    int inspect_count = 0;
};

int main()
{
    vector<Monkey> monkeys;
    __int64 mod = 1;

    string s;
    while (getline(cin, s)) {
        if (s.empty()) {
            continue;
        }

        smatch sm;
        if (regex_match(s.cbegin(), s.cend(), sm, MONKEY_R)) {
            monkeys.push_back(Monkey());
        }
        else if (regex_match(s.cbegin(), s.cend(), sm, ITEMS_R)) {
            auto v = SplitInt(sm[1]);
            monkeys.back().items.assign(v.begin(), v.end());
        }
        else if (regex_match(s.cbegin(), s.cend(), sm, OP_R)) {
            monkeys.back().op = sm[1];
            monkeys.back().operand = sm[2];
        }
        else if (regex_match(s.cbegin(), s.cend(), sm, DIV_R)) {
            monkeys.back().div_test = stoi(sm[1]);
            mod *= monkeys.back().div_test;
        }
        else if (regex_match(s.cbegin(), s.cend(), sm, TRUE_R)) {
            monkeys.back().true_throw = stoi(sm[1]);
        }
        else if (regex_match(s.cbegin(), s.cend(), sm, FALSE_R)) {
            monkeys.back().false_throw = stoi(sm[1]);
        }
    }

    auto og_monkeys = monkeys;

    for (int round = 0; round < 20; ++round) {
        for (auto& monkey : monkeys) {
            while (!monkey.items.empty()) {
                int item = monkey.items.front();
                monkey.items.pop_front();

                // Change worry
                int val = item;
                if (monkey.operand != "old") {
                    val = stoi(monkey.operand);
                }
                if (monkey.op == "+") {
                    item = item + val;
                }
                else {
                    item = item * val;
                }

                item = item / 3;

                if (item % monkey.div_test == 0) {
                    monkeys[monkey.true_throw].items.push_back(item);
                }
                else {
                    monkeys[monkey.false_throw].items.push_back(item);
                }

                ++monkey.inspect_count;
            }
        }
    }

    // RESULTS...

    sort(monkeys.begin(), monkeys.end(), [](auto& m1, auto& m2) { return m1.inspect_count > m2.inspect_count; });

    cout << "Part 1: " << monkeys[0].inspect_count * monkeys[1].inspect_count << endl;

    monkeys = og_monkeys;

    for (int round = 0; round < 10000; ++round) {
        for (auto& monkey : monkeys) {
            while (!monkey.items.empty()) {
                __int64 item = monkey.items.front();
                monkey.items.pop_front();

                // Change worry
                __int64 val = item;
                if (monkey.operand != "old") {
                    val = stoi(monkey.operand);
                }
                if (monkey.op == "+") {
                    item = item + val;
                }
                else {
                    item = item * val;
                }

                item = item % mod;

                if (item % monkey.div_test == 0) {
                    monkeys[monkey.true_throw].items.push_back(item);
                }
                else {
                    monkeys[monkey.false_throw].items.push_back(item);
                }

                ++monkey.inspect_count;
            }
        }
    }

    sort(monkeys.begin(), monkeys.end(), [](auto& m1, auto& m2) { return m1.inspect_count > m2.inspect_count; });

    __int64 part2 = monkeys[0].inspect_count;
    part2 *= monkeys[1].inspect_count;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}