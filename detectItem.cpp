#include "detectItem.h"
#include <iostream>
#include <sstream>

using namespace std;

void useMineRevealer(
    int &totalScore,
    vector<vector<bool>> &revealed,
    const vector<vector<bool>> &mineGrid,
    int rows,
    int cols
) {
    if (totalScore < 1) { // Check if the user has at least 1 point
        cout << "\033[1;31mNot enough total points to use the Mine Revealer! You need at least 1 point.\033[0m\n";
        return;
    }

    int r = -1, c = -1;
    while (true) {
        cout << "Enter the center of the 3x3 area (e.g., '3 5'): ";
        string coordInput;
        getline(cin, coordInput);

        stringstream ss(coordInput);
        if (!(ss >> r >> c)) {
            cout << "Invalid input! Please enter TWO numbers separated by space.\n";
            continue;
        }

        string remaining;
        if (ss >> remaining) {
            cout << "Invalid input! Only two numbers allowed (e.g., '3 5').\n";
            continue;
        }

        if (r < 0 || r >= rows || c < 0 || c >= cols) {
            cout << "Invalid position! Row and column must be between 0 and " << rows - 1 << ".\n";
            continue;
        }

        break;
    }

    // Deduct 1 point from total score
    totalScore--;

    // Reveal mines in the 3x3 range
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            int nr = r + dr;
            int nc = c + dc;
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                revealed[nr][nc] = true;
            }
        }
    }

    cout << "\033[1;33mMines in the 3x3 area have been revealed! 1 point deducted from your total score.\033[0m\n";
}