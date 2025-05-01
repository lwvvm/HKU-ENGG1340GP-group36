#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <string>
#include <limits>
#include <fstream>
#include <sstream>

#include "shop_menu.h"
#include "Item.h"
#include "quiz_challenge.h"
#include "guide.h"


using namespace std;

int rows;
int cols;
int mines;
int score;
int totalScore;
double totalPlayTime;
int TemporaryInvincibility;
int AutoSweep;
int ShieldCount;
vector<vector<bool> > mineGrid;
vector<vector<bool> > revealed;
vector<vector<bool> > flagged;

bool hasActiveGame = false;

void saveGameState() {
    std::ofstream file("saved_game_state.txt");
    if (file) {
        file << rows << " " << cols << " " << mines << "\n";
        file << totalScore << "\n";
        file << TemporaryInvincibility << " " << AutoSweep << " " << ShieldCount << "\n";
        file << totalPlayTime << "\n";
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                file << mineGrid[r][c] << " ";
            }
            file << "\n";
        }
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                file << revealed[r][c] << " ";
            }
            file << "\n";
        }
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                file << flagged[r][c] << " ";
            }
            file << "\n";
        }
        file.close();
    } 
}

bool loadGameState() {
    std::ifstream file("saved_game_state.txt");
    if (!file) {
        std::cout << "\033[1;31mError: No saved game found.\033[0m\n";
        hasActiveGame = false; // No valid game state
        return false;
    }

    if (!(file >> rows >> cols >> mines)) {
        std::cout << "\033[1;31mError: Invalid saved game data.\033[0m\n";
        hasActiveGame = false; // No valid game state
        return false;
    }

    if (rows == 0 || cols == 0 || mines == 0) {
        std::cout << "\033[1;31mError: No valid game state found.\033[0m\n";
        hasActiveGame = false;
        return false;
    }

    file >> totalScore;
    file >> TemporaryInvincibility >> AutoSweep >> ShieldCount;
    file >> totalPlayTime;
    mineGrid.assign(rows, vector<bool>(cols, false));
    revealed.assign(rows, vector<bool>(cols, false));
    flagged.assign(rows, vector<bool>(cols, false));

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            int val;
            if (file >> val) {
                mineGrid[r][c] = val;
            }
            else {
                cout << "\033[1;31mError: Failed to load mineGrid.\033[0m\n";
                return false;
            }
        }
    }
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            int val;
            if (file >> val) {
                revealed[r][c] = val;
            }
            else {
                cout << "\033[1;31mError: Failed to load mineGrid.\033[0m\n";
                return false;
            }
        }
    }
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            int val;
            if (file >> val) {
                flagged[r][c] = val;
            }
            else {
                cout << "\033[1;31mError: Failed to load mineGrid.\033[0m\n";
                return false;
            }
        }
    }
    file.close();
    hasActiveGame = true;
    return true;
}


class Minesweeper {
private:
    bool gameWon;
    bool gameOver;

    Item gameItem;
    QuizChallenge quizChallenge;

    void initializeGrid() {
        mineGrid.assign(rows, vector<bool>(cols, false));
        revealed.assign(rows, vector<bool>(cols, false));
        flagged.assign(rows, vector<bool>(cols, false));
        gameOver = false;
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
            if (gameItem.isProtected()) {
                if (gameItem.isShieldActive()) {
                    cout << "\033[1;34mShield protected you!\033[0m\n";
                    gameItem.deactivateShield();
                } else if (gameItem.isInvincible()) {
                    cout << "\033[1;33mInvincibility protected you!\033[0m\n";
                    gameItem.countInvincibilityReveal();
                }
                return;
            } 
            else {
                gameOver = true;
            }
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
  


public:
    Minesweeper() : gameOver(false), gameWon(false) {
        srand(time(0));
        if (!loadGameState()) { 
            initializeGrid();
        }
    }

    ~Minesweeper() {
        saveGameState();
    }

    void setDifficulty(int level) {
        totalPlayTime = 0; // Reset play time for new game
        invincibility.deactivate(); // Reset invincibility for new game
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
        hasActiveGame = true; // Set to true when a new game is started
    }

    void printBoard(bool showMines = false) const {
        //invincibility notice
        if (gameItem.isInvincible()) {
            cout << "\033[1;33mInvincibility: " << gameItem.getRemainingInvincibilityReveals() 
                 << " moves remaining.\033[0m\n";
        }
        //shield notice
        if (gameItem.isShieldActive()) {
            cout << "\033[1;34m[Shield: " << gameItem.getShieldRemainingTime() 
                 << " seconds remaining]\033[0m\n";
        }
        // Print column numbers
        cout << "   ";
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

    int play() {
        time_t sessionStartTime = time(nullptr);

        while (!gameOver && !gameWon) {
            printBoard();
            cout << "Total Score: " << totalScore << "\n";
            cout << "Current game score: " << score << "\n";
    
            string cmd;
            while (true) {
                cout << "Enter command (r for reveal, f for flag/unflag, i for items, s for shop, q to save and quit): ";
                getline(cin, cmd);
                
    
                if (cmd.empty()) {
                    continue;
                }
    
                if (cmd.size() == 1 && (cmd[0] == 'r' || cmd[0] == 'f' || cmd[0] == 'q' || cmd[0] == 's' || cmd[0] == 'i')) {
                    break;
                } else {
                    cout << "\033[1;32mInvalid command! Please enter ONLY 'r', 'f', 's' or 'q'.\033[0m\n";
                }
            }
    
            if (cmd[0] == 'q') {
                time_t sessionEndTime = time(nullptr);
                totalPlayTime += difftime(sessionEndTime, sessionStartTime);

                saveGameState();
                cout << "\033[1;32mGame progress saved successfully.\033[0m\n";
                cout << "\033[1;32mYou can continue next time!\033[0m\n";
                return 0;//Quit the game
            }
            
            if (cmd[0] == 's') {
                shop_menu(totalScore, TemporaryInvincibility, AutoSweep, ShieldCount);
                continue;
            }

            if (cmd[0] == 'i') {
                while (true) {
                    cout << "\n=== Items Menu ===\n";
                    cout << "1. Use Temporary Invincibility (" << TemporaryInvincibility << " available)\n";
                    cout << "2. Use Auto Sweep (" << AutoSweep << " available)\n";
                    cout << "3. Use Shield (" << ShieldCount << " available)\n";
                    cout << "4. Back to Game\n";
                    cout << "Select an option (1-4): ";
                
                    string itemInput;
                    getline(cin, itemInput);

                    if (itemInput.empty()) {
                        cout << "\033[1;32mInvalid input! Please enter a number between 1 and 4.\033[0m\n";
                        continue;
                    }

                    bool is_valid = true;
                    for (char c : itemInput) {
                        if (!isdigit(c)) {
                            is_valid = false;
                            break;
                        }
                    }
                    if (!is_valid) {
                        cout << "\033[1;32mInvalid input! Please enter a number between 1 and 4.\033[0m\n";
                        continue;
                    } 

                    int itemChoice = stoi(itemInput);
                    if (itemChoice == 4) {
                        break; // Back to game
                    }

                    switch(itemChoice){
                        case 1: // Temporary Invincibility
                        if (TemporaryInvincibility > 0) {
                            TemporaryInvincibility--;
                            saveGameState();
                            gameItem.activateInvincibility(3);
                            cout << "Invincibility activated! "<< invincibility.getRemainingReveals()<< "moves protected.\n";
                        } else {
                                cout << "\033[1;31mYou don't have any Invincibility left!\033[0m\n";
                            }
                            break;

                        case 2: // Auto Sweep
                            if (AutoSweep > 0) {
                                AutoSweep--;
                                saveGameState(); // update file
                                cout << "\033[1;32mAuto Sweep activated! All adjacent cells are revealed.\033[0m\n";
                                Item::performAutoSweep(revealed, mineGrid, rows, cols);
                                break;
                            } else {
                                cout << "\033[1;31mYou don't have any AutoSweep left!\033[0m\n";
                            }
                            break;

                        case 3: // Shield
                        if (ShieldCount > 0) {  
                            ShieldCount--;
                            gameItem.activateShield(30);  // 30 seconds protection
                            saveGameState();
                        }else {
                                cout << "\033[1;31mYou don't have any Invincibility left!\033[0m\n";
                            }
                            break;    

                        default:
                            cout << "\033[1;32mInvalid choice! Please enter a number between 1 and 4.\033[0m\n";
                            break;
                    }
                    break; // Exit the item menu after one action
                }
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
                gameItem.countInvincibilityReveal();
                
                revealCell(r, c);
                if (gameOver) {
                    time_t sessionEndTime = time(nullptr);
                    totalPlayTime += difftime(sessionEndTime, sessionStartTime); // game time

                    cout << "\033[1;31mGame Over! You hit a mine.\033[0m\n";
                    cout << "\033[1;31mTime spent: " << totalPlayTime << " seconds.\033[0m\n";
                    printBoard(true);
                    return 1;// Game over, return to difficulty selection
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
                time_t sessionEndTime = time(nullptr);
                totalPlayTime += difftime(sessionEndTime, sessionStartTime); // game time
                
                
                cout << "\033[1;31mCongratulations! You won!\033[0m\n";
                cout << "\033[1;31mTime spent: " << totalPlayTime << " seconds.\033[0m\n";

                // Check for difficulty-specific bonuses
                if (rows == 9 && cols == 9 && mines == 10 && totalPlayTime < 300) {
                    score += 4; // Easy 
                    cout << "\033[1;33mBonus! You completed the game in less than 300 seconds. Your score is doubled!\033[0m\n";
                } else if (rows == 9 && cols == 9 && mines == 20 && totalPlayTime < 600) {
                    score += 5; // Medium
                    cout << "\033[1;33mBonus! You completed the game in less than 600 seconds. You earned 10 extra points!\033[0m\n";
                } else if (rows == 12 && cols == 12 && mines == 45 && totalPlayTime < 1200) {
                    score += 8; // Hard 
                    cout << "\033[1;33mBonus! You completed the game in less than 1200 seconds. You earned 16 extra points!\033[0m\n";
                } else if (rows == 12 && cols == 12 && mines == 60 && totalPlayTime < 2000) {
                    score += 10; // Expert 
                    cout << "\033[1;33mBonus! You completed the game in less than 2000 seconds. You earned 20 extra points!\033[0m\n";
                }
    
                totalScore += score;
                cout << "\033[1;31mYou earned " << score << " points!\033[0m\n";

                gameWon = true; 
                
                printBoard(true);
                return 1; // Game won, return to difficulty selection
            }
        }
        return 0;
    };
void showMenu() {
    while (true) {
        cout << "\n=== Minesweeper ===\n";
        cout << "Total Score: " << totalScore << "\n";
        cout << "1. New Game\n";
        cout << "2. Continue Last Game\n";
        cout << "3. Shop Menu\n";
        cout << "4. Challenge Quiz" << endl;
        cout << "5. Gameplay Introduction\n"; 
        cout << "q. Quit\n";

        cout << "Select your choice (1-5 or q to quit): ";
        string input;
        getline(cin, input);

        if (input.empty()) {
            cout << "\033[1;32mInvalid input! Please enter a number between 1 and 5 or 'q' to quit.\033[0m\n";
            continue;
        }

        if (input == "q") {
            return; // Quit the game
        }

        bool is_valid = true;
        for (char c : input) {
            if (!isdigit(c)) {
                is_valid = false;
                break;
            }
        }

        if (!is_valid) {
            cout << "\033[1;32mInvalid input! Please enter a number between 1 and 5 or 'q' to quit.\033[0m\n";
            continue;
        }

        int choice = stoi(input);
        if (choice < 1 || choice > 5) {
            cout << "\033[1;32mInvalid input! Please enter a number between 1 and 5 or 'q' to quit.\033[0m\n";
            continue;
        }
         
            else if (choice == 2) {
                // Continue last game
                if (hasActiveGame && loadGameState()){
                    if (gameOver){
                        cout << "\033[1;31mYou cannot continue this game. You already lost! Please start a new game.\033[0m\n";
                    } else if (gameWon) {
                        cout << "\033[1;32mYou cannot continue this game. You already won! Please start a new game.\033[0m\n";
                    }
                    else{
                        play();
                    }
                }
                else {
                    cout << "\033[1;31mNo saved game found. Please start a new game.\033[0m\n";
                }
            }
        
            else if (choice == 1){
                // Start a new game
                while (true) {    
                    cout << "\nSelect difficulty level:\n";
                    cout << "1. Easy (9x9, 10 mines) - 3 points\n";
                    cout << "2. Medium (9x9, 20 mines) - 5 points\n";
                    cout << "3. Hard (12x12, 45 mines) - 8 points\n";
                    cout << "4. Expert (12x12, 60 mines) - 10 points\n";
                    cout << "q. Quit\n";  
                                
                    cout << "Enter difficulty level (1-4): ";
                    string levelInput;
                    getline(cin, levelInput);

                    if (levelInput.empty()) {
                        cout << "\033[1;32mInvalid input! Please enter a number between 1 and 4.\033[0m\n";
                        continue;
                    }
                    if (levelInput == "q") {
                        break; // Quit the game
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
                    if (level >= 1 && level <= 4) {
                        setDifficulty(level);
                        int result = play();
                        saveGameState();
                        if (result == 1) {
                            continue; // Return to difficulty selection
                        }
                        break;
                    } else {
                        cout << "\033[1;32mInvalid choice! Please enter a number between 1 and 4.\033[0m\n";
                    }
                }   
            } 

            else if (choice == 3) {
                // Open shop menu
                shop_menu(totalScore, TemporaryInvincibility, AutoSweep, ShieldCount);
                continue;
            }

            else if (choice == 4) {
                quizChallenge.showQuizChallengeMenu(totalScore);
            }
            else if (choice == 5) {
                // Gameplay Introduction
                guide();
            }
                else {
                cout << "\033[1;32mInvalid choice! Please enter a number between 1 and 4.\033[0m\n";
            }
        }
    }
};

int main() {
    Minesweeper game;
    game.showMenu();
    
    return 0;
}
