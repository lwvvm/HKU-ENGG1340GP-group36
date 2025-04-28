#include "quiz_challenge.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include <unordered_set>
#include <fstream>

QuizChallenge::QuizChallenge() : QuizScore(0) {
    srand(time(0));
    initializeQuestionBank();
    loadAnsweredQuestions();
}

void QuizChallenge::initializeQuestionBank() {
    questionBank = {
        // Basic Syntax (10 questions)
        {"What is the correct syntax to output 'Hello World' in C++?",
            {"System.out.println(\"Hello World\");", "cout << \"Hello World\";", 
             "print(\"Hello World\");", "Console.WriteLine(\"Hello World\");"}, 1, "q1"},
        {"Which operator is used to allocate memory in C++?",
            {"malloc", "new", "alloc", "create"}, 1, "q2"},
        {"How do you insert a single-line comment in C++?",
            {"// Comment", "/* Comment */", "# Comment", "-- Comment"}, 0, "q3"},
        {"Which of these is not a valid C++ variable name?",
            {"myVariable", "_myVar", "123var", "var123"}, 2, "q4"},
        {"What is the correct way to declare a constant in C++?",
            {"constant int x = 5;", "#define x 5", "const int x = 5;", "final int x = 5;"}, 2, "q5"},
        {"Which header file is needed for input/output operations in C++?",
            {"<stdio.h>", "<iostream>", "<inputoutput>", "<ios>"}, 1, "q6"},
        {"What is the size of 'int' data type in C++ (typically)?",
            {"1 byte", "2 bytes", "4 bytes", "Depends on the system"}, 3, "q7"},
        {"Which of these is not a primitive data type in C++?",
            {"int", "float", "string", "double"}, 2, "q8"},
        {"What does the '++' operator do?",
            {"Adds 2 to a variable", "Increments by 1", "Doubles the value", "Raises to power"}, 1, "q9"},
        {"Which loop is guaranteed to execute at least once?",
            {"for", "while", "do-while", "foreach"}, 2, "q10"},
    };
}

bool QuizChallenge::runQuiz() {
    std::vector<Question> availableQuestions;
    for (const auto& q : questionBank) {
        if (answeredQuestions.find(q.questionId) == answeredQuestions.end()) {
            availableQuestions.push_back(q);
        }
    }

    if (availableQuestions.empty()) {
        std::cout << "You've answered all available questions!\n";
        return false;
    }

    int randomIndex = rand() % availableQuestions.size();
    Question q = availableQuestions[randomIndex];

    answeredQuestions.insert(q.questionId);
    saveAnsweredQuestions();

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
        QuizScore += 2; 
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

void QuizChallenge::loadAnsweredQuestions() {
    std::ifstream file("minesweeper_score.txt");
    if (file) {
        std::string line;
        
        if (getline(file, line)) {
            try {
                QuizScore = stoi(line);
            } catch (...) {
                QuizScore = 0; 
            }
        }

        while (getline(file, line)) {
            if (line.find("answered:") == 0) {
                answeredQuestions.insert(line.substr(9)); 
            }
        }
    }
}

void QuizChallenge::saveAnsweredQuestions() {
    std::ofstream file("minesweeper_score.txt");
    if (!file) {
        std::cerr << "Error: Could not save progress to minesweeper_score.txt\n";
        return;
    }
    
    file << QuizScore << "\n";
    
    for (const auto& qId : answeredQuestions) {
        file << "answered:" << qId << "\n";
    }
}

QuizChallenge::~QuizChallenge() {
    saveAnsweredQuestions(); 
}

void QuizChallenge::showQuizChallengeMenu(int& mainGameScore) {
    while (true) {
        int remaining = 0;
        for (const auto& q : questionBank) {
            if (answeredQuestions.find(q.questionId) == answeredQuestions.end()) {
                remaining++;
            }
        }
        std::cout << "\n=== Quiz Challenge ===\n";
        std::cout << "Score Earned by Quiz: " << getQuizScore() << "\n";
        std::cout << "Main Game Score: " << mainGameScore << "\n";
        std::cout << "1. Answer a question (earn 2 points if correct)\n";
        std::cout << "2. Back to main menu\n";
        std::cout << "Questions remaining: " << remaining << "\n";

        std::string input;
        std::cout << "Select option: ";
        getline(std::cin, input);

        if (input == "1") {
            if (runQuiz()) {
                mainGameScore += 2; 
            }
        } else if (input == "2") {
            break;
        } else {
            std::cout << "Invalid option!\n";
        }
    }
}