#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <conio.h>
#include "../shared/Point.h"
#include "../shared/Rect.h"
#include <cassert>

using namespace std;

const Point2 NOONE = { -1000000,-1000000 };

bool elf(const set<Point2>& elves, const Point2& p) {
    return elves.find(p) != elves.end();
}

vector<vector<Point2>> LOOKDIRS = {
    {P2::N, P2::NE, P2::NW},
    {P2::S, P2::SE, P2::SW},
    {P2::W, P2::NW, P2::SW},
    {P2::E, P2::NE, P2::SE}
};

int main()
{
    set<Point2> elves;

    int y = 0;
    while (!cin.eof()) {
        string s;
        cin >> s;

        for (int x = 0; x < s.length(); ++x) {
            if (s[x] == '#') {
                elves.insert(Point2({ x,y }));
            }
        }
        ++y;
    }

    int part1 = 0;
    int part2 = 0;

    for (int round = 0;; ++round) {
        cout << endl << "Round " << round + 1 << endl;

        int lookdir = round % 4;

        map<Point2, Point2> proposed;   // Proposed -> who is winning
        set<Point2> nomove;

        for (auto p : elves) {
            //cout << "Elf at " << p << ": ";

            int count = 0;
            p.DoNeighbours([&](auto n) {if (elf(elves, n)) ++count; });
            bool moved = false;
            if (count > 0) {

                for (int l = 0; l < 4; ++l) {
                    int testdir = (l + lookdir) % 4;
                    bool bad = false;
                    for (Point2 d : LOOKDIRS[testdir]) {
                        if (elf(elves, p + d)) {
                            bad = true;
                        }
                    }

                    if (!bad) {
                        moved = true;
                        auto newpos = p + LOOKDIRS[testdir][0];
                        //cout << "Propose " << newpos << ", ";

                        auto it = proposed.find(newpos);
                        if (it == proposed.end()) {
                            proposed[newpos] = p;
                            //cout << "ok" << endl;
                        }
                        else {
                            nomove.insert(p);
                            if (it->second != NOONE) {
                                nomove.insert(it->second);
                                it->second = NOONE;
                            }
                            //cout << "blocked" << endl;
                        }
                        break;
                    }
                }
            }

            if (!moved) {
                //cout << "no move" << endl;
                nomove.insert(p);
                auto it = proposed.find(p);
                if (it != proposed.end()) {
                    if (it->second != NOONE) {
                        nomove.insert(it->second);
                        it->second = NOONE;
                    }
                }
            }
        }

        bool moved = false;
        for (auto pp : proposed) {
            if (pp.second != NOONE) {
                nomove.insert(pp.first);
                moved = true;
            }
        }

        assert(nomove.size() == elves.size());
        elves.swap(nomove);

        if (round == 9) {
            Rect2 r = Rect2::BoundOf(elves.begin(), elves.end());
            part1 = r.Size() - elves.size();
        }

        if (!moved) {
            part2 = round + 1;
            break;
        }
    }


    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}