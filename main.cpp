#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <string>
#include <limits>
#include <fstream>
#include <sstream>
#include <unistd.h>

#include "shop_menu.h"
#include "Item.h"
#include "quiz_challenge.h"
#include "guide.h"
#include "cover_display.h"

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
bool gameOver = false;
bool gameWon = false;

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
        }// save the mine grid
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                file << revealed[r][c] << " ";
            }
            file << "\n";
        }// save the revealed grid
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                file << flagged[r][c] << " ";
            }
            file << "\n";
        }// save the flagged grid
        file.close();
    } 
}// save game state to check the statement of the game

bool loadGameState() {
    std::ifstream file("saved_game_state.txt");
    if (!file) {
        hasActiveGame = false; // No valid game state
        return false;
    }// check if the file is opened

    else if (!(file >> rows >> cols >> mines)) {
        std::cout << "\033[1;31mError: Invalid saved game data.\033[0m\n";
        hasActiveGame = false;
        return false;
    }// check if the file has valid data

    else if (rows == 0 || cols == 0 || mines == 0) {
        hasActiveGame = false;
        return false;
    }// check if the file has valid data

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
    }// load the mine grid
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
    }// load the revealed grid
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
    }// load the flagged grid
    file.close();
    gameOver = false;
    gameWon = true;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (revealed[r][c] && mineGrid[r][c]) {
                gameOver = true;
                gameWon = false;
                break;
            }
            if (!revealed[r][c] && !mineGrid[r][c]) {
                gameWon = false;
            }
        }
        if (gameOver) break;
    }// check if the game is over or won

    hasActiveGame = !(gameOver || gameWon);
    return true;
}// load game state to check the statement of the game

class Minesweeper {
private:
    bool gameWon;
    bool gameOver;
    mutable std::string gameMessage;

    Item gameItem;
    QuizChallenge quizChallenge;

    void initializeGrid() {
        mineGrid.assign(rows, vector<bool>(cols, false));
        revealed.assign(rows, vector<bool>(cols, false));
        flagged.assign(rows, vector<bool>(cols, false));
        gameOver = false;
        gameWon = false;

        int minesPlaced = 0;
        while (minesPlaced < mines) {
            int r = rand() % rows;
            int c = rand() % cols;
            if (!mineGrid[r][c]) {
                mineGrid[r][c] = true;
                minesPlaced++;
            }
        }
    }// initialize the mine grid

    bool isValid(int r, int c) const {
        return r >= 0 && r < rows && c >= 0 && c < cols;
    }// check if the cell is valid

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
    }// count the number of adjacent mines

    void revealCell(int r, int c) {
        if (!isValid(r, c) || revealed[r][c] || flagged[r][c]) {
            return;
        }

        revealed[r][c] = true;

        if (mineGrid[r][c]) {
            if (gameItem.isProtected()) {
                if (gameItem.isShieldActive()) {
                    setGameMessage("\033[1;34mShield protected you!\033[0m");
                    gameItem.deactivateShield();
                } else if (gameItem.isInvincible()) {
                    setGameMessage("\033[1;33mInvincibility protected you!\033[0m");
                }
                return;
            } // protect the player from the mine
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
    }// check if the game is won

    void printHorizontalLine() const {
        cout << "   +";
        for (int c = 0; c < cols; ++c) {
            cout << "---+";
        }
        cout << "\n";
    }// print the horizontal line
    
    void clearScreen() const {
        #ifdef _WIN32
                system("cls");
        #else
                system("clear");
        #endif
    }// clear the screen

public:
    Minesweeper() : gameOver(false), gameWon(false) {
        srand(time(0));
        if (!loadGameState()) { 
            if (gameOver) {
                hasActiveGame = false; 
            }
        } else {
            initializeGrid();
        }
    }

    ~Minesweeper() {
        saveGameState();
    }
    
    void setGameMessage(const string& message) {
        gameMessage = message;
    }
    
    void clearGameMessage() {
        gameMessage.clear();
    }
    
    void setDifficulty(int level) {
        totalPlayTime = 0;
        gameItem.deactivateInvincibility();
        switch (level) {
            case 1:// Easy
                rows = 9;
                cols = 9;
                mines = 10;
                score = 3;
                break;
            case 2:// Medium
                rows = 9;
                cols = 9;
                mines = 20;
                score = 5;
                break;
            case 3:// Hard
                rows = 12;
                cols = 12;
                mines = 45;
                score = 8;
                break;
            case 4:// Expert
                rows = 12;
                cols = 12;
                mines = 60;
                score = 10;
                break;
            default:
                break;
        }
        initializeGrid();
        hasActiveGame = true;
    }

    void printBoard(bool showMines = false) const {
        clearScreen();
        if (gameItem.isInvincible()) {
            cout << "\033[1;32mInvincibility: " << gameItem.getRemainingInvincibilityReveals() - 1
                 << " moves remaining.\033[0m\n";
        }
        if (gameItem.isShieldActive()) {
            cout << "\033[1;32m[Shield: " << gameItem.getShieldRemainingTime() 
                 << " seconds remaining]\033[0m\n";
        }
        
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
                            case 1: color = "\033[1;34m"; break;// Blue
                            case 2: color = "\033[1;32m"; break;// Green
                            case 3: color = "\033[1;31m"; break;// Red
                            case 4: color = "\033[1;35m"; break;// Magenta
                            case 5: color = "\033[1;33m"; break;// Yellow
                            case 6: color = "\033[1;36m"; break;// Cyan
                            case 7: color = "\033[1;37m"; break;// White
                            case 8: color = "\033[1;90m"; break;// Gray
                            default: color = "\033[0m"; break;// Default
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

        if (!gameMessage.empty()) {
            cout << gameMessage << endl;
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
                cout << "Enter command (r for reveal, f for flag/unflag, i for items, q for save and quit): ";
                getline(cin, cmd);
                
                if (cmd.empty()) {
                    continue;
                }
    
                if (cmd.size() == 1 && (cmd[0] == 'r' || cmd[0] == 'f' || cmd[0] == 'q' || cmd[0] == 'i')) {
                    break;
                } else {
                    cout << "\033[1;31mInvalid command! Please enter ONLY 'r', 'f', 'i' or 'q'.\033[0m\n";
                }
            }
    
            if (cmd[0] == 'q') {
                time_t sessionEndTime = time(nullptr);
                totalPlayTime += difftime(sessionEndTime, sessionStartTime);

                saveGameState();
                setGameMessage("\033[1;32mGame progress saved successfully. You can continue next time!\033[0m");
                printBoard();
                return 0;
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
                        cout << "\033[1;31mInvalid input! Please enter a number between 1 and 4.\033[0m\n";
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
                        cout <<"\033[1;31mInvalid input! Please enter a number between 1 and 4.\033[0m\n";
                        continue;
                    } 

                    int itemChoice = stoi(itemInput);
                    if (itemChoice == 4) {
                        break;
                    }

                    switch(itemChoice){
                        case 1:
                            if (TemporaryInvincibility > 0) {
                                TemporaryInvincibility--;
                                saveGameState();
                                gameItem.activateInvincibility(4);
                                setGameMessage("\033[1;32mTemporary Invincibility activated for 3 moves!\033[0m");
                            } else {
                                setGameMessage("\033[1;31mYou don't have any Invincibility left!\033[0m");
                            }
                            break;

                        case 2:
                            if (AutoSweep > 0) {
                                AutoSweep--;
                                saveGameState();
                                setGameMessage("\033[1;32mAuto Sweep activated! All adjacent cells are revealed.\033[0m");
                                Item::performAutoSweep(revealed, mineGrid, rows, cols);
                            } else {
                                setGameMessage("\033[1;31mYou don't have any AutoSweep left!\033[0m");
                            }
                            break;

                        case 3:
                            if (ShieldCount > 0) {  
                                ShieldCount--;
                                gameItem.activateShield(30);
                                saveGameState();
                                setGameMessage("\033[1;32mShield activated for 30 seconds!\033[0m");
                            } else {
                                setGameMessage("\033[1;31mYou don't have any Shields left!\033[0m");
                            }
                            break;    

                        default:
                            setGameMessage("\033[1;31mInvalid choice! Please enter a number between 1 and 4.\033[0m");
                            break;
                    }
                    break;
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
                    cout << "\033[1;31mInvalid input! Please enter TWO numbers separated by space.\033[0m\n";
                    continue;
                }
    
                string remaining;
                if (ss >> remaining) {
                    cout << "\033[1;31mInvalid input! Only two numbers allowed (e.g. '3 5').\033[0m\n";
                    continue;
                }
    
                if (!isValid(r, c)) {
                    cout << "\033[1;31mInvalid position! Row and column must be 0-" + std::to_string(rows-1) + ".\033[0m\n";
                    continue;
                }
    
                break;
            }
    
            if (cmd[0] == 'r') {
                if (flagged[r][c]) {
                    setGameMessage("\033[1;31mCell is flagged. Unflag it first.\033[0m");
                    continue;
                }
                else{
                    setGameMessage("\033[1;32mCell revealed successfully!\033[0m");
                }
                gameItem.countInvincibilityReveal();
                
                revealCell(r, c);
                if (gameOver) {
                    time_t sessionEndTime = time(nullptr);
                    totalPlayTime += difftime(sessionEndTime, sessionStartTime);
                    
                    printBoard(true);
                    cout << "\033[1;31mGame Over! You hit a mine.\033[0m\n";
                    cout << "\033[1;31mTime spent: " << totalPlayTime << " seconds.\033[0m\n";
                    
                    hasActiveGame = false; 
                    saveGameState();
                    return 1;
                }
            }
            else if (cmd[0] == 'f') {
                if (!revealed[r][c]) {
                    flagged[r][c] = !flagged[r][c];
                    setGameMessage(flagged[r][c] ? "\033[1;32mCell flagged successfully!\033[0m" : "\033[1;32mFlag removed successfully!\033[0m");
                } else {
                    setGameMessage("\033[1;31mCannot flag a revealed cell.\033[0m");
                }
            }
    
            if (checkWin()) {
                time_t sessionEndTime = time(nullptr);
                totalPlayTime += difftime(sessionEndTime, sessionStartTime);
                
                string bonusMessage = "";
                if (rows == 9 && cols == 9 && mines == 10 && totalPlayTime < 300) {
                    score += 3;
                    bonusMessage = "\033[1;33mBonus! You completed the game in less than 300 seconds. Your score is doubled!\033[0m\n";
                } else if (rows == 9 && cols == 9 && mines == 20 && totalPlayTime < 600) {
                    score += 5;
                    bonusMessage = "\033[1;33mBonus! You completed the game in less than 600 seconds. You earned 10 extra points!\033[0m\n";
                } else if (rows == 12 && cols == 12 && mines == 45 && totalPlayTime < 1200) {
                    score += 8;
                    bonusMessage = "\033[1;33mBonus! You completed the game in less than 1200 seconds. You earned 16 extra points!\033[0m\n";
                } else if (rows == 12 && cols == 12 && mines == 60 && totalPlayTime < 2000) {
                    score += 10;
                    bonusMessage = "\033[1;33mBonus! You completed the game in less than 2000 seconds. You earned 20 extra points!\033[0m\n";
                }
    
                totalScore += score;
                cout << "\033[1;32mCongratulations! You won!\nYou earned " + to_string(score) + " points!\033[0m\n" + bonusMessage;

                printBoard(true);
                hasActiveGame = false;
                saveGameState();
                
                // Wait for user to see the message
                cout << "Press Enter to continue...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                return 1;
            }
        }
        return 0;
    }

    void showMenu() {
        while (true) {
            cout << "\n=== Minesweeper ===\n";
            cout << "Total Score: " << totalScore << "\n";
            cout << "1. New Game\n";
            cout << "2. Continue Last Game\n";
            cout << "3. Shop Menu\n";
            cout << "4. Challenge Quiz\n"; 
            cout << "5. Gameplay Introduction\n"; 
            cout << "q. Quit\n";

            cout << "Select your choice (1-5 or q to quit): ";
            string input;
            getline(cin, input);

            if (input.empty()) {
                cout << "\033[1;31mInvalid input! Please enter a number between 1 and 5 or 'q' to quit.\033[0m\n";
                continue;
            }

            if (input == "q") {
                return;
            }

            bool is_valid = true;
            for (char c : input) {
                if (!isdigit(c)) {
                    is_valid = false;
                    break;
                }
            }

            if (!is_valid) {
                cout << "\033[1;31mInvalid input! Please enter a number between 1 and 5 or 'q' to quit.\033[0m\n";
                continue;
            }

            int choice = stoi(input);
            if (choice < 1 || choice > 5) {
                cout << "\033[1;31mInvalid input! Please enter a number between 1 and 5 or 'q' to quit.\033[0m\n";
                continue;
            }
             
            else if (choice == 2) {
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
                    cout << "\033[1;31mNo saved game found or invalid game state. Please start a new game.\033[0m\n";
                }
            }
        
            else if (choice == 1){
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
                        cout << "\033[1;31mInvalid input! Please enter a number between 1 and 4.\033[0m\n";
                        continue;
                    }
                    if (levelInput == "q") {
                        break;
                    }
                    bool level_valid = true;
                    for (char c : levelInput) {
                        if (!isdigit(c)) {
                            level_valid = false;
                            break;
                        }
                    }

                    if (!level_valid) {
                        cout << "\033[1;31mInvalid input! Please enter a number between 1 and 4.\033[0m\n";
                        continue;
                    }

                    int level = stoi(levelInput);
                    if (level >= 1 && level <= 4) {
                        setDifficulty(level);
                        int result = play();
                        saveGameState();
                        if (result == 1) {
                            continue;
                        }
                        break;
                    } else {
                        cout << "\033[1;31mInvalid choice! Please enter a number between 1 and 4.\033[0m\n";
                    }
                }   
            } 

            else if (choice == 3) {
                shop_menu(totalScore, TemporaryInvincibility, AutoSweep, ShieldCount);
                continue;
            }

            else if (choice == 4) {
                quizChallenge.showQuizChallengeMenu(totalScore);
            }
            else if (choice == 5) {
                guide();
            }
        }
    }
};

int main() {
    CoverDisplay cover;
    cover.showAnimatedCover();
    
    cin.get();
    
    Minesweeper game;
    game.showMenu();

    if (hasActiveGame) {
        saveGameState();
    }
    
    return 0;
}
