#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <set>
#include <algorithm>
#include <list>
#include <conio.h>

using namespace std;

int main()
{
    vector<string> sacks;
    int value = 0;

    while (!cin.eof())
    {
        string s;
        cin >> s;
        sacks.push_back(s);

        string part1 = s.substr(0, s.length() / 2);
        string part2 = s.substr(s.length() / 2);

        sort(part1.begin(), part1.end());
        sort(part2.begin(), part2.end());

        vector<char> common;
        set_intersection(part1.begin(), part1.end(), part2.begin(), part2.end(), back_inserter(common));
        cout << "Sack " << s << " common = " << common[0] << endl;

        value += isupper(common[0]) ? (common[0] - 'A' + 27) : (common[0] - 'a' + 1);
    }

    int value2 = 0;

    for (size_t i = 0; i < sacks.size(); i += 3)
    {
        vector<char> common1;
        vector<char> common2;

        string s1 = sacks[i];
        string s2 = sacks[i + 1];
        string s3 = sacks[i + 2];

        sort(s1.begin(), s1.end());
        sort(s2.begin(), s2.end());
        sort(s3.begin(), s3.end());

        set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), back_inserter(common1));
        set_intersection(s3.begin(), s3.end(), common1.begin(), common1.end(), back_inserter(common2));

        cout << "Group " << i / 3 << " common = " << common2[0] << endl;
        value2 += isupper(common2[0]) ? (common2[0] - 'A' + 27) : (common2[0] - 'a' + 1);
    }

    // RESULTS...

    cout << "Part 1: " << value << endl;
    cout << "Part 2: " << value2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}