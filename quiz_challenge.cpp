#include "quiz_challenge.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include <unordered_set>
#include <fstream>
#include <thread> 

QuizChallenge::QuizChallenge() : QuizScore(0) {
    srand(time(0));
    initializeQuestionBank();
    loadAnsweredQuestions();
}

void QuizChallenge::initializeQuestionBank() {
    questionBank = {
        {"What is the correct syntax to output 'Hello World' in C++?",
            {"System.out.println(\"Hello World\");", "cout << \"Hello World\";", 
             "print(\"Hello World\");", "Console.WriteLine(\"Hello World\");"}, 1, "q1"},
        {"Which operator is used to allocate memory in C++?",
            {"malloc", "new", "alloc", "create"}, 1, "q2"},
        {"What is the extension for C++ source files?",
            {".c", ".cpp", ".cxx", ".cs"}, 1, "q3"},
        {"Which header file is needed for input/output operations in C++?",
            {"<stdio.h>", "<iostream>", "<inputoutput>", "<ios>"}, 1, "q4"},
        {"What is the correct way to include the standard namespace in C++?",
            {"using namespace std;", "import std;", "#include std", "namespace std;"}, 0, "q5"},
        {"Which of these is a valid variable name in C++?",
            {"int", "2var", "_var", "var-name"}, 2, "q6"},
        {"What is the correct syntax for a single-line comment in C++?",
            {"<!-- comment -->", "/* comment */", "// comment", "# comment"}, 2, "q7"},
        {"Which data type is used to store true/false values in C++?",
            {"bool", "bit", "boolean", "flag"}, 0, "q8"},
        {"What is the size of 'int' data type in C++ typically?",
            {"1 byte", "2 bytes", "4 bytes", "8 bytes"}, 2, "q9"},
        {"Which loop is guaranteed to execute at least once?",
            {"for", "while", "do-while", "repeat-until"}, 2, "q10"},
        {"What is the correct way to declare a constant in C++?",
            {"constant int x = 5;", "#define x 5", "const int x = 5;", "final int x = 5;"}, 2, "q11"},
        {"Which operator is used to access members of a structure?",
            {".", "->", "::", ":"}, 0, "q12"},
        {"What is the correct way to declare a pointer in C++?",
            {"int ptr;", "int *ptr;", "*int ptr;", "ptr int;"}, 1, "q13"},
        {"Which function is used to deallocate memory in C++?",
            {"free", "delete", "remove", "dealloc"}, 1, "q14"},
        {"What is the default return type of main() function in C++?",
            {"void", "int", "char", "float"}, 1, "q15"},
        {"Which of these is not a valid C++ comparison operator?",
            {"==", "!=", "=<", ">="}, 2, "q16"},
        {"What is the correct syntax for a function prototype in C++?",
            {"function int foo();", "int foo() {}", "int foo();", "def foo():"}, 2, "q17"},
        {"Which of these is not a valid C++ data type?",
            {"double", "string", "real", "char"}, 2, "q18"},
        {"What is the correct way to read input in C++?",
            {"scanf", "cin >>", "input", "read"}, 1, "q19"},
        {"Which operator is used for dynamic casting in C++?",
            {"dynamic_cast", "cast", "convert", "typeid"}, 0, "q20"},
        {"What is the correct syntax for a class definition in C++?",
            {"class MyClass {}", "def MyClass:", "MyClass class {}", "new class MyClass"}, 0, "q21"},
        {"Which of these is not a valid C++ storage class?",
            {"auto", "static", "extern", "public"}, 3, "q22"},
        {"What is the correct way to declare an array in C++?",
            {"int array[10];", "array int[10];", "int[10] array;", "array[10] int;"}, 0, "q23"},
        {"Which header file is needed for file operations in C++?",
            {"<file>", "<fstream>", "<filestream>", "<stdio.h>"}, 1, "q24"},
        {"What is the correct syntax for a switch statement in C++?",
            {"switch (x) { case 1: ... }", "switch x: case 1 ...", "case (x) { 1: ... }", "when x then 1 ..."}, 0, "q25"},
        {"Which operator is used for scope resolution in C++?",
            {".", "->", "::", ":"}, 2, "q26"},
        {"What is the correct way to pass by reference in C++?",
            {"void func(int x)", "void func(int &x)", "void func(int *x)", "void func(ref int x)"}, 1, "q27"},
        {"Which of these is not a valid C++ arithmetic operator?",
            {"+", "-", "*", "x"}, 3, "q28"},
        {"What is the correct way to declare a string in C++?",
            {"string s = \"hello\";", "char s[] = \"hello\";", "char *s = \"hello\";", "All of the above"}, 3, "q29"},
        {"Which function is used to get the length of a string in C++?",
            {"len()", "length()", "size()", "Both b and c"}, 3, "q30"},
        {"What is the correct syntax for a constructor in C++?",
            {"class MyClass { MyClass() {} };", "class MyClass { def __init__() };", "class MyClass { function MyClass() };", "class MyClass { new() };"}, 0, "q31"},
        {"Which of these is not a valid C++ access specifier?",
            {"public", "private", "protected", "internal"}, 3, "q32"},
        {"What is the correct way to concatenate strings in C++?",
            {"str1 + str2", "strcat(str1, str2)", "str1.append(str2)", "All of the above"}, 3, "q33"},
        {"Which header file is needed for mathematical functions in C++?",
            {"<math.h>", "<cmath>", "<numeric>", "Both a and b"}, 3, "q34"},
        {"What is the correct way to declare a function template in C++?",
            {"template <typename T> T func(T x);", "T func<T>(T x);", "template T func(T x);", "generic T func(T x);"}, 0, "q35"},
        {"Which operator is used for member access via pointer in C++?",
            {".", "->", "::", ":"}, 1, "q36"},
        {"What is the correct way to handle exceptions in C++?",
            {"try-catch", "try-except", "catch-throw", "throw-catch"}, 0, "q37"},
        {"Which of these is not a valid C++ loop structure?",
            {"for", "while", "do-while", "repeat-until"}, 3, "q38"},
        {"What is the correct way to declare a static member variable?",
            {"static int x;", "int static x;", "int x static;", "static(x);"}, 0, "q39"},
        {"Which function is used to generate random numbers in C++?",
            {"random()", "rand()", "rnd()", "math.random()"}, 1, "q40"},
        {"What is the correct syntax for a destructor in C++?",
            {"~ClassName()", "!ClassName()", "ClassName~()", "ClassName!()"}, 0, "q41"},
        {"Which of these is not a valid C++ STL container?",
            {"vector", "array", "list", "tuple"}, 3, "q42"},
        {"What is the correct way to declare a reference variable?",
            {"int &x = y;", "int x & = y;", "int & x = y;", "Both a and c"}, 3, "q43"},
        {"Which header file is needed for string operations in C++?",
            {"<string.h>", "<cstring>", "<string>", "Both b and c"}, 3, "q44"},
        {"What is the correct way to declare a constant pointer?",
            {"int *const ptr;", "const int *ptr;", "int const *ptr;", "All of the above"}, 0, "q45"},
        {"Which operator is used for function overloading in C++?",
            {"::", ":", "operator", "overload"}, 2, "q46"},
        {"What is the correct way to declare a friend function?",
            {"friend void func();", "void friend func();", "void func() friend;", "friend(func);"}, 0, "q47"},
        {"Which of these is not a valid C++ STL algorithm header?",
            {"<algorithm>", "<numeric>", "<functional>", "<math>"}, 3, "q48"},
        {"What is the correct way to declare a virtual function?",
            {"virtual void func();", "void virtual func();", "void func() virtual;", "virtual(func);"}, 0, "q49"},
        {"Which operator is used for bitwise AND in C++?",
            {"&", "&&", "|", "||"}, 0, "q50"}    
    };
}

void QuizChallenge::startImmediateQuiz(int& mainGameScore) {
    while (runQuiz(mainGameScore)) {}
    
    std::cout << "\nQuiz completed! Your total score: " << getQuizScore() << "\n";
    std::cout << "Press Enter to return to main menu...";
    std::string dummy;
    getline(std::cin, dummy);
}

bool QuizChallenge::runQuiz(int& mainGameScore) {
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

    while (true) {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        std::cout << "=== Quiz Challenge ===\n";
        std::cout << "Rules:\n";
        std::cout << "- Correct answer: +2 points\n";
        std::cout << "- Wrong answer: 0 points\n";
        std::cout << "- Type 'q' to quit\n\n";
        std::cout << "Score Earned by Quiz: " << getQuizScore() << "\n";
        std::cout << "Main Game Score: " << mainGameScore << "\n";
        std::cout << "Questions remaining: " << availableQuestions.size() << "\n\n";

        std::cout << "Question: " << q.question << "\n";
        for (int i = 0; i < 4; i++) {
            std::cout << i+1 << ". " << q.options[i] << "\n";
        }

        std::cout << "\nYour answer (1-4) or 'q' to quit: ";
        std::string input;
        getline(std::cin, input);
        
        if (input == "q" || input == "Q") {
            return false;
        }

        try {
            size_t pos;
            int answer = std::stoi(input, &pos); 
        
            if (pos != input.length() || answer < 1 || answer > 4) {
                throw std::invalid_argument("Invalid input");
            }
        
            answeredQuestions.insert(q.questionId);
            saveAnsweredQuestions();
        
            if (answer - 1 == q.correctAnswer) {
                QuizScore += 2;
                mainGameScore += 2;
                std::cout << "\033[1;32mCorrect! +2 points\033[0m\n";
            } else {
                std::cout << "\033[1;31mIncorrect! The correct answer was: " 
                          << q.options[q.correctAnswer] << "\033[0m\n";
            }
        
            std::cout << "Press Enter to continue...";
            std::string pause;
            std::getline(std::cin, pause);
            return true;
        }
        catch (...) {
            std::cout << "Invalid input. Please enter a number between 1 and 4 or 'q' to quit.\n";
            std::cout << "Press Enter to try again...";
            std::string pause;
            std::getline(std::cin, pause);
        }
        
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
    std::ifstream file(scoreFileName);
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
    std::ofstream file(scoreFileName);
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
    startImmediateQuiz(mainGameScore); 
}