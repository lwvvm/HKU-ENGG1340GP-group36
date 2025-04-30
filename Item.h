#ifndef ITEM_H
#define ITEM_H

#include <vector>

// Function declaration
void performAutoSweep(
    std::vector<std::vector<bool> > &revealed, // Revealed grid (modified)
    const std::vector<std::vector<bool> > &mineGrid, // Mine positions (read-only)
    int rows,  
    int cols                           
);

#endif
