#ifndef INVINCIBILITY_H
#define INVINCIBILITY_H

class Invincibility {
private:
    int remainingMoves;  // Number of remaining protected moves
    bool isActive;       // Whether invincibility is currently active

public:
    Invincibility();
    
    // Activate invincibility with specified number of moves
    void activate(int moves);
    
    // Use one invincibility move
    void useMove();
    
    // Check if invincibility is currently active
    bool isInvincible() const;
    
    // Get remaining protected moves
    int getRemainingMoves() const;
    
    // Deactivate invincibility
    void deactivate();
};

#endif
