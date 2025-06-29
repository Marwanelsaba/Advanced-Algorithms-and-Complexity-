#include <iostream>
#include <vector>
using namespace std;

const int n = 8;
const int moves[8][2] = {
    { -2, -1 }, { -2, 1 }, { -1, -2 }, { -1, 2 },
    { 1, -2 }, { 1, 2 }, { 2, -1 }, { 2, 1 }
};

struct Move {
    int x;
    int y;
};
bool check_validation(int x, int y) {
    return (x >= 0 && x < n && y >= 0 && y < n);
}

int available_neighbours(const vector<vector<int>>& board, int x, int y) {
    int count = 0;
    for (auto& move : moves) {
        int next_x = x + move[0];
        int next_y = y + move[1];
        if (check_validation(next_x, next_y) && board[next_y][next_x] == 0) {
            count++;
        }
    }
    return count;
}
Move select_move(const vector<vector<int>>& board, int x, int y) {
    int min_neighbours = 8;
    Move next_move = { -1, -1 };
    for (auto& move : moves) {
        int next_x = x + move[0];
        int next_y = y + move[1];
        if (check_validation(next_x, next_y) && board[next_y][next_x] == 0) {
            int unvisited_neighbours = available_neighbours(board, next_x, next_y);
            if (unvisited_neighbours < min_neighbours) {
                min_neighbours = unvisited_neighbours;
                next_move = { next_x, next_y };
            }
        }
    }
    return next_move;
}
void board_display(const vector<vector<int>>& board, int x, int y) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == y && j == x) {
                cout << " KN ";
            }
            else {
                if (board[i][j] < 10)
                    cout << "  " << board[i][j] << " ";
                else
                    cout << " " << board[i][j] << " ";
            }
        }
        cout << "\n\n";
    }
}
void knighttour(int start_x, int start_y) {
    cout << "\nSTARTING KNIGHT'S TOUR FROM (" << start_x << ", " << start_y << ")\n";

    vector<vector<int>> board(n, vector<int>(n, 0));
    int x = start_x, y = start_y;
    board[y][x] = 1;

    int initial_x = x, initial_y = y;
    for (int i = 2; i <= n * n; i++) {
        Move next_move = select_move(board, x, y);
        if (next_move.x == -1 || next_move.y == -1) {
            cout << "TOUR ENDED EARLY, NOT ALL CELLS WERE VISITED\n";
            board_display(board, x, y);
            return;
        }
        x = next_move.x;
        y = next_move.y;
        board[y][x] = i;

        if (i == 64) {
            bool is_closed = false;
            for (auto& move : moves) {
                if (x + move[0] == initial_x && y + move[1] == initial_y) {
                    is_closed = true;
                    break;
                }
            }
            if (is_closed)
                cout << "KNIGHT'S TOUR IS CLOSED (RETURNED TO START POINT)\n";
            else
                cout << "KNIGHT'S TOUR IS OPEN\n\n";
            board_display(board, x, y);
        }
    }
}

int main() {
    int num_positions;
    cout << "Enter number of starting positions:\n";
    cin >> num_positions;

    for (int i = 0; i < num_positions; i++) {
        int x, y;
        cout << "\nEnter starting position: " << i + 1 << " (x,y):\n";
        cin >> x;
        cin >> y;

        if (check_validation(x, y)) {
            knighttour(x, y);
        }
        else {
            cout << "Invalid position\n";
        }
    }
    return 0;
}
