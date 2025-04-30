#ifndef INVINCIBILITY_H
#define INVINCIBILITY_H

class Invincibility {
private:
    int remainingReveals;
    bool activeStatus;
    
public:
    Invincibility();
    
    void activate(int reveals);
    void countReveal();
    bool isActive() const;
    int getRemainingReveals() const;
    void deactivate();
    bool Protect();  
};

#endif
