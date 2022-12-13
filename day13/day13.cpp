#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <conio.h>

using namespace std;

class Elem;

class NumList {
public:
    NumList() {}
    NumList(const Elem& e);

    void extract(istream& is);

    bool operator<(const NumList& other) const;
    bool operator==(const NumList& other) const;

private:
    vector<Elem> m_elems;
};

istream& operator>> (istream& is, NumList& n) {
    n.extract(is);
    return is;
}

class Elem {
public:
    Elem() {}
    Elem(const string& s);

    void extract(istream& is);
    bool operator<(const Elem& other) const;
    bool operator==(const Elem& other) const;
    bool operator!=(const Elem& other) const { return !((*this) == other); }

    bool is_int;
    int i;
    NumList n;
};

istream& operator>>(istream& is, Elem& e) {
    e.extract(is);
    return is;
}

int main()
{
    int index = 1;
    int part1 = 0;

    set<Elem> all;

    string s;
    while (getline(cin, s)) {
        if (s.empty()) continue;

        stringstream ss(s);
        Elem e1;
        ss >> e1;
        all.insert(e1);

        getline(cin, s);
        ss = stringstream(s);
        Elem e2;
        ss >> e2;
        all.insert(e2);

        if (e1 < e2) {
            part1 += index;
        }
        ++index;
    }

    Elem flag1("[[2]]");
    Elem flag2("[[6]]");

    all.insert(flag1);
    all.insert(flag2);

    int part2 = (distance(all.begin(), all.find(flag1)) + 1) * (distance(all.begin(), all.find(flag2)) + 1);

    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}

NumList::NumList(const Elem& e) {
    m_elems.push_back(e);
}

void NumList::extract(istream& is) {
    while (true) {
        int c = is.peek();
        if (c == EOF || c == ']') {
            return;
        }

        Elem e;
        is >> e;
        m_elems.push_back(e);

        if (is.peek() == ',') {
            is.ignore();
        }
    }
}

bool NumList::operator<(const NumList& other) const {
    int p = 0;
    while (true) {
        if (p >= other.m_elems.size()) {
            return false;
        }
        if (p >= m_elems.size()) {
            return true;
        }
        if (m_elems[p] < other.m_elems[p]) {
            return true;
        }
        if (other.m_elems[p] < m_elems[p]) {
            return false;
        }
        ++p;
    }
}

bool NumList::operator==(const NumList& other) const {
    if (m_elems.size() != other.m_elems.size()) return false;
    for (int i = 0; i < m_elems.size(); ++i) {
        if (m_elems[i] != other.m_elems[i]) {
            return false;
        }
    }
    return true;
}

Elem::Elem(const string& s) {
    stringstream ss(s);
    extract(ss);
}

void Elem::extract(istream& is) {
    if (is.peek() == '[') {
        is.ignore();
        is_int = false;
        i = 0;
        is >> n;
        if (is.peek() == ']') {
            is.ignore();
        }
    }
    else {
        is_int = true;
        is >> i;
    }
}

bool Elem::operator<(const Elem& other) const {
    if (is_int && other.is_int) {
        return i < other.i;
    }
    if (is_int && !other.is_int) {
        return NumList(*this) < other.n;
    }
    if (!is_int && other.is_int) {
        return n < NumList(other);
    }
    return n < other.n;
}

bool Elem::operator==(const Elem& other) const {
    if (is_int != other.is_int) return false;
    if (is_int && (i != other.i)) return false;
    return n == other.n;
}