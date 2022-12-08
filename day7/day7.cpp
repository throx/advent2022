#include <iostream>
#include <string>
#include <regex>
#include <map>
#include <list>
#include <numeric>
#include <conio.h>

using namespace std;

const regex CD_R("\\$ cd (.+)");
const regex LS_R("\\$ ls");
const regex DIR_R("dir (.+)");
const regex FILE_R("(\\d+) (.+)");

struct Dir {
    string name;
    map<string, int> files;
    list<Dir> dirs;
    Dir* parent;

    int get_size() {
        int filesize = accumulate(files.begin(), files.end(), 0, [](auto& x, auto& y) {return x + y.second; });
        int dirsize = accumulate(dirs.begin(), dirs.end(), 0, [](auto& x, auto& y) {return x + y.get_size(); });
        return dirsize + filesize;
    }

    Dir* cd(const string& d) {
        if (d == "..") {
            return parent;
        }
        auto it = find_if(dirs.begin(), dirs.end(), [&](auto& x) { return x.name == d; });
        return &(*it);
    }

    int sum_below(int limit) {
        int s = 0;
        int me = get_size();
        if (me <= limit) {
            s = me;
        }
        return accumulate(dirs.begin(), dirs.end(), s, [=](auto& x, auto& y) { return x + y.sum_below(limit); });
    }

    int min_above(int limit) {
        int me = get_size();
        int best = numeric_limits<int>::max();
        if (me >= limit) {
            best = me;
        }

        for (auto& d : dirs) {
            int sub = d.min_above(limit);
            if (sub < best) {
                best = sub;
            }
        }
        
        return best;
    }
};

int main()
{
    Dir root;
    Dir* cwd = &root;

    string s;
    while (getline(cin, s)) {
        smatch sm;
        if (regex_match(s.cbegin(), s.cend(), sm, CD_R)) {
            if (sm[1] == "/") {
                cwd = &root;
            }
            else {
                cwd = cwd->cd(sm[1]);
            }
        }
        else if (regex_match(s.cbegin(), s.cend(), sm, LS_R)) {
            // Do nothing
        }
        else if (regex_match(s.cbegin(), s.cend(), sm, DIR_R)) {
            Dir sub;
            sub.parent = cwd;
            sub.name = sm[1];
            cwd->dirs.push_back(sub);
        }
        else if (regex_match(s.cbegin(), s.cend(), sm, FILE_R)) {
            cwd->files[sm[2]] = stoi(sm[1]);
        }
    }


    // RESULTS...

    cout << "Part 1: " << root.sum_below(100000) << endl;

    int needed = root.get_size() - 40000000;

    cout << "Part 2: " << root.min_above(needed) << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}