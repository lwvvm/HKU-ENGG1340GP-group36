#ifndef QUIZ_CHALLENGE_H
#define QUIZ_CHALLENGE_H

#include <vector>
#include <string>

class QuizChallenge {
private:
    struct Question {
        std::string question;
        std::string options[4];
        int correctAnswer;
    };

    std::vector<Question> questionBank;
    int totalScore;

    void initializeQuestionBank();

public:
    QuizChallenge();
    
    bool runQuiz();  
    void addScore(int points);
    int getTotalScore() const;
    void resetScore();

    void showQuizChallengeMenu(int& mainGameScore);
    
    // For potential future expansion
    void addQuestion(const std::string& question, 
                    const std::string options[4], 
                    int correctAnswer);
};

#endif 