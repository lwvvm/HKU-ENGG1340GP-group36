#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <string>

using namespace std;

class Minesweeper {
private:
    int rows;
    int cols;
    int mines;
    int score;
    int totalScore;
    vector<vector<bool>> mineGrid;
    vector<vector<bool>> revealed;
    vector<vector<bool>> flagged;
    bool gameOver;
    bool gameWon;

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

public:
    Minesweeper() : rows(9), cols(9), mines(10), score(0), totalScore(0), gameOver(false), gameWon(false) {
        srand(time(0));
        initializeGrid();
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
                rows = 16;
                cols = 16;
                mines = 40;
                score = 5;
                break;
            case 3: // Hard
                rows = 16;
                cols = 30;
                mines = 99;
                score = 8;
                break;
            case 4: // Expert
                rows = 20;
                cols = 40;
                mines = 180;
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
                        cout << " " << count << " |";
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
            cout << "Enter command (r for reveal, f for flag/unflag, q to quit): ";
            char cmd;
            cin >> cmd;

            if (cmd == 'q') {
                return;
            }

            int r, c;
            cout << "Enter row and column: ";
            cin >> r >> c;

            if (!isValid(r, c)) {
                cout << "Invalid position!\n";
                continue;
            }

            if (cmd == 'r') {
                if (flagged[r][c]) {
                    cout << "Cell is flagged. Unflag it first.\n";
                    continue;
                }
                revealCell(r, c);
                if (gameOver) {
                    cout << "Game Over! You hit a mine.\n";
                    printBoard(true);
                    return;
                }
            } else if (cmd == 'f') {
                if (!revealed[r][c]) {
                    flagged[r][c] = !flagged[r][c];
                } else {
                    cout << "Cannot flag a revealed cell.\n";
                }
            }

            if (checkWin()) {
                gameWon = true;
                totalScore += score;
                cout << "Congratulations! You won!\n";
                cout << "You earned " << score << " points!\n";
                printBoard(true);
                return;
            }
        }
    }

    void showMenu() {
        while (true) {
            cout << "\n=== Minesweeper ===\n";
            cout << "Total Score: " << totalScore << "\n";
            cout << "1. Easy (9x9, 10 mines) - 3 points\n";
            cout << "2. Medium (16x16, 40 mines) - 5 points\n";
            cout << "3. Hard (16x30, 99 mines) - 8 points\n";
            cout << "4. Expert (20x40, 180 mines) - 10 points\n";
            cout << "5. Quit\n";
            cout << "Select difficulty: ";

            int choice;
            cin >> choice;

            if (choice == 5) {
                break;
            } else if (choice >= 1 && choice <= 4) {
                setDifficulty(choice);
                play();
            } else {
                cout << "Invalid choice!\n";
            }
        }
    }
};

int main() {
    Minesweeper game;
    game.showMenu();
    return 0;
}