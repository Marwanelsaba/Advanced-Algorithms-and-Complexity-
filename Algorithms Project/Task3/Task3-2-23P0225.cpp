#include <iostream>
#include <cmath>

using namespace std;

int moveCount = 0;

void hanoi3(int n, int diskStart, char from, char to, char aux) {
    if (n == 0) return;

    hanoi3(n - 1, diskStart, from, aux, to);

    moveCount++;
    cout << "Move disk " << (diskStart + n - 1) << " from " << from << " to " << to << endl;

    hanoi3(n - 1, diskStart, aux, to, from);
}

void hanoi4(int n, int diskStart, char from, char to, char aux1, char aux2) {
    if (n == 0) return;

    if (n == 1) {
        moveCount++;
        cout << "Move disk " << diskStart << " from " << from << " to " << to << endl;
        return;
    }

    int k = n - (int)round(sqrt(2 * n + 1)) + 1;

    hanoi4(k, diskStart, from, aux1, aux2, to);
    hanoi3(n - k, diskStart + k, from, to, aux2);
    hanoi4(k, diskStart, aux1, to, from, aux2);
}

int main() {
    int n;
    cout << "Enter the number of disks: ";
    cin >> n;

    cout << "\nSolving Tower of Hanoi with " << n << " disks and 4 pegs:\n" << endl;

    hanoi4(n, 1, 'A', 'B', 'C', 'D');

    cout << "\nTotal number of moves: " << moveCount << endl;

    return 0;
}
