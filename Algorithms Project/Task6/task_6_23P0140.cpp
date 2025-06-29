#include <iostream>
#include <vector>
#include <unordered_map>
#include <sstream>

using namespace std;

void printLines(const vector<pair<int, int>>& line) {
    for (const auto& point : line) {
        cout << "(" << point.first << "," << point.second << ") -> ";
    }
    cout << endl;
}

string matrixToKey(const vector<vector<bool>>& visited) {
    stringstream ss;
    for (const auto& row : visited)
        for (bool val : row)
            ss << val;
    return ss.str();
}

unordered_map<string, pair<int, int>> memo;

pair<int, int> dpMove(string direction, int r, int c, int x, vector<vector<pair<int, int>>>& Lines, vector<pair<int, int>>& line, const vector<vector<pair<int, int>>>& arr, vector<vector<bool>>& visited) {
    string key = direction + ":" + to_string(r) + "," + to_string(c) + "," + to_string(x) + "," + matrixToKey(visited);
    if (memo.find(key) != memo.end()) return memo[key];

    line.push_back(arr[r][c]);
    for (int i = 0; i < x; i++) {
        if (direction == "Left") c -= 1;
        else if (direction == "Right") c += 1;
        else if (direction == "Up") r -= 1;
        else if (direction == "Down") r += 1;

        line.push_back(arr[r][c]);
        visited[r][c] = true;
    }
    Lines.push_back(line);
    return memo[key] = { r, c };
}

void findPath(int n) {
    vector<vector<pair<int, int>>> arr(n, vector<pair<int, int>>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            arr[i][j] = { i, j };

    int mid = (n % 2 == 0) ? (n / 2 - 1) : (n / 2);
    vector<vector<pair<int, int>>> Lines;
    vector<vector<bool>> visited(n, vector<bool>(n, false));

    vector<pair<int, int>> Line1 = { arr[mid + 1][mid - 1], arr[mid][mid], arr[mid - 1][mid + 1] };
    vector<pair<int, int>> Line2 = { arr[mid - 1][mid + 1], arr[mid][mid + 1], arr[mid + 1][mid + 1] };
    if (mid + 2 < n) Line2.push_back(arr[mid + 2][mid + 1]);

    vector<pair<int, int>> Line3;
    if (mid + 2 < n) Line3.push_back(arr[mid + 2][mid + 1]);
    Line3.insert(Line3.end(), { arr[mid + 1][mid], arr[mid][mid - 1] });
    if (mid - 2 >= 0) Line3.push_back(arr[mid - 1][mid - 2]);

    vector<pair<int, int>> Line4;
    if (mid - 2 >= 0) Line4.push_back(arr[mid - 1][mid - 2]);
    Line4.insert(Line4.end(), { arr[mid - 1][mid - 1], arr[mid - 1][mid], arr[mid - 1][mid + 1] });
    if (mid + 2 < n) Line4.push_back(arr[mid - 1][mid + 2]);

    Lines.push_back(Line1);
    Lines.push_back(Line2);
    Lines.push_back(Line3);
    Lines.push_back(Line4);

    if (n == 4) {
        vector<pair<int, int>> line;
        int r = mid - 1, c = mid + 2;
        pair<int, int> result = dpMove("Down", r, c, 3, Lines, line, arr, visited);
        r = result.first; c = result.second;

        line.clear();
        result = dpMove("Left", r, c, 3, Lines, line, arr, visited);
        r = result.first; c = result.second;
    }
    else if (n > 4) {
        vector<pair<int, int>> Line5;
        for (int i = 0; i < 4; i++) Line5.push_back(arr[mid - 1 + i][mid + 2]);
        Lines.push_back(Line5);

        int r = mid + 2, c = mid + 2, x = 4, dir = 0;
        while (x < n - 1) {
            vector<pair<int, int>> line;
            pair<int, int> result;

            if (dir == 0) {
                result = dpMove("Left", r, c, x, Lines, line, arr, visited);
                r = result.first; c = result.second;
                dir = 1;
            }
            else if (dir == 1) {
                result = dpMove("Up", r, c, x, Lines, line, arr, visited);
                r = result.first; c = result.second;
                dir = 2;
                x++;
            }
            else if (dir == 2) {
                result = dpMove("Right", r, c, x, Lines, line, arr, visited);
                r = result.first; c = result.second;
                dir = 3;
            }
            else if (dir == 3) {
                result = dpMove("Down", r, c, x, Lines, line, arr, visited);
                r = result.first; c = result.second; dir = 0;
                x++;
            }
        }
        if (x == n - 1) {
            vector<pair<int, int>> li;
            pair<int, int> result;

            if (n % 2 != 0) {
                result = dpMove("Left", r, c, x, Lines, li, arr, visited);
                r = result.first; c = result.second;

                li.clear();
                result = dpMove("Up", r, c, x, Lines, li, arr, visited);
                r = result.first; c = result.second;

                li.clear();
                result = dpMove("Right", r, c, x, Lines, li, arr, visited);
                r = result.first; c = result.second;
            }
            else {
                result = dpMove("Right", r, c, x, Lines, li, arr, visited);
                r = result.first; c = result.second;

                li.clear();
                result = dpMove("Down", r, c, x, Lines, li, arr, visited);
                r = result.first; c = result.second;

                li.clear();
                result = dpMove("Left", r, c, x, Lines, li, arr, visited);
                r = result.first; c = result.second;
            }
        }
    }

    for (const auto& row : Lines)
        printLines(row);

    cout << "\nTotal Number of Lines: " << Lines.size() << endl;
}

int main() {
    int n;
    cout << "Enter the Order of the dots: ";
    cin >> n;
    findPath(n);
    return 0;
}
