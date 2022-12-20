#include <iostream>
#include <string>
#include <regex>
#include <map>
#include <vector>
#include <conio.h>

using namespace std;

const regex INPUT_R("Blueprint \\d+: Each ore robot costs (\\d+) ore. Each clay robot costs (\\d+) ore. Each obsidian robot costs (\\d+) ore and (\\d+) clay. Each geode robot costs (\\d+) ore and (\\d+) obsidian.");

typedef vector<int> Robot; // Type -> Count

struct Blueprint : public vector<Robot> {
    vector<int> typemax;
};

typedef vector<Blueprint> Blueprints;

enum RobotType {
    ORE = 0,
    CLAY,
    OBSIDIAN,
    GEODE,
    MAXTYPE
};

struct Turn {
    int num;
    unsigned char resources[4];
    unsigned char robots[4];

    bool operator < (const Turn& other) const {
        if (num < other.num) return true;
        if (num > other.num) return false;

        int res = *(__int32*)resources;
        int ores = *(__int32*)other.resources;
        if (res < ores) return true;
        if (res > ores) return false;

        int rob = *(__int32*)robots;
        int orob = *(__int32*)other.robots;
        if (rob < orob) return true;
        if (rob > orob) return false;

        return false;
    }

    bool operator == (const Turn& other) const {
        if (num != other.num) return false;

        int res = *(__int32*)resources;
        int ores = *(__int32*)other.resources;
        if (res != ores) return false;

        int rob = *(__int32*)robots;
        int orob = *(__int32*)other.robots;
        return rob == orob;
    }
};

namespace std {

    template <>
    struct hash<Turn>
    {
        std::size_t operator()(const Turn& t) const
        {
            using std::size_t;
            using std::hash;
            using std::string;

            size_t seed = t.num;
            seed ^= ((size_t)(*(int*)t.resources)) << 8;
            seed ^= ((size_t)(*(int*)t.robots)) << 36;

            return seed;
        }
    };

}

// For some reason, map<> is faster than unordered_map<> for me.
typedef map<Turn, int> Cache;

static int bb = 999;

int dfs_build(const Blueprint& b, int time_limit, const Turn& turn, Cache& cache)
{
    int now = turn.num;

    // Compute turn
    if (now == time_limit) {
        return turn.resources[GEODE];
    }

    {
        static int cc = 0;
        ++cc;
        if (cc % 10000000 == 0) {
            cout << cc << endl;
        }
    }

    auto it = cache.find(turn);
    if (it != cache.end()) {
        return it->second;
    }

    // Try to build all 4
    int best = 0;
    Turn new_turn;
    new_turn.num = turn.num + 1;

    bool can_build_all = true;
    for (int r = GEODE; r >= 0; --r) {

        // Check if we don't need it - this was the magic that made it computable.  Thanks AA.
        if (turn.robots[r] >= b.typemax[r]) {
            continue;
        }

        // Check if we can build it
        bool can_build = true;
        for (int c = 0; c < MAXTYPE; ++c) {
            if (turn.resources[c] < b[r][c]) {
                can_build = false;
                break;
            }
        }

        if (can_build) {
            for (int c = 0; c < MAXTYPE; ++c) {
                new_turn.resources[c] = turn.resources[c] - b[r][c] + turn.robots[c];
                new_turn.robots[c] = turn.robots[c];
            }
            new_turn.robots[r]++;

            best = max(best, dfs_build(b, time_limit, new_turn, cache));

            if (r == GEODE) {
                cache[turn] = best;
                return best;
            }
        }
        else {
            can_build_all = false;
        }
    }

    // Only check the "don't build" option if there's not a surplus for every robot
    if (!can_build_all) {
        for (int c = 0; c < MAXTYPE; ++c) {
            new_turn.resources[c] = turn.resources[c] + turn.robots[c];
            new_turn.robots[c] = turn.robots[c];
        }

        best = max(best, dfs_build(b, time_limit, new_turn, cache));
    }

    {
        if (bb >= now) {
            bb = now;
            cout << "BB: " << now << endl;
        }
    }

    cache[turn] = best;
    return best;
}

int dfs_build(const Blueprint& b, int time_limit) {
    Turn turn = {
        0,
        {0,0,0,0},
        {1,0,0,0}
    };
    bb = 99;

    Cache cache;

    int r = dfs_build(b, time_limit, turn, cache);

    cout << "Best = " << r << endl;
    return r;
}

int main()
{
    Blueprints blueprints;

    string s;
    while (getline(cin, s)) {
        smatch sm;
        if (regex_match(s.cbegin(), s.cend(), sm, INPUT_R)) {
            Robot ore;
            ore.resize(4);
            ore[ORE] = stoi(sm[1]);

            Robot clay;
            clay.resize(4);
            clay[ORE] = stoi(sm[2]);

            Robot obsidian;
            obsidian.resize(4);
            obsidian[ORE] = stoi(sm[3]);
            obsidian[CLAY] = stoi(sm[4]);

            Robot geode;
            geode.resize(4);
            geode[ORE] = stoi(sm[5]);
            geode[OBSIDIAN] = stoi(sm[6]);

            Blueprint b;
            b.push_back(ore);
            b.push_back(clay);
            b.push_back(obsidian);
            b.push_back(geode);

            b.typemax.resize(4);
            b.typemax[ORE] = max({ ore[ORE], clay[ORE], obsidian[ORE], geode[ORE] });
            b.typemax[CLAY] = obsidian[CLAY];
            b.typemax[OBSIDIAN] = geode[OBSIDIAN];
            b.typemax[GEODE] = std::numeric_limits<int>().max();

            blueprints.push_back(b);
        }
    }
    /*
    int part1 = 0;
    const int TIME1 = 24;
    for (int i = 0; i < blueprints.size(); ++i) {
        part1 += dfs_build(blueprints[i], TIME1) * (i + 1);
    }
    cout << "Part 1: " << part1 << endl;
    */

    int part2 = 1;
    const int TIME2 = 32;
    for (int i = 0; i < blueprints.size() && i < 3; ++i) {
        part2 *= dfs_build(blueprints[i], TIME2);
    }
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}