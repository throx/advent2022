#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <numeric>
#include <conio.h>
#include "../shared/Point.h"

using namespace std;

struct Wall {
    Point2 pos;
    Point2 dir;
};

// I've resorted to globals.  Ugh.
int xmax = 0;
int ymax = 0;
int repeat = 0;
Point2 start;
Point2 goal;
vector<Wall> walls;
map<int, set<Point2>> wallcache;

// May have to cache this
bool has_wall(const Point2& pos, int step) {

    set<Point2>& now_walls = wallcache[step];
    if (now_walls.empty()) {
        for (auto& w : walls) {
            Point2 p = w.pos + w.dir * step;
            p[0] = (p[0] % xmax + xmax) % xmax;
            p[1] = (p[1] % ymax + ymax) % ymax;
            now_walls.insert(p);
        }
    }

    return now_walls.find(pos) != now_walls.end();
}

void part1()
{
    // Dijkstra, I guess
    struct Node {
        Point2 pos;
        int step;
        vector<Point2> path;

        bool operator<(const Node& other) const {
            if (pos < other.pos) return true;
            if (pos > other.pos) return false;
            return step < other.step;
        }

        bool operator==(const Node& other) const {
            return (pos == other.pos) && (step == other.step);
        }
    };

    multimap<int, Node> todo;
    map<Node, int> done;

    Node startnode = { start, 0 };

    todo.insert({ 0, startnode });
    int part1 = 0;
    vector<Point2> path;

    while (!todo.empty()) {
        int turn = todo.begin()->first;
        Node node = todo.begin()->second;
        todo.erase(todo.begin());

        // Check if better
        auto done_it = done.find(node);
        if (done_it != done.end()) {
            if (done_it->second <= turn) {
                continue;
            }
        }
        done[node] = turn;
        if (node.pos == goal) {
            part1 = turn;
            path = node.path;
            path.push_back(goal);
            break;
        }

        // Find next options
        ++turn;
        int step = turn % repeat;
        node.path.push_back(node.pos);

        node.pos.DoSquareNeighbours([&](const Point2& n) {
            if (n[0] < 0 || n[0] >= xmax) return;
            if (n[1] < 0) return;
            if (n == goal) {
                todo.insert({ turn, {goal, step, node.path } });
                return;
            }
            if (n[1] >= ymax && n != goal) return;
            if (has_wall(n, step)) return;
            todo.insert({ turn, {n, step, node.path } });
            });
        if (!has_wall(node.pos, step)) {
            todo.insert({ turn, {node.pos, step, node.path } });
        }
    }

    // RESULTS...

    cout << "Part 1: " << part1 << endl;

    //for (int t = 0; t < path.size(); ++t) {
    //    int step = t % repeat;
    //    cout << "Turn " << t << endl;
    //    for (int y = 0; y < ymax; ++y) {
    //        for (int x = 0; x < xmax; ++x) {
    //            Point2 p = { x, y };
    //            if (has_wall(p, step)) {
    //            cout << '#';
    //            }
    //            else if (p == path[t]) {
    //                cout << 'E';
    //            }
    //            else {
    //                cout << '.';
    //            }
    //        }
    //        cout << endl;
    //    }
    //    cout << endl;
    //}
    //cout << endl;


}

void part2()
{
    struct Node {
        Point2 pos;
        int step;
        int phase;      // 0 = to goal, 1 = back, 2 = to goal

        bool operator<(const Node& other) const {
            if (pos < other.pos) return true;
            if (pos > other.pos) return false;
            if (step < other.step) return true;
            if (step > other.step) return false;
            return phase < other.phase;
        }

        bool operator==(const Node& other) const {
            return (pos == other.pos) && (step == other.step) && (phase == other.phase);
        }
    };

    multimap<int, Node> todo;
    map<Node, int> done;

    Node startnode = { start, 0, 0 };

    todo.insert({ 0, startnode });
    int part2 = 0;
    vector<Point2> path;

    while (!todo.empty()) {
        int turn = todo.begin()->first;
        Node node = todo.begin()->second;
        todo.erase(todo.begin());

        // Check if better
        auto done_it = done.find(node);
        if (done_it != done.end()) {
            if (done_it->second <= turn) {
                continue;
            }
        }
        done[node] = turn;
        if (node.pos == goal && node.phase == 2) {
            part2 = turn;
            path.push_back(goal);
            break;
        }

        // Find next options
        ++turn;
        int step = turn % repeat;
        int phase = node.phase;
        if (node.pos == goal && node.phase == 0) {
            phase = 1;
        }
        if (node.pos == start && node.phase == 1) {
            phase = 2;
        }

        node.pos.DoSquareNeighbours([&](const Point2& n) {
            if (n[0] < 0 || n[0] >= xmax) return;
            if (n == start && phase == 1) {
                todo.insert({ turn, {start, step, phase} });
                return;
            }
            if (n[1] < 0) return;
            if (n == goal && (phase == 0 || phase == 2)) {
                todo.insert({ turn, {goal, step, phase } });
                return;
            }
            if (n[1] >= ymax && n != goal) return;
            if (has_wall(n, step)) return;
            todo.insert({ turn, {n, step, phase } });
            });
        if (!has_wall(node.pos, step)) {
            todo.insert({ turn, {node.pos, step, phase } });
        }
    }

    // RESULTS...

    cout << "Part 2: " << part2 << endl;
}

int main()
{
    int y = -1;

    while (!cin.eof()) {
        string s;
        cin >> s;
        xmax = s.length() - 2;

        for (int x = 0; x < s.length(); ++x) {
            auto it = P2::DIRS.find(s[x]);
            if (it != P2::DIRS.end()) {
                Wall w = { {x-1,y}, it->second };
                walls.push_back(w);
            }
        }
        ++y;
    }

    ymax = y - 1;

    start = { 0, -1 };
    goal = { xmax-1, ymax };

    repeat = lcm(xmax, ymax);

    part1();
    part2();
    cout << "Part 2: " << 0 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}