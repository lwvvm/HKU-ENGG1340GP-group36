#include "shield.h"
#include <ctime>
#include <iostream>

Shield::Shield() : activeStatus(false), activationTime(0), durationSeconds(0) {}

void Shield::activate(int seconds) {
    activeStatus = true;
    activationTime = time(nullptr);
    durationSeconds = seconds;
}
void Shield::deactivate() {
    activeStatus = false;
    std::cout << "\033[1;34mShield has expired.\033[0m\n";
}

bool Shield::Protect() {
    if (!activeStatus) return false;
    
    time_t currentTime = time(nullptr);
    if (difftime(currentTime, activationTime) >= durationSeconds) {
        deactivate();
        return false;
    }
    return true;
}

bool Shield::isActive() const {
    return activeStatus && 
           (difftime(time(nullptr), activationTime) < durationSeconds);
}

int Shield::getRemainingTime() const {
    if (!activeStatus) return 0;
    int remaining = durationSeconds - difftime(time(nullptr), activationTime);
    return remaining > 0 ? remaining : 0;
}
