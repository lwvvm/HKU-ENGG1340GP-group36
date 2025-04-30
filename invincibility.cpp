#include "invincibility.h"

Invincibility::Invincibility() : remainingMoves(0), isActive(false) {}

void Invincibility::activate(int moves) {
    remainingMoves += moves;  // Add to existing moves (allows stacking)
    isActive = true;
}

bool Invincibility::useMove() {
    if (remainingMoves > 0 && isActive) {
        remainingMoves--;
        return true;
    }
    return false;
}

bool Invincibility::isInvincible() const {
    return isActive && remainingMoves > 0;
}

int Invincibility::getRemainingMoves() const {
    return remainingMoves;
}

void Invincibility::deactivate() {
    isActive = false;
    remainingMoves = 0;
}
