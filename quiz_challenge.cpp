#include "quiz_challenge.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>

QuizChallenge::QuizChallenge() : QuizScore(0) {
    srand(time(0));
    initializeQuestionBank();
}

void QuizChallenge::initializeQuestionBank() {
    questionBank = {
        // Basic Syntax (10 questions)
        {"What is the correct syntax to output 'Hello World' in C++?",
            {"System.out.println(\"Hello World\");", "cout << \"Hello World\";", 
             "print(\"Hello World\");", "Console.WriteLine(\"Hello World\");"}, 1},
        {"Which operator is used to allocate memory in C++?",
            {"malloc", "new", "alloc", "create"}, 1},
        {"How do you insert a single-line comment in C++?",
            {"// Comment", "/* Comment */", "# Comment", "-- Comment"}, 0},
        {"Which of these is not a valid C++ variable name?",
            {"myVariable", "_myVar", "123var", "var123"}, 2},
        {"What is the correct way to declare a constant in C++?",
            {"constant int x = 5;", "#define x 5", "const int x = 5;", "final int x = 5;"}, 2},
        {"Which header file is needed for input/output operations in C++?",
            {"<stdio.h>", "<iostream>", "<inputoutput>", "<ios>"}, 1},
        {"What is the size of 'int' data type in C++ (typically)?",
            {"1 byte", "2 bytes", "4 bytes", "Depends on the system"}, 3},
        {"Which of these is not a primitive data type in C++?",
            {"int", "float", "string", "double"}, 2},
        {"What does the '++' operator do?",
            {"Adds 2 to a variable", "Increments by 1", "Doubles the value", "Raises to power"}, 1},
        {"Which loop is guaranteed to execute at least once?",
            {"for", "while", "do-while", "foreach"}, 2},
    };
}

bool QuizChallenge::runQuiz() {
    if (questionBank.empty()) {
        std::cout << "No questions available!\n";
        return false;
    }

    int randomIndex = rand() % questionBank.size();
    Question q = questionBank[randomIndex];

    std::cout << "\nQuestion: " << q.question << "\n";
    for (int i = 0; i < 4; i++) {
        std::cout << i+1 << ". " << q.options[i] << "\n";
    }

    int answer;
    while (true) {
        std::cout << "Your answer (1-4): ";
        std::string input;
        getline(std::cin, input);
        
        try {
            answer = stoi(input);
            if (answer >= 1 && answer <= 4) break;
            std::cout << "Please enter a number between 1 and 4.\n";
        } catch (...) {
            std::cout << "Invalid input! Please enter a number.\n";
        }
    }

    if (answer-1 == q.correctAnswer) {
        QuizScore += 2; // Award 2 points for correct answer
        std::cout << "\033[1;32mCorrect! You earned 2 points.\033[0m\n";
        return true;
    } else {
        std::cout << "\033[1;31mIncorrect! The correct answer was: " 
                 << q.options[q.correctAnswer] << "\033[0m\n";
        return false;
    }
}

void QuizChallenge::addScore(int points) {
    QuizScore += points;
}

int QuizChallenge::getQuizScore() const {
    return QuizScore;
}

void QuizChallenge::setQuizScore(int score) {
    QuizScore = score;
}

void QuizChallenge::resetScore() {
    QuizScore = 0;
}

void QuizChallenge::addQuestion(const std::string& question, 
                              const std::string options[4], 
                              int correctAnswer) {
    Question newQ;
    newQ.question = question;
    for (int i = 0; i < 4; i++) {
        newQ.options[i] = options[i];
    }
    newQ.correctAnswer = correctAnswer;
    questionBank.push_back(newQ);
}

void QuizChallenge::showQuizChallengeMenu(int& mainGameScore) {
    while (true) {
        std::cout << "\n=== Quiz Challenge ===\n";
        std::cout << "Score Earned by Quiz: " << getQuizScore() << "\n";
        std::cout << "Main Game Score: " << mainGameScore << "\n";
        std::cout << "1. Answer a question (earn 2 points if correct)\n";
        std::cout << "2. Back to main menu\n";

        std::string input;
        std::cout << "Select option: ";
        getline(std::cin, input);

        if (input == "1") {
            if (runQuiz()) {
                mainGameScore += 2; // Update the main game score directly
            }
        } else if (input == "2") {
            break;
        } else {
            std::cout << "Invalid option!\n";
        }
    }
}