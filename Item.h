#ifndef ITEM_H
#define ITEM_H

#include <vector>
#include <ctime>
#include <iostream>

class Item {
private:
    // AutoSweep doesn't need state, just functions
    
    // Invincibility state
    int remainingReveals;
    bool invincibilityActive;
    
    // Shield state
    bool shieldActive;
    time_t shieldActivationTime;
    int shieldDurationSeconds;

public:
    Item();
    
    // AutoSweep functionality
    static void performAutoSweep(
        std::vector<std::vector<bool>>& revealed,
        const std::vector<std::vector<bool>>& mineGrid,
        int rows,
        int cols
    );
    
    // Invincibility methods
    void activateInvincibility(int reveals);
    void countInvincibilityReveal();
    bool isInvincible() const;
    int getRemainingInvincibilityReveals() const;
    void deactivateInvincibility();
    
    // Shield methods
    void activateShield(int seconds);
    void deactivateShield();
    bool isShieldActive() const;
    int getShieldRemainingTime() const;
    
    // Combined protection check
    bool isProtected() const;
};

#endif
