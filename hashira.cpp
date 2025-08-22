#include <bits/stdc++.h>
#include "json.hpp"  // nlohmann/json.hpp (put this header in same folder)

using json = nlohmann::json;
using namespace std;

// Convert string in given base to decimal (big number using long long)
long long convertToDecimal(const string &value, int base) {
    long long result = 0;
    for (char c : value) {
        int digit;
        if (isdigit(c)) digit = c - '0';
        else if (isalpha(c)) digit = 10 + (tolower(c) - 'a');
        else continue;

        result = result * base + digit;
    }
    return result;
}

// Lagrange interpolation at x=0
long long lagrangeInterpolation(const vector<pair<int, long long>> &points, int k) {
    long double result = 0.0;

    for (int i = 0; i < k; i++) {
        long double xi = points[i].first;
        long double yi = points[i].second;

        long double term = yi;
        for (int j = 0; j < k; j++) {
            if (i != j) {
                long double xj = points[j].first;
                term *= (0.0 - xj) / (xi - xj);
            }
        }
        result += term;
    }
    return llround(result); // nearest integer
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: ./main <input.json>" << endl;
        return 1;
    }

    // Load JSON
    ifstream file(argv[1]);
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return 1;
    }
    json j;
    file >> j;

    int n = j["keys"]["n"];
    int k = j["keys"]["k"];

    vector<pair<int, long long>> points;

    for (int i = 1; i <= n; i++) {
        string idx = to_string(i);
        if (j.contains(idx)) {
            int base = stoi((string) j[idx]["base"]);
            string val = j[idx]["value"];
            long long y = convertToDecimal(val, base);
            points.push_back({i, y});
        }
    }

    if ((int)points.size() < k) {
        cerr << "Not enough points to interpolate!" << endl;
        return 1;
    }

    // Use first k points (could also choose random k)
    vector<pair<int,long long>> subset(points.begin(), points.begin() + k);

    long long secret = lagrangeInterpolation(subset, k);

    cout << "Recovered secret (constant term c) = " << secret << endl;

    return 0;
}
