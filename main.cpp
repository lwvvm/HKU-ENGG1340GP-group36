#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <string>
#include <limits>
#include <fstream>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include "shop_menu.h"
#include "detectItem.h"
//#include "quiz_challenge.h"

using namespace std;

class Minesweeper {
private:
    int rows;
    int cols;
    int mines;
    int score;
    int totalScore;
    int TemporaryInvincibility;
    int AutoSweep;
    int MineScanner;
    int ExtraHint;
    vector<vector<bool>> mineGrid;
    vector<vector<bool>> revealed;
    vector<vector<bool>> flagged;
    bool gameOver;
    bool gameWon;

    time_t startTime;
    time_t endTime;

    void initializeGrid() {
        mineGrid.assign(rows, vector<bool>(cols, false));
        revealed.assign(rows, vector<bool>(cols, false));
        flagged.assign(rows, vector<bool>(cols, false));
        gameOver = false;
        startTime = time(nullptr);
        gameWon = false;

        // Place mines randomly
        int minesPlaced = 0;
        while (minesPlaced < mines) {
            int r = rand() % rows;
            int c = rand() % cols;
            if (!mineGrid[r][c]) {
                mineGrid[r][c] = true;
                minesPlaced++;
            }
        }
    }

    bool isValid(int r, int c) const {
        return r >= 0 && r < rows && c >= 0 && c < cols;
    }

    int countAdjacentMines(int r, int c) const {
        int count = 0;
        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                if (dr == 0 && dc == 0) continue;
                int nr = r + dr;
                int nc = c + dc;
                if (isValid(nr, nc) && mineGrid[nr][nc]) {
                    count++;
                }
            }
        }
        return count;
    }

    void revealCell(int r, int c) {
        if (!isValid(r, c) || revealed[r][c] || flagged[r][c]) {
            return;
        }

        revealed[r][c] = true;

        if (mineGrid[r][c]) {
            gameOver = true;
            return;
        }

        if (countAdjacentMines(r, c) == 0) {
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    revealCell(r + dr, c + dc);
                }
            }
        }
    }

    bool checkWin() const {
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (!mineGrid[r][c] && !revealed[r][c]) {
                    return false;
                }
            }
        }
        return true;
    }

    void printHorizontalLine() const {
        cout << "   +";
        for (int c = 0; c < cols; ++c) {
            cout << "---+";
        }
        cout << "\n";
    }

    string getMachineName() {
        string name;
        
        #ifdef _WIN32
        char computerName[MAX_COMPUTERNAME_LENGTH + 1];
        DWORD size = sizeof(computerName);
        GetComputerNameA(computerName, &size);
        name = "win_" + std::string(computerName);
        
        #else
        char hostname[256];
        gethostname(hostname, sizeof(hostname));
        name = "mac_" + std::string(hostname);
        #endif
        
        return name;
    }

    string scoreFile = "minesweeper_score_" + getMachineName() + ".txt";

    void saveScore() {
        std::ofstream file(scoreFile);
        if (file) {
            file << totalScore << "\n";
            file << rows << " " << cols << " " << mines << "\n";
            for (int r = 0; r < rows; ++r) {
                for (int c = 0; c < cols; ++c) {
                    file << mineGrid[r][c] << " ";
                }
                file << "\n";
            }
        }
    }

    void loadScore() {
        std::ifstream file(scoreFile);
        if (file) {
            file >> totalScore;
            if (file >> rows >> cols >> mines) {
                mineGrid.assign(rows, vector<bool>(cols, false));
                revealed.assign(rows, vector<bool>(cols, false));
                flagged.assign(rows, vector<bool>(cols, false));
                for (int r = 0; r < rows; ++r) {
                    for (int c = 0; c < cols; ++c) {
                        int val;
                        if (file >> val) {
                            mineGrid[r][c] = val;
                        }
                    }
                }
            }
        } else {
            totalScore = 0;
        }
    }    


public:
    Minesweeper() : rows(9), cols(9), mines(10), score(0), totalScore(0), gameOver(false), gameWon(false),
                    TemporaryInvincibility(0), AutoSweep(0), MineScanner(0), ExtraHint(0) {
        srand(time(0));
        initializeGrid();
        loadScore();
        loadItems(TemporaryInvincibility, AutoSweep, MineScanner, ExtraHint);
    }

    ~Minesweeper() {
        saveScore();
        saveItems(TemporaryInvincibility, AutoSweep, MineScanner, ExtraHint);
    }

    void setDifficulty(int level) {
        switch (level) {
            case 1: // Easy
                rows = 9;
                cols = 9;
                mines = 10;
                score = 3;
                break;
            case 2: // Medium
                rows = 9;
                cols = 9;
                mines = 20;
                score = 5;
                break;
            case 3: // Hard
                rows = 12;
                cols = 12;
                mines = 45;
                score = 8;
                break;
            case 4: // Expert
                rows = 12;
                cols = 12;
                mines = 60;
                score = 10;
                break;
            default:
                break;
        }
        initializeGrid();
    }

    void printBoard(bool showMines = false) const {
        // Print column numbers
        cout << "    ";
        for (int c = 0; c < cols; ++c) {
            cout << setw(3) << c << " ";
        }
        cout << "\n";

        printHorizontalLine();

        for (int r = 0; r < rows; ++r) {
            cout << setw(2) << r << " |";
            for (int c = 0; c < cols; ++c) {
                if (gameOver && mineGrid[r][c] && showMines) {
                    cout << " * |";
                } else if (flagged[r][c]) {
                    cout << " \033[1;31mF\033[0m |";
                } else if (!revealed[r][c]) {
                    cout << " . |";
                } else if (mineGrid[r][c]) {
                    cout << " * |";
                } else {
                    int count = countAdjacentMines(r, c);
                    if (count > 0) {
                        string color;
                        switch (count) {
                            case 1: color = "\033[1;34m"; break; // blue
                            case 2: color = "\033[1;32m"; break; // green
                            case 3: color = "\033[1;31m"; break; // red
                            case 4: color = "\033[1;35m"; break; // purple
                            case 5: color = "\033[1;33m"; break; // yellow
                            case 6: color = "\033[1;36m"; break; // cyan
                            case 7: color = "\033[1;37m"; break; // white
                            case 8: color = "\033[1;90m"; break; // grey
                            default: color = "\033[0m"; break;
                        }
                        cout << " " << color << count << "\033[0m |"; 
                    } else {
                        cout << "   |";
                    }

                }
            }
            cout << "\n";
            printHorizontalLine();
        }
    }

    void play() {
        while (!gameOver && !gameWon) {
            printBoard();
            cout << "Total Score: " << totalScore << "\n";
            cout << "Current game score: " << score << "\n";
    
            string cmd;
            while (true) {
                cout << "Enter command (r for reveal, f for flag/unflag, s for shop, q to quit): ";
                getline(cin, cmd);
                    
                cmd.erase(0, cmd.find_first_not_of(" \t"));
                cmd.erase(cmd.find_last_not_of(" \t") + 1);
    
                if (cmd.empty()) {
                    continue;
                }
    
                if (cmd.size() == 1 && (cmd[0] == 'r' || cmd[0] == 'f' || cmd[0] == 'q' || cmd[0] == 's')) {
                    break;
                } else {
                    cout << "\033[1;32mInvalid command! Please enter ONLY 'r', 'f', 's' or 'q'.\033[0m\n";
                }
            }
    
            if (cmd[0] == 'q') {
                return;
            }
            
            if (cmd[0] == 's') {
                shop_menu(totalScore, TemporaryInvincibility, AutoSweep, MineScanner, ExtraHint);
                continue;
            }
    
            int r = -1, c = -1;
            while (true) {
                cout << "Enter row and column (e.g. '3 5'): ";
                string coordInput;
                getline(cin, coordInput);
    
                stringstream ss(coordInput);
                if (!(ss >> r >> c)) {
                    cout << "\033[1;32mInvalid input! Please enter TWO numbers separated by space.\033[0m\n";
                    continue;
                }
    
                string remaining;
                if (ss >> remaining) {
                    cout << "\033[1;32mInvalid input! Only two numbers allowed (e.g. '3 5').\033[0m\n";
                    continue;
                }
    
                if (!isValid(r, c)) {
                    cout << "\033[1;32mInvalid position! Row  and column must be 0-" << (rows-1) << ".\033[0m\n";
                    continue;
                }
    
                break;
            }
    
            if (cmd[0] == 'r') {
                if (flagged[r][c]) {
                    cout << "\033[1;32mCell is flagged. Unflag it first.\033[0m\n";
                    continue;
                }
                revealCell(r, c);
                if (gameOver) {
                    endTime = time(nullptr);
                    cout << "\033[1;31mGame Over! You hit a mine.\033[0m\n";
                    cout << "\033[1;31mTime spent: " << difftime(endTime, startTime) << " seconds.\033[0m\n";
                    printBoard(true);
                    return;
                }
            } 
            else if (cmd[0] == 'f') {
                if (!revealed[r][c]) {
                    flagged[r][c] = !flagged[r][c];
                } else {
                    cout << "\033[1;32mCannot flag a revealed cell.\033[0m\n";
                }
            }
    
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
        }
    }

    void showMenu() {
        loadScore(); 
        while (true) {
            cout << "\n=== Minesweeper ===\n";
            cout << "Total Score: " << totalScore << "\n";
            cout << "1. New Game\n";
            cout << "2. Shop Menu\n";
            cout << "3. Challenge Quiz" << endl;
            cout << "4. Quit\n";
    
            while (true) {
                cout << "Select your choice (1-4): ";
                string input;
                getline(cin, input);
    
                if (input.empty()) {
                    continue;
                }
    
                bool is_valid = true;
                for (char c : input) {
                    if (!isdigit(c)) {
                        is_valid = false;
                        break;
                    }
                }
                if (!is_valid) {
                    cout << "\033[1;32mInvalid input! Please enter a number between 1 and 4.\033[0m\n";
                    continue;
                }
    
                int choice = stoi(input);
                if (choice == 4) {
                    return;// Quit the game
                } 
                else if (choice == 1){
                    // Start a new game
                    cout << "\nSelect difficulty level:\n";
                    cout << "1. Easy (9x9, 10 mines) - 3 points\n";
                    cout << "2. Medium (9x9, 20 mines) - 5 points\n";
                    cout << "3. Hard (12x12, 45 mines) - 8 points\n";
                    cout << "4. Expert (12x12, 60 mines) - 10 points\n";
                    cout << "5. Quit\n";

                    while (true) {
                        cout << "Enter difficulty level (1-5): ";
                        string levelInput;
                        getline(cin, levelInput);
    
                        if (levelInput.empty()) {
                            continue;
                        }
    
                        bool level_valid = true;
                        for (char c : levelInput) {
                            if (!isdigit(c)) {
                                level_valid = false;
                                break;
                            }
                        }
                        if (!level_valid) {
                            cout << "\033[1;32mInvalid input! Please enter a number between 1 and 4.\033[0m\n";
                            continue;
                        }

                        int level = stoi(levelInput);
                        if (level == 5) {
                            break; // Quit the game
                        } 
                        
                        else if (level >= 1 && level <= 4) {
                            setDifficulty(level);
                            play();
                            saveScore();
                            break;
                        } else {
                            cout << "\033[1;32mInvalid choice! Please enter a number between 1 and 4.\033[0m\n";
                        }
                    }
                    break;
                }

                else if (choice == 2) {
                    // Open shop menu
                    shop_menu(totalScore, TemporaryInvincibility, AutoSweep, MineScanner, ExtraHint);
                    break;
                } 
                else if (choice == 3) {
                    // Challenge Quiz
                    quizGame.showQuizChallengeMenu(totalScore);
                    break;
                }

                 else {
                    cout << "\033[1;32mInvalid choice! Please enter a number between 1 and 4.\033[0m\n";
                }
            }
        }
    }
};

int main() {
    Minesweeper game;
    game.showMenu();
    
    return 0;
}