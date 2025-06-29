#include <iostream>
#include <vector>
#include <queue>
using namespace std;

// Board dimensions
static const int R = 4, C = 3;

// Knight moves
const int dr[8] = { +2,+2,-2,-2,+1,-1,+1,-1 };
const int dc[8] = { +1,-1,+1,-1,+2,+2,-2,-2 };

// A board state
struct State {
    int a[R][C];
    bool operator==(State const& o) const {
        for (int i = 0; i < R; i++) for (int j = 0; j < C; j++)
            if (a[i][j] != o.a[i][j]) return false;
        return true;
    }
};

State make_start() {
    State s{};
    for (int c = 0; c < C; c++) {
        s.a[0][c] = -1;
        s.a[R - 1][c] = +1;
    }
    return s;
}
State make_goal() {
    State g{};
    for (int c = 0; c < C; c++) {
        g.a[0][c] = +1;
        g.a[R - 1][c] = -1;
    }
    return g;
}

// BFS node for heuristic
struct HNode { int r, c, d; };

// Heuristic: sum of minimal knight‐move distances
int heuristic(const State& s, const State& goal) {
    int H = 0;
    bool seen[R][C];
    for (int r = 0; r < R; r++) {
        for (int c = 0; c < C; c++) {
            int piece = s.a[r][c];
            if (piece == 0) continue;
            // BFS until we hit same color target
            for (int i = 0; i < R; i++) for (int j = 0; j < C; j++) seen[i][j] = false;
            queue<HNode> q;
            seen[r][c] = true;
            q.push(HNode{ r,c,0 });
            while (!q.empty()) {
                HNode u = q.front(); q.pop();
                if (goal.a[u.r][u.c] == piece) {
                    H += u.d;
                    break;
                }
                for (int k = 0; k < 8; k++) {
                    int nr = u.r + dr[k], nc = u.c + dc[k];
                    if (nr < 0 || nr >= R || nc < 0 || nc >= C) continue;
                    if (seen[nr][nc]) continue;
                    seen[nr][nc] = true;
                    q.push(HNode{ nr,nc,u.d + 1 });
                }
            }
        }
    }
    return H;
}

// Generate all single‐knight jumps into empty squares
vector<State> neighbors(const State& s) {
    vector<State> out;
    for (int r = 0; r < R; r++) {
        for (int c = 0; c < C; c++) {
            int piece = s.a[r][c];
            if (piece == 0) continue;
            for (int k = 0; k < 8; k++) {
                int nr = r + dr[k], nc = c + dc[k];
                if (nr < 0 || nr >= R || nc < 0 || nc >= C) continue;
                if (s.a[nr][nc] != 0) continue;
                State t = s;
                t.a[r][c] = 0;
                t.a[nr][nc] = piece;
                out.push_back(t);
            }
        }
    }
    return out;
}

// A* node
struct ANode {
    State s;
    int g, f;
    int parent;   
};

// Pop index of best‐f element from 'open'
int pop_best(vector<ANode>& open) {
    int bi = 0;
    for (int i = 1; i < (int)open.size(); i++) {
        if (open[i].f < open[bi].f) bi = i;
    }
    ANode best = open[bi];
    open.erase(open.begin() + bi);
    open.push_back(best);
    return open.size() - 1;
}

// Print a state
void print_state(const State& s) {
    for (int r = 0; r < R; r++) {
        for (int c = 0; c < C; c++) {
            char ch = s.a[r][c] == +1 ? 'W'
                : s.a[r][c] == -1 ? 'B'
                : '.';
            cout << ch << ' ';
        }
        cout << "\n";
    }
}

int main() {
    State start = make_start();
    State goal = make_goal();

    vector<ANode> open;
    vector<ANode> closed;

    // init open
    int h0 = heuristic(start, goal);
    open.push_back(ANode{ start, 0, h0, -1 });

    int goal_idx = -1;
    while (!open.empty()) {
        int oi = pop_best(open);
        ANode node = open.back();
        open.pop_back();

        // skip if already closed
        bool in_closed = false;
        for (auto& cn : closed) {
            if (cn.s == node.s) { in_closed = true; break; }
        }
        if (in_closed) continue;

        // record in closed
        closed.push_back(node);
        int my_idx = closed.size() - 1;

        // goal?
        if (node.s == goal) {
            goal_idx = my_idx;
            break;
        }

        // expand
        auto nbrs = neighbors(node.s);
        for (auto& ns : nbrs) {
            // skip closed
            bool seen = false;
            for (auto& cn : closed) {
                if (cn.s == ns) { seen = true; break; }
            }
            if (seen) continue;

            int g2 = node.g + 1;
            int f2 = g2 + heuristic(ns, goal);

            // skip worse in open
            bool worse = false;
            for (auto& on : open) {
                if (on.s == ns && on.f <= f2) {
                    worse = true; break;
                }
            }
            if (worse) continue;

            open.push_back(ANode{ ns, g2, f2, my_idx });
        }
    }

    if (goal_idx < 0) {
        cout << "No solution\n";
        return 0;
    }

    // Reconstruct path
    vector<State> path;
    for (int cur = goal_idx; cur != -1; cur = closed[cur].parent) {
        path.push_back(closed[cur].s);
    }
    reverse(path.begin(), path.end());

    cout << "Solved in " << path.size() - 1 << " moves:\n\n";
    for (int i = 0; i < (int)path.size(); i++) {
        cout << "Step " << i << ":\n";
        print_state(path[i]);
        cout << "\n";
    }
    return 0;
}

