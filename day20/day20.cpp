#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <conio.h>

using namespace std;

struct Record {
    __int64 val;
    int orig;
};

typedef list<Record> Records;

Records::iterator adv(Records& recs, const Records::iterator& it, __int64 count) {
    auto ret = it;

    if (count > 0) {
        for (int j = 0; j < count % recs.size(); ++j) {
            ++ret;
            if (ret == recs.end()) {
                ret = recs.begin();
            }
        }
    }

    if (count < 0) {
        for (int j = 0; j < (-count) % recs.size(); ++j) {
            if (ret == recs.begin()) {
                ret = recs.end();
            }
            --ret;
        }
    }

    return ret;
}

int main()
{
    vector<int> og;

    Records recs;
    vector<Records::iterator> rec_its;

    int pos = 0;
    while (!cin.eof()) {
        string s;
        cin >> s;
        og.push_back(stoi(s));
    }

    for (int x : og) {
        recs.push_back({ x, pos++ });
    }

    int zero = 0;
    for (auto it = recs.begin(); it != recs.end(); ++it) {
        rec_its.push_back(it);
        if (it->val == 0) {
            zero = rec_its.size() - 1;
        }
    }

    for (int i = 0; i < rec_its.size(); ++i) {
        auto it = rec_its[i];
        Record rec = *it;

        it = recs.erase(it);
        if (it == recs.end()) {
            it = recs.begin();
        }

        it = adv(recs, it, rec.val);

        if (it == recs.begin()) {
            it = recs.end();
        }

        it = recs.insert(it, rec);
        rec_its[i] = it;

        if (recs.size() < 10) {
            for (auto& r : recs) {
                cout << r.val << ", ";
            }
            cout << endl;
        }
    }

    __int64 part1 = 0;

    auto it = rec_its[zero];
    it = adv(recs, it, 1000 % recs.size());
    part1 = it->val;

    it = rec_its[zero];
    it = adv(recs, it, 2000 % recs.size());
    part1 += it->val;

    it = rec_its[zero];
    it = adv(recs, it, 3000 % recs.size());
    part1 += it->val;

    cout << "Part 1: " << part1 << endl;


    recs.clear();
    rec_its.clear();
    for (int x : og) {
        recs.push_back({ 811589153i64 * x, pos++ });
    }

    zero = 0;
    for (auto it = recs.begin(); it != recs.end(); ++it) {
        rec_its.push_back(it);
        if (it->val == 0) {
            zero = rec_its.size() - 1;
        }
    }

    for (int loops = 0; loops < 10; ++loops) {
        for (int i = 0; i < rec_its.size(); ++i) {
            auto it = rec_its[i];
            Record rec = *it;

            it = recs.erase(it);
            if (it == recs.end()) {
                it = recs.begin();
            }

            it = adv(recs, it, rec.val);

            if (it == recs.begin()) {
                it = recs.end();
            }

            it = recs.insert(it, rec);
            rec_its[i] = it;

            if (recs.size() < 10) {
                for (auto& r : recs) {
                    cout << r.val << ", ";
                }
                cout << endl;
            }
        }
    }

    __int64 part2 = 0;

    it = rec_its[zero];
    it = adv(recs, it, 1000 % recs.size());
    part2 = it->val;

    it = rec_its[zero];
    it = adv(recs, it, 2000 % recs.size());
    part2 += it->val;

    it = rec_its[zero];
    it = adv(recs, it, 3000 % recs.size());
    part2 += it->val;


    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}