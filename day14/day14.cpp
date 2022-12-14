#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <conio.h>
#include "../shared/Split.h"
#include "../shared/Point.h"

using namespace std;

int main()
{
    map<Point2, char> world;
    __int64 ymax = 0;

    string s;
    while (getline(cin, s)) {
        auto v = Split<Point2>(s, "->");
        DoLines(v.begin(), v.end(), [&](auto& p) {
            world[p] = '#';
            ymax = max(p[1], ymax);
            });
    }

    auto og_world = world;

    // Drop sand
    bool exit = false;
    int part1 = 0;
    while (!exit) {
        Point2 sand({ 500,0 });

        while (true) {
            // Drop down?
            if (world.find(sand + P2::D) == world.end()) {
                sand += P2::D;

                // Into the void?
                if (sand[1] > ymax) {
                    exit = true;
                    break;
                }
            }

            // Slide left?
            else if (world.find(sand + P2::D + P2::L) == world.end()) {
                sand += P2::D + P2::L;
            }

            // Slide right?
            else if (world.find(sand + P2::D + P2::R) == world.end()) {
                sand += P2::D + P2::R;
            }

            // Nowhere to go?  Stop
            else {
                world[sand] = 'o';
                ++part1;
                break;
            }
        }
    }

    // Drop sand 2
    world = og_world;
    DoLine(Point2({ 0,ymax + 2 }), Point2({1000,ymax+2}), [&](auto& p) {
        world[p] = '#';
        });
    ymax += 3;

    exit = false;
    int part2 = 0;
    while (!exit) {
        Point2 sand({ 500,0 });

        while (true) {
            // Drop down?
            if (world.find(sand + P2::D) == world.end()) {
                sand += P2::D;

                // Into the void?
                if (sand[1] > ymax) {
                    throw new exception("BOOM");
                }
            }

            // Slide left?
            else if (world.find(sand + P2::D + P2::L) == world.end()) {
                sand += P2::D + P2::L;
            }

            // Slide right?
            else if (world.find(sand + P2::D + P2::R) == world.end()) {
                sand += P2::D + P2::R;
            }

            // Nowhere to go?  Stop
            else {
                world[sand] = 'o';
                ++part2;

                if (sand == Point2({ 500, 0 })) {
                    exit = true;
                }
                break;
            }
        }
    }


    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}