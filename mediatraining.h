#ifndef MEDIATRAINING_H
#define MEDIATRAINING_H

#include <QWidget>
#include "quiz.h"

/**
 * The MediaTraining class is for the interactive quiz module section in the Media Training module. It creates a quiz on the screen
 * that the user can interact with, in the style of a Fifa video game press conference.
 *
 * @authors Heiffel Tower
 * @version April 24th, 2025
 */
namespace Ui {
class MediaTraining;
}
class MediaTraining : public QWidget
{
    Q_OBJECT

public:
    /**
     * Constructor for the MediaTraining class
     * @param parent
     */
    explicit MediaTraining(QWidget *parent = nullptr);
    /**
     * Destructor for the MediaTraining class
     */
    ~MediaTraining();

private:
    /**
     * @brief ui - the user interactive ui
     */
    Ui::MediaTraining *ui;
    /**
     * Sets up all the questions for the quiz and creates a Quiz object
     */
    void setupQuiz();
    /**
     * Asks a question from the quiz and changes the text on the screen
     */
    void askQuestion();
    /**
     * A quiz object that holds all the questions
     */
    Quiz quiz;
    /**
     * Holds the index of the previous question to see if the quiz needs to be reshuffled
     */
    int previousQuestionIndex;
    /**
     * Holds the index of the correct answer to compare to which button the user pressed to check for correctness
     */
    int correctIndex;
    /**
     * Changes the background image depending on how the user is doing
     */
    void changeImage();
    /**
     * Checks if the quiz has been passed
     */
    void emitQuizDone();

private slots:
    /**
     * Checks if the picked the correct answer by seeing if the correct answer was the first option
     */
    void handleOption1();
    /**
     * Checks if the picked the correct answer by seeing if the correct answer was the second option
     */
    void handleOption2();
    /**
     * Checks if the picked the correct answer by seeing if the correct answer was the third option
     */
    void handleOption3();
    /**
     * Checks if the picked the correct answer by seeing if the correct answer was the fourth option
     */
    void handleOption4();
signals:
    /**
     * Signal that emits once the quiz is passed
     */
    void mediaQuizPassed();
};

#endif // MEDIATRAINING_H
