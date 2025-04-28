#ifndef DETECTITEM_H
#define DETECTITEM_H

#include <vector>

// Function declaration
void useMineRevealer(
    int &totalScore,                   // Total score (modified)
    std::vector<std::vector<bool>> &revealed, // Revealed grid (modified)
    const std::vector<std::vector<bool>> &mineGrid, // Mine positions (read-only)
    int rows,                          // Number of rows
    int cols                           // Number of columns
);

#endif

// for temporary inplementing the function into main.cpp: 1. #include "detectItem.h" 2. integrate the following lines under void play() in main.cpp
/*

Corret the original:

if (cmd.size() == 1 && (cmd[0] == 'r' || cmd[0] == 'f' || cmd[0] == 'q' || cmd[0] == '2')) {
                    break;
                } else {
                    cout << "Invalid command! Please enter ONLY 'r', 'f', '2', or 'q'.\n";
                }

Adding:

if (cmd[0] == '2') {
    useMineRevealer(totalScore, revealed, mineGrid, rows, cols);
    if (checkWin()) {
        endTime = time(nullptr);
        gameWon = true;
        totalScore += score;
        cout << "\033[1;32mCongratulations! You won!\033[0m\n";
        cout << "\033[1;32mYou earned " << score << " points!\033[0m\n";
        cout << "\033[1;32mTime spent: " << difftime(endTime, startTime) << " seconds.\033[0m\n";
        printBoard(true);
        return;
    }
    continue;
}

*/