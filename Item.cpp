#include "Item.h"
#include <sstream>
#include <iostream>

using namespace std;

Item::Item() : 
    remainingReveals(0),
    invincibilityActive(false),
    shieldActive(false),
    shieldActivationTime(0),
    shieldDurationSeconds(0) 
{}

// AutoSweep implementation
void Item::performAutoSweep(
    vector<vector<bool>>& revealed,
    const vector<vector<bool>>& mineGrid,
    int rows,
    int cols) 
{
    int r = -1, c = -1;
    while (true) {
        cout << "Enter the center of the 3x3 area (e.g., '2 3'): ";
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

    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            int nr = r + dr;
            int nc = c + dc;
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                revealed[nr][nc] = true;
            }
        }
    }

    cout << "\033[1;33mMines in the 3x3 area have been revealed!\033[0m\n";
}

// Invincibility implementations
void Item::activateInvincibility(int reveals) {
    remainingReveals = reveals;
    invincibilityActive = true;
}

void Item::countInvincibilityReveal() {
    if (!invincibilityActive) return;
    
    remainingReveals--;
    if (remainingReveals <= 0) {
        deactivateInvincibility();
    }
}

bool Item::isInvincible() const {
    return invincibilityActive && remainingReveals > 0;
}

int Item::getRemainingInvincibilityReveals() const {
    return remainingReveals;
}

void Item::deactivateInvincibility() {
    invincibilityActive = false;
    remainingReveals = 0;
}

// Shield implementations
void Item::activateShield(int seconds) {
    shieldActive = true;
    shieldActivationTime = time(nullptr);
    shieldDurationSeconds = seconds;
}

void Item::deactivateShield() {
    shieldActive = false;
    cout << "\033[1;34mShield has expired.\033[0m\n";
}

bool Item::isShieldActive() const {
    return shieldActive && 
           (difftime(time(nullptr), shieldActivationTime) < shieldDurationSeconds);
}

int Item::getShieldRemainingTime() const {
    if (!shieldActive) return 0;
    int remaining = shieldDurationSeconds - difftime(time(nullptr), shieldActivationTime);
    return remaining > 0 ? remaining : 0;
}

// Combined protection
bool Item::isProtected() const {
    return (invincibilityActive && remainingReveals > 0) || 
           (shieldActive && (difftime(time(nullptr), shieldActivationTime) < shieldDurationSeconds));
}
