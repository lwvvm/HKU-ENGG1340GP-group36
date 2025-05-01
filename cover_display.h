#ifndef COVER_DISPLAY_H
#define COVER_DISPLAY_H

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>

class CoverDisplay {
public:
    CoverDisplay();
    void showAnimatedCover();
    void printAsciiArt();
    void print3DText(const std::string& text);
    void animateText(const std::string& text, int delayMs = 100);
    void clearScreen();
    void printWithColor(const std::string& text, int colorCode);

private:
    const std::vector<std::string> mineAscii;
    const std::vector<std::string> hackAscii;
};

#endif
