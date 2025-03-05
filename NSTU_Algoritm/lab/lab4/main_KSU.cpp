#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void min_banknotes(int n, vector<int>& denominations, int s) {
    sort(denominations.rbegin(), denominations.rend()); // Сортируем номиналы по убыванию
    vector<int> result;

    for (int denom : denominations) {
        while (s >= denom) {
            s -= denom;
            result.push_back(denom);
        }
    }

    if (s == 0) {
        for (int note : result) {
            cout << note << " ";
        }
        cout << endl;
    } else {
        cout << "No solution" << endl;
    }
}

int main() {
    int n, s;
    cin >> n;
    vector<int> denominations(n);

    for (int i = 0; i < n; i++) {
        cin >> denominations[i];
    }

    cin >> s;

    min_banknotes(n, denominations, s);

    return 0;
}
