

#include <iostream>
#include <vector>
using namespace std;

//=============================================================================
// Core divide-and-conquer shot plan generator
//=============================================================================
vector<int> generateShotPlan(int L, int R) {
    if (L == R) {
        return {L};
    }
    int M = (L + R) / 2;
    vector<int> plan;

    // 1) Knock out the left half, with double-barrier after each shot
    auto leftPlan = generateShotPlan(L, M);
    for (int p : leftPlan) {
        plan.push_back(p);
        plan.push_back(M);   // barrier shot at M
        plan.push_back(M+1); // barrier shot at M+1
    }

    // 2) Knock out the right half, with double-barrier before each shot
    auto rightPlan = generateShotPlan(M+1, R);
    for (int p : rightPlan) {
        plan.push_back(M);   // barrier shot at M
        plan.push_back(M+1); // barrier shot at M+1
        plan.push_back(p);
    }

    return plan;
}


//=============================================================================
// Option 1: View the static plan
//=============================================================================
void viewPlan(const vector<int>& plan) {
    cout << "\n--- Full Shot Sequence ---\n";
    for (int i = 0; i < plan.size(); ++i) {
        cout << "Shot " << (i + 1)
             << ": at position " << plan[i] << "\n";
    }
    cout << "--- End of Plan ---\n\n";
}

//=============================================================================
// Option 2: Play interactively against a moving target
//=============================================================================
void playPlan(const vector<int>& plan, int n) {
    cout << "\n--- Play Mode ---\n";
    int target;
    // choose a starting spot
    while (true) {
        cout << "Target starts at (1 - " << n << "): ";
        if (cin >> target && target >= 1 && target <= n) break;
        cin.clear();
        char c;
        while (cin.get(c) && c != '\n') { }
        cout << "  Enter an integer between 1 and " << n << ".\n";
    }

    for (int i = 0; i <plan.size(); ++i) {
        int shotPos = plan[i];
        cout << "Shot " << (i + 1)
             << " at " << shotPos << ": ";
        if (shotPos == target) {
            cout << "HIT!\n";
            return;
        }
        cout << "miss.\n";

        // target moves
        int move;
        while (true) {
            cout << "  Move target (-1 left, +1 right): ";
            if (cin >> move && (move == -1 || move == 1)) {
                if ((move == -1 && target == 1) ||
                    (move ==  1 && target == n)) {
                    cout << "   Cannot move out of bounds.\n";
                } else {
                    target += move;
                    cout << "   Target now at " << target << ".\n";
                    break;
                }
            } else {
                cin.clear();
                char c;
                while (cin.get(c) && c != '\n') { }
                cout << "   Enter -1 or +1 \n";
            }
        }
    }

    //cout << "Plan exhausted—target never hit \n\n";
}

int main() {
    int n;
    while (true) {
        cout << "Enter number of hiding spots (n > 1): ";
        if (cin >> n && n > 1) break;
        cin.clear();
        char c;
        while (cin.get(c) && c != '\n') { }
        cout << "  Invalid; please enter an integer > 1.\n";
    }

    auto plan = generateShotPlan(1, n);

    while (true) {
        cout << "Menu:\n"
             << " 1) View full shot sequence\n"
             << " 2) Play as the target\n"
             << " 0) Exit\n"
             << "Enter choice: ";
        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            char c;
            while (cin.get(c) && c != '\n') { }
            cout << "  Invalid enter 0, 1, or 2 \n";
            continue;
        }
        if (choice == 0) {
            cout << "Goodbye \n";
            break;
        }
        else if (choice == 1) {
            viewPlan(plan);
        }
        else if (choice == 2) {
            playPlan(plan, n);
        }
        else {
            cout << "  Unknown choic enter 0, 1, or 2.\n";
        }
    }

    return 0;
}
