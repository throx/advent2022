#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <conio.h>
#include "../shared/Split.h"

using namespace std;

const regex INPUT_R("Valve (..) has flow rate=(\\d+); tunnels? leads? to valves? (.*)");
const int TIME1 = 30;
const int TIME2 = 26;

// This optimisation didn't really save any time and made it harder to debug
#if 0
typedef short NAME;
NAME encode_name(const string& s) { return ((short)s[0] << 8) | s[1]; }
string decode_name(NAME n) { return string({(char)(n>>8), (char)n}); }
const NAME NONAME = NAME(0);
#else
typedef string NAME;
NAME encode_name(const string& s) { return s; }
string decode_name(const NAME& n) { return n; }
const NAME NONAME = NAME();
#endif

const NAME START = encode_name("AA");

struct Room {
    int rate;
    vector<NAME> links;
};

typedef map<NAME, Room> Index;


struct Valve {
    int rate;
    map<NAME, int> dist;
};

typedef map<NAME, Valve> ValveIndex;

typedef set<NAME> Visited;


int search(ValveIndex& index, NAME where, const Visited& visited, int time_left, string& curr_path) {
    Valve& valve = index[where];
    int my_score = valve.rate * time_left;
    curr_path = decode_name(where) + "@" + to_string(TIME1 - time_left) + " ";

    if (visited.size() == index.size()) {
        return my_score;
    }

    if (time_left <= 1) {
        return my_score;
    }

    auto new_visited = visited;
    new_visited.insert(where);

    int best_sub = 0;
    string og_cp = curr_path;
    for (auto& dd : valve.dist) {
        if (visited.find(dd.first) == visited.end()) {
            int new_time_left = time_left - dd.second - 1;
            if (new_time_left > 0) {
                string path;
                int sub = search(index, dd.first, new_visited, new_time_left, path);
                if (sub > best_sub) {
                    best_sub = sub;
                    curr_path = og_cp + path;
                }
            }
        }
    }

    return best_sub + my_score;
}

int search2(ValveIndex& index, NAME where1, int waited1, NAME where2, int waited2, const Visited& visited, int time_left, string& curr_path1, string& curr_path2) {
    Valve& valve1 = index[where1];
    Valve& valve2 = index[where2];
    int my_score = 0;
    if (waited1 == 0) {
        curr_path1 = decode_name(where1) + "@" + to_string(TIME2 - time_left) + " ";
        my_score += valve1.rate * time_left;
    }
    if (waited2 == 0) {
        curr_path2 = decode_name(where2) + "@" + to_string(TIME2 - time_left) + " ";
        my_score += valve2.rate * time_left;
    }     

    if (visited.size() == index.size()) {
        return my_score;
    }

    if (time_left <= 0) {
        return my_score;
    }

    // Perf note: Most of the time goes into copying this set.
    auto new_visited = visited;
    new_visited.insert(where1);
    new_visited.insert(where2);

    int best_sub = 0;

    // Ok.  This is fun.  Need to find the set of valid moves that can occur next.  It is either
    // 1 moving first, 2 waiting to move somewhere, both moving at same time, or 2 moving first,
    // 1 waiting to move somewhere.
    //
    // Note - it doesn't matter WHERE you're moving if you're waiting, only that it's somehwere
    // further than the wait time.
    //
    // So, get all the moves where:
    //  1 has time > wait time and (t - wt) < max 2 time (1 moves, 2 waits)
    //  2 has time > wait time and (t - wt) < max 1 time (2 moves, 1 waits)
    //  1 has no moves - use all of 2's
    //  2 has no moves - use all of 1's
    //  1 has time > wait time, 2 has time > wait time and (t1 - wt1) == (t2 - wt2)   (both move)

    map<NAME, int> time_left1, time_left2;
    int max1 = 0;
    int max2 = 0;
    for (auto& dd : valve1.dist) {
        if (new_visited.find(dd.first) == new_visited.end()) {
            if (dd.second + 1 > waited1 && dd.second + 1 <= time_left) {
                int tl = dd.second + 1 - waited1;
                time_left1[dd.first] = tl;
                max1 = max(max1, tl);
            }
        }
    }
    for (auto& dd : valve2.dist) {
        if (new_visited.find(dd.first) == new_visited.end()) {
            if (dd.second + 1 > waited2 && dd.second + 1 <= time_left) {
                int tl = dd.second + 1 - waited2;
                time_left2[dd.first] = tl;
                max2 = max(max2, tl);
            }
        }
    }

    struct Move {
        NAME where1;
        NAME where2;
        int wait;
    };
    vector<Move> moves;

    for (auto& tt : time_left1) {
        if (tt.second < max2 || time_left2.empty()) {
            moves.push_back({ tt.first, NONAME, tt.second });
        }
    }
    for (auto& tt : time_left2) {
        if (tt.second < max1 || time_left1.empty()) {
            moves.push_back({ NONAME, tt.first, tt.second });
        }
    }
    for (auto& tt1 : time_left1) {
        for (auto& tt2 : time_left2) {
            if (tt1.second == tt2.second && tt1.first != tt2.first) {
                moves.push_back({ tt1.first, tt2.first, tt1.second });
            }
        }
    }

    string og_cp1 = curr_path1;
    string og_cp2 = curr_path2;
    for (auto& move : moves) {
        int new_time_left = time_left - move.wait;
        int new_waited1 = waited1 + move.wait;
        int new_waited2 = waited2 + move.wait;
        NAME new_where1 = where1;
        NAME new_where2 = where2;
        if (move.where1 != NONAME) {
            new_waited1 = 0;
            new_where1 = move.where1;
        }
        if (move.where2 != NONAME) {
            new_waited2 = 0;
            new_where2 = move.where2;
        }

        string path1;
        string path2;
        int sub = search2(index, new_where1, new_waited1, new_where2, new_waited2, new_visited, new_time_left, path1, path2);
        if (sub > best_sub) {
            best_sub = sub;
            curr_path1 = og_cp1 + path1;
            curr_path2 = og_cp2 + path2;
        }
    }

    return best_sub + my_score;
}

int main()
{
    Index index;

    string s;
    while (getline(cin, s)) {
        smatch sm;
        if (regex_match(s.cbegin(), s.cend(), sm, INPUT_R)) {
            NAME name = encode_name(sm[1]);
            Room& room = index[name];
            room.rate = stoi(sm[2]);
            Split(sm[3], [&](const string& s) { room.links.push_back(encode_name(s)); }, ',');
        }
    }

    // Find distance between (non-zero) valves to seriously minimise the search space.
    // Thanks AA for the chat that tweaked my brain to exclude zero nodes from the
    // interconnections.
    ValveIndex valveindex;

    for (auto& rr : index) {
        NAME from = rr.first;
        Room& room = rr.second;
        if (room.rate == 0 && from != START) {
            continue;
        }

        map<NAME, int> done;
        multimap<NAME, int> todo;

        todo.insert({ from, 0 });
        while (!todo.empty()) {
            auto next = *todo.begin();
            todo.erase(todo.begin());

            Room& next_room = index[next.first];
            auto it = done.find(next.first);
            if (it == done.end() || it->second > next.second) {
                done[next.first] = next.second;
                for (auto& l : next_room.links) {
                    todo.insert({ l, next.second + 1 });
                }
            }
        }

        Valve& v = valveindex[from];
        v.rate = room.rate;
        v.dist = done;
        v.dist.erase(from);
        auto it = v.dist.begin(); 
        while (it != v.dist.end()) {
            Room& room = index[it->first];
            if (room.rate == 0) {
                it = v.dist.erase(it);
            }
            else {
                ++it;
            }
        }
    }

    string path;
    int part1 = search(valveindex, START, {}, TIME1, path);

    // RESULTS...

    cout << "Part 1: " << part1 << ": " << path << endl;

    string path1;
    string path2;
    int part2 = search2(valveindex, START, 0, START, 0, {}, TIME2, path1, path2);
    
    cout << "Part 2: " << part2 << ": " << endl << path1 << endl << path2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}