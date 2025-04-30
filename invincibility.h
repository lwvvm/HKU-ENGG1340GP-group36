#ifndef INVINCIBILITY_H
#define INVINCIBILITY_H

class Invincibility {
private:
    int remainingMoves;  // Number of safe moves remaining
    bool isActive;       // Whether invincibility is currently active

public:
    Invincibility();
    
    void activate(int moves);
    bool useMove();      // Returns true if invincibility was used
    bool isInvincible() const;
    int getRemainingMoves() const;
    void deactivate();
};

#endif
