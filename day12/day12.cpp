#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <map>
#include <regex>
#include <conio.h>
#include "../shared/Point.h"

using namespace std;

const regex INPUT_R("(\\d+) (\\d+)");

int at(vector<string>& world, Point2 p)
{
    return world[p[1]][p[0]] - 'a';
}

int main()
{
    vector<string> world;
    Point2 start;
    Point2 end;

    string s;
    while (getline(cin, s)) {
        size_t pos = s.find('S');
        if (pos != string::npos) {
            s[pos] = 'a';
            start[0] = pos;
            start[1] = world.size();
        }
        pos = s.find('E');
        if (pos != string::npos) {
            s[pos] = 'z';
            end[0] = pos;
            end[1] = world.size();
        }

        world.push_back(s);
    }

    map<Point2, vector<Point2>> todo;
    map<Point2, vector<Point2>> done;

    todo[start] = vector<Point2>();

    while (!todo.empty()) {
        Point2 curr = todo.begin()->first;
        vector<Point2> curr_hist = todo.begin()->second;
        todo.erase(todo.begin());

        auto it = done.find(curr);
        if (it == done.end() || it->second.size() > curr_hist.size()) {
            done[curr] = curr_hist;
            curr_hist.push_back(curr);

            curr.DoSquareNeighbours([&](auto& p) {
                if (p[0] < 0 || p[1] < 0 || p[0] >= world[0].length() || p[1] >= world.size()) {
                    return;
                }

                // Is valid?
                if (at(world, p) - at(world, curr) > 1) {
                    return;
                }

                auto it = todo.find(p);
                if (it == todo.end() || it->second.size() > curr_hist.size()) {
                    todo[p] = curr_hist;
                }
            });
        }
    }

    auto path = done[end];

    cout << "Part 1: " << path.size() << endl;

    todo.clear();
    done.clear();
    todo[end] = vector<Point2>();

    while (!todo.empty()) {
        Point2 curr = todo.begin()->first;
        vector<Point2> curr_hist = todo.begin()->second;
        todo.erase(todo.begin());

        auto it = done.find(curr);
        if (it == done.end() || it->second.size() > curr_hist.size()) {
            done[curr] = curr_hist;
            curr_hist.push_back(curr);

            curr.DoSquareNeighbours([&](auto& p) {
                if (p[0] < 0 || p[1] < 0 || p[0] >= world[0].length() || p[1] >= world.size()) {
                    return;
                }

                // Is valid?
                if (at(world, curr) - at(world, p) > 1) {
                    return;
                }

                auto it = todo.find(p);
                if (it == todo.end() || it->second.size() > curr_hist.size()) {
                    todo[p] = curr_hist;
                }
                });
        }
    }

    size_t minpath = 99999;
    for (int y = 0; y < world.size(); ++y) {
        for (int x = 0; x < world[0].length(); ++x) {
            if (world[y][x] == 'a') {
                Point2 p({ x,y });
                if (done.find(p) != done.end()) {
                    minpath = min(done[p].size(), minpath);
                }
            }
        }
    }

    cout << "Part 2: " << minpath << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}