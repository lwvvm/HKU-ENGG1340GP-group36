#ifndef QUIZ_CHALLENGE_H
#define QUIZ_CHALLENGE_H

#include <vector>
#include <string>
#include <unordered_set>  
#include <fstream>        

class QuizChallenge {
private:
    int QuizScore;
    std::unordered_set<std::string> answeredQuestions; 
    std::string scoreFileName = "quiz_score_data.txt";
    
    struct Question {
        std::string question;
        std::string options[4];
        int correctAnswer;
        std::string questionId;
    };

    std::vector<Question> questionBank;
    
    void initializeQuestionBank();
    void loadAnsweredQuestions();  
    void saveAnsweredQuestions();  

public:
    QuizChallenge();
    ~QuizChallenge();
    
    bool runQuiz(int& mainGameScore);
    void addScore(int points);
    int getQuizScore() const;
    void setQuizScore(int score);
    void resetScore();
    void showQuizChallengeMenu(int& mainGameScore);
    void startImmediateQuiz(int& mainGameScore);
};

#endif