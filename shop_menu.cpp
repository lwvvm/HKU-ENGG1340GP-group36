#include <iostream>
#include <fstream>
#include <string>
#include "shop_menu.h"
using namespace std;

void saveItems(int TemporaryInvincibility, int AutoSweep, int Hint) {
    std::ofstream file("items_storage.txt");
    if (file) {
        file << TemporaryInvincibility << "\n";
        file << AutoSweep << "\n";
        file << Hint << "\n";
        file.close();
    } else {
        cout << "Error: Unable to save items to file.\n";
    }
}

void loadItems(int& TemporaryInvincibility, int& AutoSweep, int& Hint) {
    std::ifstream file("items_storage.txt");
    if (file) {
        file >> TemporaryInvincibility;
        file >> AutoSweep;
        file >> Hint;
        file.close();
    } else {
        TemporaryInvincibility = 0;
        AutoSweep = 0;
        Hint = 0;
    }
}

bool isValidNumber(const string& input) {
    for (char c : input) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

void shop_menu(int& totalScore, int& TemporaryInvincibility, int& AutoSweep, int& Hint) {
    int choice = 0;
    while (true) {
        cout << "\n=== Shop Menu ===\n";
        cout << "1. Buy items\n";
        cout << "2. Items storage\n";
        cout << "3. Exit Shop\n";
        cout << "Total Score: " << totalScore << "\n"; 
        cout << "Select an option (1-3): ";
        
        string input;
        getline(cin, input);
        
        if (input.empty()) {
            cout << "\033[1;32mInvalid input! Please enter a number between 1 and 3.\033[0m\n";
            continue;
        }
        
        try {
            if (!isValidNumber(input)) {
                throw invalid_argument("Invalid input");
            }
            choice = stoi(input);
        } catch (...) {
            cout << "\033[1;32mInvalid input! Please enter a number between 1 and 3.\033[0m\n";
            continue;
        }
        
        if (choice == 3) {
            saveItems(TemporaryInvincibility, AutoSweep, Hint);
            break;
        } else if (choice < 1 || choice > 3) {
            std::cout << "\033[1;32mInvalid choice! Please enter a number between 1 and 3.\033[0m\n";
            continue;
        }
        
        switch (choice) {
            case 1: { // Buy Items
                while (true){
                    cout << "\n=== Buy Items ===\n";
                    cout << "Total Score: " << totalScore << endl;
                    cout << "1. Buy Temporary Invincibility (-5 points)\n";
                    cout << "2. Buy Auto Sweep (-10 points)\n";
                    cout << "3. Buy Hint (-2 points)\n";
                    cout << "4. Back to Shop Menu\n";
                    cout << "Select an option (1-4): ";
                    
                    string buyInput;
                    getline(cin, buyInput);
                    if (buyInput.empty()) {
                        cout << "\033[1;32mInvalid input! Please enter a number between 1 and 4.\033[0m\n";
                        continue;
                    }
                    
                    int buyChoice = 0;
                    try {
                        if (!isValidNumber(buyInput)) {
                            throw invalid_argument("Invalid input");
                        }
                        buyChoice = stoi(buyInput);
                    } catch (...) {
                        cout << "\033[1;32mInvalid input! Please enter a number between 1 and 4.\033[0m\n";
                        continue;
                    }
    
                    if (buyChoice == 4) {
                        break; // Back to shop menu
                    }
    
                    int cost = 0;
                    switch (buyChoice) {
                        case 1: cost = 5; TemporaryInvincibility++; break;//ITEMS PURCHASED
                        case 2: cost = 10; AutoSweep++; break;
                        case 3: cost = 2; Hint++; break;
                        default:
                            cout << "\033[1;32mInvalid choice! Please enter a number between 1 and 4.\033[0m\n";
                            continue;
                    }
    
                    if (totalScore >= cost) {
                        totalScore -= cost;
                        cout << "\033[1;32mPurchase successful! You bought option " << buyChoice << ".\033[0m\n";
                        saveItems(TemporaryInvincibility, AutoSweep, Hint);
                    } else {
                        cout << "\033[1;32mNot enough points! You need at least " << cost << " points.\033[0m\n";
                        // Revert the increment if purchase fails
                        switch (buyChoice) {
                            case 1: TemporaryInvincibility--; break;
                            case 2: AutoSweep--; break;
                            case 3: Hint--; break;
                        }
                    } 
                }
                break;
            }
            case 2: { // Items Storage
                cout << "\n\033[1;32m=== Items Storage ===\033[0m\n";
                cout << "\033[1;32mTemporary Invincibility: " << TemporaryInvincibility << "\033[0m\n";
                cout << "\033[1;32mAuto Sweep: " << AutoSweep << "\033[0m\n";
                cout << "\033[1;32mHint: " << Hint << "\033[0m\n";
                break;
            }
            default:
                cout << "\033[1;32mInvalid choice! Please enter a number between 1 and 3.\033[0m\n";
                break;
        }
    }
}
