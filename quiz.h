/**
 * Quiz Class, manages the backend/model for all quizes
 *
 * @authors Ishan Sharma
 * @version 4/27/2025
 */
#ifndef QUIZ_H
#define QUIZ_H

#include <QVector>

class Quiz
{
private:
    /**
     * Contains all the questions and options
     * .size = size of question bank
     * [question index].size-1 = number of options
     * [question index][0] is the question
     * [question index][1] is the correct option
     * [question index][2-size] are the incorrect options
     */
    QVector<QVector<QString>> questionsAndOptions;

    /**
     * There's going to be a question bank and this will control the size of the actual size of the quiz
     */
    int quizSize;

public:
    /**
     * Contructor
     * @param questionsAndOptions
     * @param quizSize
     */
    Quiz(QVector<QVector<QString>> questionsAndOptions, int quizSize);

    /**
     * Default Contructor
     */
    Quiz();

    /**
     * @brief getQuestionsAndOptions
     * @param index
     * @param options
     * @param correctAnswer
     * @return
     */
    QString getQuestionAndOptions(int index, QVector<QString>& options, int& correctAnswer);

    /**
     * Size to traverse the quiz
     * @return quizSize
     */
    int size();

    /**
     * Shuffles questionsAndOptions
     */
    void shuffle();
};

#endif // QUIZ_H

