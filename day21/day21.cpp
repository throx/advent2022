#include <iostream>
#include <string>
#include <regex>
#include <set>
#include <map>
#include <conio.h>
#include <cassert>

using namespace std;

const regex INPUT_EXP("(....): (....) (.) (....)");
const regex INPUT_VAL("(....): (\\d+)");
const __int64 UNK = numeric_limits<__int64>::min();

struct Node {
    string name;
    __int64 value = UNK;
    char op = 0;

    string left;
    string right;
};

typedef map<string, Node> NodeMap;

int main()
{
    NodeMap nodes;

    string s;
    while (getline(cin, s)) {
        smatch sm;
        if (regex_match(s.cbegin(), s.cend(), sm, INPUT_VAL)) {
            nodes[sm[1]] = Node({ sm[1], stoi(sm[2]) });
        }
        else if (regex_match(s.cbegin(), s.cend(), sm, INPUT_EXP)) {
            nodes[sm[1]] = Node({ sm[1], UNK, sm[3].str()[0], sm[2], sm[4]});
        }
    }

    auto og_nodes = nodes;

    while (nodes["root"].value == UNK) {
        for (auto& nn : nodes) {
            Node& node = nn.second;
            if (node.value == UNK) {
                __int64 l = nodes[node.left].value;
                __int64 r = nodes[node.right].value;
                if (l != UNK && r != UNK) {                    
                    switch (node.op) {
                    case '+':
                        node.value = l + r;
                        break;
                    case '-':
                        node.value = l - r;
                        break;
                    case '*':
                        node.value = l * r;
                        break;
                    case '/':
                        node.value = l / r;
                        break;
                    default:
                        throw new exception("boom");
                    }
                    assert(node.value > 0);
                }
            }
        }
    }

    cout << "Part 1: " << nodes["root"].value << endl;


    nodes = og_nodes;
    nodes["root"].op = '=';
    nodes["root"].value = UNK + 1;
    nodes["humn"].value = UNK;
    nodes["humn"].left = "humn";
    nodes["humn"].right = "humn";

    while (nodes["humn"].value == UNK) {
        for (auto& nn : nodes) {
            Node& node = nn.second;
            Node& nodel = nodes[node.left];
            Node& noder = nodes[node.right];
            __int64 l = nodel.value;
            __int64 r = noder.value;
            __int64 v = node.value;

            if (v == UNK) {
                if (l != UNK && r != UNK) {
                    switch (node.op) {
                    case '+':
                        node.value = l + r;
                        break;
                    case '-':
                        node.value = l - r;
                        break;
                    case '*':
                        node.value = l * r;
                        break;
                    case '/':
                        node.value = l / r;
                        break;
                    default:
                        throw new exception("boom");
                    }
                    assert(node.value > 0);
                }
            }
            else {
                if (l == UNK && r != UNK) {
                    switch (node.op) {
                    case '+':
                        nodel.value = v - r;
                        break;
                    case '-':
                        nodel.value = v + r;
                        break;
                    case '*':
                        assert(v % r == 0);
                        nodel.value = v / r;
                        break;
                    case '/':
                        nodel.value = v * r;
                        break;
                    case '=':
                        nodel.value = r;
                        break;
                    default:
                        throw new exception("boom");
                    }
                    assert(nodel.value > 0);
                }
                else if (l != UNK && r == UNK) {
                    switch (node.op) {
                    case '+':
                        noder.value = v - l;
                        break;
                    case '-':
                        noder.value = l - v;
                        break;
                    case '*':
                        assert(v % l == 0);
                        noder.value = v / l;
                        break;
                    case '/':
                        assert(v % l == 0);
                        noder.value = l / v;
                        break;
                    case '=':
                        noder.value = l;
                        break;
                    default:
                        throw new exception("boom");
                    }
                    assert(noder.value > 0);
                }
            }
        }
    }

    cout << "Part 2: " << nodes["humn"].value << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}