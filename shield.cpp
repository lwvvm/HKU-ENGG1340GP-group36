#include "shield.h"
#include <ctime>
#include <iostream>

Shield::Shield() : activeStatus(false), activationTime(0), durationSeconds(0) {}

void Shield::activate(int seconds) {
    activeStatus = true;
    activationTime = time(nullptr);
    durationSeconds = seconds;
    std::cout << "\033[1;34mShield activated! You're protected for " 
              << durationSeconds << " seconds.\033[0m\n";
// }
    // Add debug output here:
    std::cout << "Shield status: " << (activeStatus ? "Active" : "Inactive") 
              << ", Remaining: " << getRemainingTime() << "s\n";}

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
