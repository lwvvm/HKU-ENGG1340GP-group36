#include "invincibility.h"

Invincibility::Invincibility() 
    : remainingMoves(0), isActive(false) {}

void Invincibility::activate(int moves) {
    remainingMoves = moves;
    isActive = true;
}

void Invincibility::useMove() {
    if (remainingMoves > 0) {
        remainingMoves--;
    }
    if (remainingMoves <= 0) {
        deactivate();
    }
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
