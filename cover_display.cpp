#include <iostream>
#include <iomanip>
#include <limits>
#include "guide.h"

using namespace std;

void guide() {
    cout << string(20, '\n') << "\033[1;33mWelcome to MineHack!!! (･ω´･ )\033[0m\n";
    cout << "MineHack is a reimagined version of the classic Minesweeper game that integrates with powerful items.\n";
    cout << "Whether you're a Minesweeper pro or a newcomer, MineHack offers new mechanics and endless fun.\n";
    cout << endl;

    while (true) {
        cout << "\033[1;33mWould you like to know about...\033[0m\n";
        cout << "1. Basic Gameplay\n";
        cout << "2. Item System\n";
        cout << "3. Return to Main Menu\n" << endl;
        cout << "\033[1;34mEnter your choice (1-3): \033[0m";

        string input;
        int choice;

        // Input validation loop
        while (true) {
            getline(cin, input);

            // Trim leading and trailing spaces
            input.erase(0, input.find_first_not_of(" \t"));
            input.erase(input.find_last_not_of(" \t") + 1);

            // Validate input
            if (input.empty() || input.size() > 1 || (input[0] < '1' || input[0] > '3')) {
                cout << "\033[1;31mInvalid input! Please enter a number between 1 and 3.\033[0m\n";
                cout << "\033[1;34mEnter your choice (1-3): \033[0m";
            } else {
                choice = input[0] - '0'; // Convert char to int
                break;
            }
        }

        if (choice == 1) {
            gameplayIntroduction(); // Call the gameplay introduction
            cout << string(20, '\n');
        } else if (choice == 2) {
            itemSystemIntroduction(); // Call the item system introduction
            cout << string(20, '\n');
        } else if (choice == 3) {
            cout << string(20, '\n');
            break; // Exit the guide and return to the main menu
        }
    }
}

void gameplayIntroduction() {
    // Page 1
    cout << string(20, '\n') << "\033[1;33m=== Gameplay Introduction ===\033[0m\n" << endl;
    cout << "   0   1   2   3   4\n";
    cout << " +---+---+---+---+---+\n";
    cout << "0| . | . | . | . | . |\n";
    cout << " +---+---+---+---+---+\n";
    cout << "1| . | . | . | . | . |\n";
    cout << " +---+---+---+---+---+\n";
    cout << "2| . | . | . | . | . |\n";
    cout << " +---+---+---+---+---+\n";
    cout << "3| . | . | . | . | . |\n";
    cout << " +---+---+---+---+---+\n";
    cout << "4| . | . | . | . | . |\n";
    cout << " +---+---+---+---+---+\n";
    cout << endl;
    cout << "This is a 5x5 grid, where you uncover cells on a grid while avoiding hidden mines.\n";
    cout << "To win, you must uncover all cells (.) of the grid except mines." << endl;
    cout << "If you uncover a mine. BOOM! The game is over!\n";
    cout << "\033[1;34m(Press Enter to continue...)\033[0m" << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Wait for Enter

    // Page 2
    cout << string(20, '\n');
    cout << "   0   1   2   3   4\n";
    cout << " +---+---+---+---+---+\n";
    cout << "0| \033[1;37m \033[0m | \033[1;34m1\033[0m | \033[1;31m*\033[0m | \033[1;32m2\033[0m | \033[1;37m \033[0m |\n";
    cout << " +---+---+---+---+---+\n";
    cout << "1| \033[1;34m1\033[0m | \033[1;32m2\033[0m | \033[1;31m3\033[0m | \033[1;31m*\033[0m | \033[1;34m1\033[0m |\n";
    cout << " +---+---+---+---+---+\n";
    cout << "2| \033[1;31m*\033[0m | \033[1;31m3\033[0m | \033[1;31m*\033[0m | \033[1;32m2\033[0m | \033[1;34m1\033[0m |\n";
    cout << " +---+---+---+---+---+\n";
    cout << "3| \033[1;32m2\033[0m | \033[1;31m*\033[0m | \033[1;31m3\033[0m | \033[1;34m1\033[0m | \033[1;37m \033[0m |\n";
    cout << " +---+---+---+---+---+\n";
    cout << "4| \033[1;34m1\033[0m | \033[1;34m1\033[0m | \033[1;34m1\033[0m | \033[1;37m \033[0m | \033[1;37m \033[0m |\n";
    cout << " +---+---+---+---+---+\n";
    cout << endl;
    cout << "This is a revealed grid, where '*' represents mines.\n";
    cout << "Each number represents how many mines are adjacent to that cell.\n";
    cout << "If you uncover an empty cell, it uncovers all cells adjacent to it.\n";
    cout << "\033[1;34m(Press Enter to continue...)\033[0m" << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Wait for Enter

    // Page 3
    cout << string(20, '\n');
    cout << "   0   1   2   3   4\n";
    cout << " +---+---+---+---+---+\n";
    cout << "0| . | . | . | . | . |\n";
    cout << " +---+---+---+---+---+\n";
    cout << "1| . | . | . | . | . |\n";
    cout << " +---+---+---+---+---+\n";
    cout << "2| . | . | \033[1;31mF\033[0m | \033[1;32m2\033[0m | \033[1;34m1\033[0m |\n";
    cout << " +---+---+---+---+---+\n";
    cout << "3| . | . | \033[1;31m3\033[0m | \033[1;34m1\033[0m |   |\n";
    cout << " +---+---+---+---+---+\n";
    cout << "4| . | . | \033[1;34m1\033[0m |   |   |\n";
    cout << " +---+---+---+---+---+\n";
    cout << endl;
    cout << "From the above grid, you can deduce that the cell with the coordinate (2, 2) is a mine.\n";
    cout << "You can flag it to keep track of it by pressing 'f' for switching to flag mode.\n";
    cout << "\033[1;34m(Press Enter to return to the main menu)\033[0m" << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Wait for Enter
    return;
}

void itemSystemIntroduction() {
    cout << string(20, '\n');
    cout << "\033[1;33m=== Item System Introduction ===\033[0m\n" << endl;
    cout << "In MineHack, you can buy special items from the shop to help you conquer the minefield:\n";
    cout << "You can earn points needed for purchasing items by taking quizzes or completing games.\n";
    cout << endl;
    cout << "\033[1;33mHere's the information of special Items and their cost: \033[0m\n";
    cout << "1. **Temporary Invincibility**: Allows you to reveal three cells without triggering the mines. (5pt)\n";
    cout << "2. **Auto Sweep**: Reveals all cells in a 3x3 area around your selected cell. (10pt)\n";
    cout << "3. **Hint**: Gives you the information of three random cells. (2pt) \n";
    cout << endl;
    cout << "\033[1;34m(Press Enter to return to the main menu)\033[0m" << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Wait for Enter
    return;
}
