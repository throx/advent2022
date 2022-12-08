#include <iostream>
#include <string>
#include <vector>
#include <set>
#include "../shared/Point.h"
#include "../shared/Rect.h"
#include <conio.h>

using namespace std;

set<Point2> count_visible(vector<string>& trees, Point2 where, Point2 dir) {
    int h = -1;
    set<Point2> visible;
    int maxx = trees[0].length();
    int maxy = trees.size();
    Rect2 bounds(Point2(), Point2({maxx, maxy}));

    while (bounds.Contains(where)) {
        int curr = trees[where[1]][where[0]] - '0';
        if (curr > h) {
            visible.insert(where);
            h = curr;
        }
        where += dir;
    }

    return visible;
}

int visible_from(vector<string>& trees, Point2 where, Point2 dir) {
    int h = trees[where[1]][where[0]] - '0';
    int s = 0;
    where += dir;
    int maxx = trees[0].length();
    int maxy = trees.size();
    Rect2 bounds(Point2(), Point2({ maxx, maxy }));

    while (bounds.Contains(where)) {
        ++s;
        int curr = trees[where[1]][where[0]] - '0';
        if (curr >= h) {
            return s;
        }
        where += dir;
    }

    return s;
}

int score(vector<string>& trees, Point2 where) {
    int s = visible_from(trees, where, P2::N);
    s *= visible_from(trees, where, P2::S);
    s *= visible_from(trees, where, P2::E);
    s *= visible_from(trees, where, P2::W);

    cout << where << ": " << s << endl;
    return s;
}

int main()
{
    vector<string> trees;

    while (!cin.eof()) {
        string s;
        cin >> s;
        trees.push_back(s);
    }

    set<Point2> part1;
    int maxx = trees[0].length();
    int maxy = trees.size();
    set<Point2> ret;

    for (int x = 0; x < maxx; ++x) {
        ret = count_visible(trees, Point2({ x, 0 }), P2::S);
        part1.insert(ret.begin(), ret.end());
        ret = count_visible(trees, Point2({ x, maxy-1 }), P2::N);
        part1.insert(ret.begin(), ret.end());
    }

    for (int y = 0; y < maxy; ++y) {
        ret = count_visible(trees, Point2({ 0, y }), P2::E);
        part1.insert(ret.begin(), ret.end());
        ret = count_visible(trees, Point2({ maxx - 1, y }), P2::W);
        part1.insert(ret.begin(), ret.end());
    }

    cout << "Part 1: " << part1.size() << endl;

    int part2 = 0;
    for (int x = 0; x < maxx; ++x) {
        for (int y = 0; y < maxy; ++y) {
            int s = score(trees, Point2({ x, y }));
            part2 = max(s, part2);
        }
    }
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}