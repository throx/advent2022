#include <iostream>
#include <string>
#include <vector>
#include "../shared/Point.h"
#include <sstream>
#include <conio.h>
#include <cassert>

using namespace std;
const vector<Point2> DIRS = { P2::R, P2::D, P2::L, P2::U };
enum DIRS_E { Right, Down, Left, Up };
const char DIRS_C[] = ">v<^";

struct World {
    vector<string> m;
    size_t ymax = 0;
};

char at(const World& world, const Point2& p) {
    return world.m[p[1]][p[0]];
}

void dump(const World& world, const Point2& p, int dir) {
    for (int y = 0; y < world.m.size(); ++y) {
        for (int x = 0; x < world.m[y].length(); ++x) {
            if (Point2({ x,y }) == p) {
                cout << DIRS_C[dir];
            }
            else {
                cout << world.m[y][x];
            }
        }
        cout << endl;
    }
    cout << endl;
}

Point2 move1(const World& world, const Point2& p, int d) {
    Point2 dir = DIRS[d];
    Point2 pt = p + dir;

    while (true) {
        if (dir[0] == 0) {
            if (pt[1] < 0) {
                pt[1] = world.ymax - 1;
            }
            else if (pt[1] >= world.ymax) {
                pt[1] = 0;
            }
        }
        else {
            if (pt[0] < 0) {
                pt[0] = world.m[p[1]].length() - 1;
            }
            else if (pt[0] >= world.m[p[1]].length()) {
                pt[0] = 0;
            }
        }

        if (pt[0] < world.m[pt[1]].length() && at(world, pt) != ' ') {
            break;
        }
        pt += dir;
    }

    return pt;
}

// Cube:
//  BA
//  C
// ED
// F
tuple<Point2, int> move2(const Point2& p, int d) {
    Point2 pt = p + DIRS[d];

    switch (d) {
    case Up:
        if (pt[1] == -1) {
            // Off top of A or B.

            if (pt[0] < 100) {
                // Top of B goes to left of F
                pt[1] = pt[0] + 100;
                pt[0] = 0;
                d = Right;
            }
            else {
                // Top of A goes to bottom of F
                pt[1] = 199;
                pt[0] = pt[0] - 100;
            }
        }
        else if (pt[1] == 99 && pt[0] < 50) {
            // Top of E goes to left of C
            pt[1] = pt[0] + 50;
            pt[0] = 50;
            d = Right;
        }
        break;

    case Down:
        if (pt[1] == 200) {
            // Bottom of F to top of A
            pt[1] = 0;
            pt[0] = pt[0] + 100;
        }
        else if (pt[1] == 150 && pt[0] >= 50) {
            // Bottom of D to right of F
            pt[1] = pt[0] + 100;
            pt[0] = 49;
            d = Left;
        }
        else if (pt[1] == 50 && pt[0] >= 100) {
            // Bottom of A to right of C
            pt[1] = pt[0] - 50;
            pt[0] = 99;
            d = Left;
        }
        break;

    case Left:
        if (pt[0] == -1) {
            // Left of E or F

            if (pt[1] >= 150) {
                // Left of F goes to top of B
                pt[0] = pt[1] - 100;
                pt[1] = 0;
                d = Down;
            }
            else {
                // Left of E goes to left of B
                pt[0] = 50;
                pt[1] = 149 - pt[1];
                d = Right;
            }
        }
        else if (pt[0] == 49 && pt[1] < 100) {
            // Left of B or C

            if (pt[1] >= 50) {
                // Left of C goes to top of E
                pt[0] = pt[1] - 50;
                pt[1] = 100;
                d = Down;
            }
            else {
                // Left of B goes to left of E
                pt[0] = 0;
                pt[1] = 149 - pt[1];
                d = Right;
            }
        }
        break;

    case Right:
        if (pt[0] == 150) {
            // Right of A goes to right of D
            pt[0] = 99;
            pt[1] = 149 - pt[1];
            d = Left;
        }
        else if (pt[0] == 100 && pt[1] >= 50) {
            if (pt[1] < 100) {
                // Right of C goes to bottom of A
                pt[0] = pt[1] + 50;
                pt[1] = 49;
                d = Up;
            }
            else {
                // Right of D goes to right of A
                pt[0] = 149;
                pt[1] = 149 - pt[1];
                d = Left;
            }
        }
        else if (pt[0] == 50 && pt[1] >= 150) {
            // Right of F goes to bottom of D
            pt[0] = pt[1] - 100;
            pt[1] = 149;
            d = Up;
        }
        break;

    default:
        throw new exception("BOOM");
    }

    return { pt, d };
}

int main()
{
    {
        // Unit test.  200 in any direction should return to same point
        Point2 p = { 130, 30 };
        for (int d = 0; d < 4; ++d) {
            cout << "Testing: " << DIRS_C[d] << endl;
            auto pp = p;
            auto dd = d;
            for (int x = 0; x < 200; ++x) {
                auto [newpp, newdd] = move2(pp, dd);
                pp = newpp;
                dd = newdd;
                cout << "At: " << pp << ", dir = " << DIRS_C[dd] << endl;
            }
            assert(p == pp);
            cout << endl;
        }

        p = { 80, 30 };
        for (int d = 0; d < 4; ++d) {
            cout << "Testing: " << DIRS_C[d] << endl;
            auto pp = p;
            auto dd = d;
            for (int x = 0; x < 200; ++x) {
                auto [newpp, newdd] = move2(pp, dd);
                pp = newpp;
                dd = newdd;
                cout << "At: " << pp << ", dir = " << DIRS_C[dd] << endl;
            }
            assert(p == pp);
            cout << endl;
        }
    }


    World world;

    string moves;
    string s;
    while (getline(cin, s)) {
        if (s.empty()) {
            continue;
        }

        if (isdigit(s[0])) {
            moves = s;
        }
        else {
            world.m.push_back(s);
        }
    }
    world.ymax = world.m.size();

    istringstream iss(moves);
    Point2 start({ (__int64)world.m[0].find_first_not_of(' '), 0 });

    Point2 pos = start;
    int dir = 0;

    while (!iss.eof()) {
        int num;
        char turn;

        iss >> num;
        for (int i = 0; i < num; ++i) {
            Point2 newpos = move1(world, pos, dir);
            if (at(world, newpos) == '.') {
                pos = newpos;
            }
            else {
                break;
            }
        }

        if (iss.eof()) {
            break;
        }
        iss >> turn;
        if (turn == 'R') {
            ++dir;
            if (dir == DIRS.size()) {
                dir = 0;
            }
        }
        else {
            --dir;
            if (dir < 0) {
                dir = DIRS.size() - 1;
            }
        }

        //dump(world, pos, dir);
    }

    __int64 part1 = pos[1] * 1000 + pos[0] * 4 + dir + 1004;

    cout << "Part 1: " << part1 << endl;

    iss = istringstream(moves);

    pos = start;
    dir = 0;

    cout << "At: " << pos << ", dir = " << DIRS_C[dir] << endl;

    while (!iss.eof()) {
        int num;
        char turn;

        iss >> num;
        cout << "Moving " << num << " turns" << endl;
        for (int i = 0; i < num; ++i) {
            auto [newpos, newdir] = move2(pos, dir);
            cout << "Trying: " << newpos << ", dir = " << DIRS_C[newdir];
            if (at(world, newpos) == '.') {
                pos = newpos;
                dir = newdir;
                cout << " ... ok" << endl;
            }
            else {
                cout << " !!! blocked" << endl;
                break;
            }
        }

        if (iss.eof()) {
            break;
        }
        iss >> turn;
        cout << "Turning " << turn << endl;
        if (turn == 'R') {
            ++dir;
            if (dir == DIRS.size()) {
                dir = 0;
            }
        }
        else {
            --dir;
            if (dir < 0) {
                dir = DIRS.size() - 1;
            }
        }

        cout << "At: " << pos << ", dir = " << DIRS_C[dir] << endl;
        //dump(world, pos, dir);
    }

    __int64 part2 = pos[1] * 1000 + pos[0] * 4 + dir + 1004;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}