#ifndef COVER_DISPLAY_H
#define COVER_DISPLAY_H

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

class CoverDisplay {
public:
    CoverDisplay();
    void showAnimatedCover();

private:
    const std::vector<std::string> mineAscii;
    const std::vector<std::string> hackAscii;

    void clearScreen();
    void printWithColor(const std::string& text, int colorCode);
    void animateText(const std::string& text, int delayMs);
    void printAsciiArt();
    void print3DText(const std::string& text);
};

#endif
