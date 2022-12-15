#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include "../shared/Point.h"
#include "../shared/Rect.h"

using namespace std;

const regex INPUT_R("Sensor at x=(-?\\d+), y=(-?\\d+): closest beacon is at x=(-?\\d+), y=(-?\\d+)");

int main()
{
    map<Point2, __int64> sensors;
    set<Point2> beacons;
    Rect2 bounds;
    __int64 max_range = 0;
    bool first = true;

    string s;
    while (getline(cin, s)) {
        smatch sm;
        if (regex_match(s.cbegin(), s.cend(), sm, INPUT_R)) {
            Point2 sensor({ stoi(sm[1]),stoi(sm[2]) });
            Point2 beacon({ stoi(sm[3]),stoi(sm[4]) });

            beacons.insert(beacon);

            __int64 range = Dist(sensor, beacon);
            sensors[sensor] = range;

            max_range = max(max_range, range);

            if (first) {
                bounds = Rect2(sensor);
                first = false;
            }
            else {
                bounds = bounds.ExpandTo(sensor);
            }
        }
    }

    bounds = bounds.ExpandBy(max_range);
    cout << bounds << endl;

    int YPOS = 2000000;
    if (sensors.size() < 20) {
        YPOS = 10;
    }

    int part1 = 0;
    for (int x = bounds.start()[0]; x < bounds.end()[0]; ++x) {
        Point2 pos({ x, YPOS });
        bool found = false;
        for (auto& ss : sensors) {
            Point2 sensor = ss.first;
            __int64 range = ss.second;

            if (Dist(sensor, pos) <= range) {
                found = true;
                break;
            }
        }

        if (found && beacons.find(pos) == beacons.end()) {
            ++part1;
        }
    }

    cout << "Part 1: " << part1 << endl;

    Rect2 search(Point2(), Point2({ 4000000,4000000 }));
    if (sensors.size() < 20) {
        search = Rect2(Point2(), Point2({ 20,20 }));
    }

    Point2 result;


    // To find the only point which isn't covered, search around each sensor at a range
    // of 1 greater than its range.  This is a much smaller search space than brute force.
    for (auto& ss : sensors) {
        Point2 sensor = ss.first;
        __int64 range = ss.second;

        ++range;

        for (int x = max(search.start()[0], sensor[0] - range); x < min(search.end()[0], sensor[0] + range + 1); ++x) {
            int y = sensor[1] + (range - abs(sensor[0] - x));
            if (y >= search.start()[1] && y <= search.end()[1]) {

                Point2 pos({ x,y });

                bool found = false;
                for (auto& ss : sensors) {
                    Point2 sensor = ss.first;
                    __int64 range = ss.second;

                    if (Dist(sensor, pos) <= range) {
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    result = pos;
                    goto FOUND_IT;
                }
            }

            y = sensor[1] - (range - abs(sensor[0] - x));
            if (y >= search.start()[1] && y <= search.end()[1]) {

                Point2 pos({ x,y });

                bool found = false;
                for (auto& ss : sensors) {
                    Point2 sensor = ss.first;
                    __int64 range = ss.second;

                    if (Dist(sensor, pos) <= range) {
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    result = pos;
                    goto FOUND_IT;
                }
            }
        }
    }

FOUND_IT:
    __int64 part2 = result[0] * 4000000i64 + result[1];

    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}