#include "cover_display.h"

CoverDisplay::CoverDisplay() : 
    mineAscii{
        ".___  ___.  __  .__   __.  _______", 
        "|   \\/   | |  | |  \\ |  | |   ____|",
        "|  \\  /  | |  | |   \\|  | |  |__",   
        "|  |\\/|  | |  | |  . `  | |   __|",  
        "|  |  |  | |  | |  |\\   | |  |____", 
        "|__|  |__| |__| |__| \\__| |_______"
    },
    hackAscii{
        "__    __       ___       ______  __  ___", 
        "|  |  |  |     /   \\     /      ||  |/  /", 
        "|  |__|  |    /  ^  \\   |  ,----'|  '  /",  
        "|   __   |   /  /_\\  \\  |  |     |    <",   
        "|  |  |  |  /  _____  \\ |  `----.|  .  \\ ",  
        "|__|  |__| /__/     \\__\\ \\______||__|\\__\\ "
    }
{}

void CoverDisplay::clearScreen() {
    // ANSI clear screen
    std::cout << "\033[2J\033[1;1H";
}

void CoverDisplay::printWithColor(const std::string& text, int colorCode) {
    // ANSI color code
    std::cout << "\033[" << colorCode << "m" << text << "\033[0m";
}

void CoverDisplay::animateText(const std::string& text, int delayMs) {
    for (char c : text) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }
    std::cout << std::endl;
}

void CoverDisplay::printAsciiArt() {
    for (const auto& line : mineAscii) {
        printWithColor(line, 32); // green
        std::cout << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void CoverDisplay::print3DText(const std::string& text) {
    for (const auto& line : hackAscii) {
        printWithColor(line, 31); // red
        std::cout << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(80));
    }
}

void CoverDisplay::showAnimatedCover() {
    clearScreen();
    
    // part1: slowly show characters
    printAsciiArt();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    // part2: 3-dimensional characters
    print3DText("MineHack");
    
    // part3: animation
    std::this_thread::sleep_for(std::chrono::seconds(1));
    animateText("> The Ultimate Minesweeper Adventure <", 50);
    
    std::this_thread::sleep_for(std::chrono::seconds(2));
    printWithColor("       Press Enter to start...", 14);
}
