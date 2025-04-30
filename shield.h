#ifndef SHIELD_H
#define SHIELD_H

#include <ctime>

class Shield {
private:
    bool activeStatus;  // Changed from isActive to activeStatus
    time_t activationTime;
    int durationSeconds;
    
public:
    Shield();
    
    void activate(int seconds);
    void deactivate();
    bool Protect();
    bool isActive() const;  // Keep this method name
    int getRemainingTime() const;
};

#endif
