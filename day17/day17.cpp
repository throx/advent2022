#include <iostream>
#include <string>
#include <regex>
#include "../shared/Point.h"
#include <conio.h>

using namespace std;

typedef set<Point2> PSet;
typedef vector<PSet> Shapes;

void sim(PSet& world, const string& moves, int& movepos, const PSet& shape)
{
    // find min y
    int miny = min_element(world.begin(), world.end(), [](auto& p1, auto& p2) { return p1[1] < p2[1]; })->at(1);
    auto it = world.begin();

    Point2 shape_pos({ 2, miny - 4 });

    while (true) {

        Point2 move = moves[movepos] == '<' ? P2::L : P2::R;
        ++movepos;
        if (movepos == moves.length()) {
            movepos = 0;
        }

        bool move_ok = true;
        for (auto& shape_point : shape) {
            Point2 pos = shape_pos + shape_point + move;
            if (pos[0] < 0 || pos[0] >= 7 || world.find(pos) != world.end()) {
                move_ok = false;
                break;
            }
        }

        if (move_ok) {
            shape_pos += move;
        }

        move = P2::D;
        move_ok = true;
        for (auto& shape_point : shape) {
            Point2 pos = shape_pos + shape_point + move;
            if (world.find(pos) != world.end()) {
                move_ok = false;
                break;
            }
        }

        if (!move_ok) {
            for (auto& shape_point : shape) {
                Point2 pos = shape_pos + shape_point;
                world.insert(pos);
            }
            break;
        }
        else {
            shape_pos += move;
        }
    }
}

int part1(const Shapes& shapes, const string& moves)
{
    PSet world;
    for (int x = 0; x < 7; ++x) {
        world.insert({ x,0 });
    }

    int movepos = 0;
    for (int rock = 0; rock < 2022; ++rock) {
        const PSet& shape = shapes.at(rock % shapes.size());
        sim(world, moves, movepos, shape);
    }

    int miny = min_element(world.begin(), world.end(), [](auto& p1, auto& p2) { return p1[1] < p2[1]; })->at(1);
    return -miny;
}

__int64 part2(const Shapes& shapes, const string& moves)
{
    __int64 TOTAL = 1000000000000i64;

    // Need to run until we run out of moves and shapes at the same time.
    // Then do it again, because first time wasn't on flat ground.

    PSet world;
    for (int x = 0; x < 7; ++x) {
        world.insert({ x,0 });
    }

    int movepos = 0;
    int rock = 0;
    __int64 turn = 0;

    // Find a repeat that is consistent
    map<pair<int, int>, int> history;
    __int64 last_diff = 0;
    __int64 last_turn = 0;
    __int64 height;
    __int64 old_height;

    while (true) {

        last_turn = turn;
        while (history.find(make_pair(rock, movepos)) == history.end()) {
            int height = -min_element(world.begin(), world.end(), [](auto& p1, auto& p2) { return p1[1] < p2[1]; })->at(1);
            history[make_pair(rock, movepos)] = height;

            const PSet& shape = shapes.at(rock);
            sim(world, moves, movepos, shape);
            ++rock;
            if (rock == shapes.size()) {
                rock = 0;
            }
            ++turn;
        }

        height = -min_element(world.begin(), world.end(), [](auto& p1, auto& p2) { return p1[1] < p2[1]; })->at(1);
        old_height = history[make_pair(rock, movepos)];

        if (height - old_height == last_diff) {
            break;
        }
        last_diff = height - old_height;
        history.clear();
    }

    __int64 cyc_len = turn - last_turn;
    __int64 cyc_height = height - old_height;
    __int64 cycs = (TOTAL - turn) / cyc_len;
    __int64 cyc_end = turn + cycs * cyc_len;
    __int64 num_left = TOTAL - cyc_end;

    while (num_left > 0) {
        const PSet& shape = shapes.at(rock);
        sim(world, moves, movepos, shape);
        ++rock;
        if (rock == shapes.size()) {
            rock = 0;
        }
        --num_left;
    }

    height = -min_element(world.begin(), world.end(), [](auto& p1, auto& p2) { return p1[1] < p2[1]; })->at(1);

    return height + cycs * cyc_height;
}

int main()
{
    string moves;
    cin >> moves;

    Shapes shapes;
    PSet world;

    shapes.push_back({
        {0,0},
        {1,0},
        {2,0},
        {3,0}
        });

    shapes.push_back({
        {0,-1},
        {1,0},
        {1,-1},
        {2,-1},
        {1,-2}
        });

    shapes.push_back({
        {0,0},
        {1,0},
        {2,0},
        {2,-1},
        {2,-2}
        });

    shapes.push_back({
        {0,0},
        {0,-1},
        {0,-2},
        {0,-3}
        });

    shapes.push_back({
        {0,0},
        {1,0},
        {0,-1},
        {1,-1}
        });

    cout << "Part 1: " << part1(shapes, moves) << endl;
    cout << "Part 2: " << part2(shapes, moves) << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}