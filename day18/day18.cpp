#include <iostream>
#include <string>
#include <set>
#include "../shared/Point.h"
#include "../shared/Rect.h"
#include <conio.h>

using namespace std;


int main()
{
    set<Point3> cubes;

    while (!cin.eof()) {
        Point3 c;
        cin >> c;

        cubes.insert(c);
    }

    int part1 = 0;
    for (auto& c : cubes) {
        c.DoSquareNeighbours([&](auto& n) {
            if (cubes.find(n) == cubes.end()) {
                ++part1;
            }
        });
    }

    cout << "Part 1: " << part1 << endl;

    Rect3 cube;

    for (auto& c : cubes) {
        cube = cube.Bound(c);
    }

    set<Point3> todo;
    set<Point3> not_internal = cubes;

    todo.insert(Point3());

    while (!todo.empty()) {
        Point3 c = *todo.begin();
        todo.erase(todo.begin());
        not_internal.insert(c);
        c.DoSquareNeighbours([&](auto& n) {
            if (cube.Contains(n)) {
                if (not_internal.find(n) == not_internal.end()) {
                    todo.insert(n);
                }
            }
        });
    }

    // Count internal edges
    int part2 = part1;
    for (auto& c : not_internal) {
        c.DoSquareNeighbours([&](auto& n) {
            if (cube.Contains(n)) {
                if (not_internal.find(n) == not_internal.end()) {
                    --part2;
                }
            }
        });
    }

    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}