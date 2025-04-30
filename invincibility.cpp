#include "invincibility.h"

Invincibility::Invincibility() : remainingReveals(0), activeStatus(false) {}

void Invincibility::activate(int reveals) {
    remainingReveals = reveals;
    activeStatus = true;
}

void Invincibility::countReveal() {
    if (!activeStatus) return;
    
    remainingReveals--;
    if (remainingReveals <= 0) {
        deactivate();
    }
}

bool Invincibility::isActive() const {
    return activeStatus && remainingReveals > 0;
}

int Invincibility::getRemainingReveals() const {
    return remainingReveals;
}

void Invincibility::deactivate() {
    activeStatus = false;
    remainingReveals = 0;
}

bool Invincibility::Protect() {
    if (!isActive()) return false;
    return true;
}
