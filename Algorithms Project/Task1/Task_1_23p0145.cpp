#include <iostream>
#include <vector>
using namespace std;

const int N = 128;
int board[N][N];
int color[N * N];
int tromino_id = 1;

// Helper to check if a value exists in a vector
bool contains(const vector<int>& vec, int val) {
    for (int v : vec)
        if (v == val) return true;
    return false;
}

vector<int> getAdjacentTrominoIDs(int x, int y, int n) {
    vector<int> neighbors;
    int dx[] = { -1, 1, 0, 0 };
    int dy[] = { 0, 0, -1, 1 };

    for (int d = 0; d < 4; d++) {
        int nx = x + dx[d];
        int ny = y + dy[d];
        if (nx >= 0 && nx < n && ny >= 0 && ny < n) {
            int id = board[nx][ny];
            if (id != 0 && !contains(neighbors, id))
                neighbors.push_back(id);
        }
    }
    return neighbors;
}

int getColor(const vector<int>& adjacent) {
    bool used[4] = { false };  // index 1,2,3
    for (int id : adjacent)
        used[color[id]] = true;

    for (int c = 1; c <= 3; c++) {
        if (!used[c])
            return c;
    }
    return 1;
}

void placeTromino(int x1, int y1, int x2, int y2, int x3, int y3, int board_size) {
    vector<int> neighbors;
    vector<int> n1 = getAdjacentTrominoIDs(x1, y1, board_size);
    vector<int> n2 = getAdjacentTrominoIDs(x2, y2, board_size);
    vector<int> n3 = getAdjacentTrominoIDs(x3, y3, board_size);

    for (int id : n1)
        if (!contains(neighbors, id)) neighbors.push_back(id);
    for (int id : n2)
        if (!contains(neighbors, id)) neighbors.push_back(id);
    for (int id : n3)
        if (!contains(neighbors, id)) neighbors.push_back(id);

    int c = getColor(neighbors);
    board[x1][y1] = board[x2][y2] = board[x3][y3] = tromino_id;
    color[tromino_id] = c;
    tromino_id++;
}

void tile(int size, int x, int y, int miss_x, int miss_y, int board_size) {
    if (size == 2) {
        vector<pair<int, int>> cells;
        for (int dx = 0; dx < 2; dx++)
            for (int dy = 0; dy < 2; dy++)
                if (x + dx != miss_x || y + dy != miss_y)
                    cells.push_back({ x + dx, y + dy });
        placeTromino(cells[0].first, cells[0].second,
            cells[1].first, cells[1].second,
            cells[2].first, cells[2].second,
            board_size);
        return;
    }

    int mid = size / 2;
    int cx = x + mid, cy = y + mid;

    int qx = miss_x < cx ? 0 : 1;
    int qy = miss_y < cy ? 0 : 1;

    if (!(qx == 0 && qy == 0)) board[cx - 1][cy - 1] = -1;
    if (!(qx == 0 && qy == 1)) board[cx - 1][cy] = -1;
    if (!(qx == 1 && qy == 0)) board[cx][cy - 1] = -1;
    if (!(qx == 1 && qy == 1)) board[cx][cy] = -1;

    placeTromino(cx - 1, cy - 1, cx - 1, cy, cx, cy - 1, board_size);

    board[cx - 1][cy - 1] = board[cx - 1][cy] = board[cx][cy - 1] = board[cx][cy];  // clean overwrite

    tile(mid, x, y, qx == 0 && qy == 0 ? miss_x : cx - 1, qx == 0 && qy == 0 ? miss_y : cy - 1, board_size);
    tile(mid, x, cy, qx == 0 && qy == 1 ? miss_x : cx - 1, qx == 0 && qy == 1 ? miss_y : cy, board_size);
    tile(mid, cx, y, qx == 1 && qy == 0 ? miss_x : cx, qx == 1 && qy == 0 ? miss_y : cy - 1, board_size);
    tile(mid, cx, cy, qx == 1 && qy == 1 ? miss_x : cx, qx == 1 && qy == 1 ? miss_y : cy, board_size);
}

int main() {
    int n;
    cout << "Enter n (board size will be 2^n x 2^n): ";
    cin >> n;

    int size = 1 << n;
    if (n < 2 || size > N) {
        cout << "Invalid size\n";
        return 1;
    }

    int miss_x, miss_y;
    cout << "Enter missing cell coordinates (row col): ";
    cin >> miss_x >> miss_y;

    if (miss_x < 0 || miss_x >= size || miss_y < 0 || miss_y >= size) {
        cout << "Invalid coordinates\n";
        return 1;
    }

    board[miss_x][miss_y] = 0;
    tile(size, 0, 0, miss_x, miss_y, size);

    cout << "\nTiled Board (Colors):\n";
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == miss_x && j == miss_y) cout << "X\t";
            else cout << color[board[i][j]] << "\t";
        }
        cout << endl;
    }

    return 0;
}

