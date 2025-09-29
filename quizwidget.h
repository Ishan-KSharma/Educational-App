/**
 * QuizWidget Class, manages the frontend for all quizes except the final quiz
 *
 * @authors Ishan Sharma
 * @version 4/27/2025
 */

#ifndef QUIZWIDGET_H
#define QUIZWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include <QButtonGroup>
#include <QBoxLayout>
#include <QPushButton>
#include "quiz.h"

class QuizWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param quiz - the model
     * @param parent
     */
    explicit QuizWidget(Quiz* quiz, QWidget* parent = nullptr);

    /**
     * Destructor
     */
    ~QuizWidget();

    /**
     * Getter for the score after the quiz has been completed
     * @return num correct
     */
    int getScore() const;

signals:
    /**
     * Handles quiz finished button
     * @param score - how many they got right
     * @param totalQuestions - out of how many total questions
     */
    void quizCompleted(int score, int totalQuestions);

private slots:
    /**
     * Handles quiz completed button click
     */
    void checkAnswers();

private:
    /**
     * Creates the mini widgets that house questions
     */
    void createQuestionWidgets();

    /**
     * The model
     */
    Quiz* quiz;

    /**
     * Score/num correct
     */
    int score;

    /**
     * Scroll area for the quiz content
     */
    QScrollArea* scrollArea;

    /**
     * Houses the quiz
     */
    QWidget* contentWidget;

    /**
     * Layout for the entire quiz
     */
    QVBoxLayout* mainLayout;

    /**
     * The button at the end to check answers
     */
    QPushButton* checkButton;

    /**
     * The choices
     */
    QVector<QButtonGroup*> answerGroups;

    /**
     * All the correct answer choices
     */
    QVector<int> correctIndices;
};

#endif // QUIZWIDGET_H
