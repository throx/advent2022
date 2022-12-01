#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    string s;
    vector<int> carry;
    int sum = 0;
    while (getline(cin, s)) {
        if (s.empty()) {
            carry.push_back(sum);
            sum = 0;
        }
        else {
            sum += stoi(s);
        }
    }
    carry.push_back(sum);

    sort(carry.begin(), carry.end());
    reverse(carry.begin(), carry.end());

    // RESULTS...

    cout << "Part 1: " << carry[0] << endl;
    cout << "Part 2: " << carry[0] + carry[1] + carry[2] << endl;
}