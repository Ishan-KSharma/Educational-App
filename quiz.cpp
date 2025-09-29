#include <quiz.h>
#include <random>

Quiz::Quiz(QVector<QVector<QString>> questionsAndOptions, int quizSize) : questionsAndOptions(questionsAndOptions), quizSize(quizSize) {
    shuffle();
}

Quiz::Quiz() : Quiz({}, 0) {}

QString Quiz::getQuestionAndOptions(int index, QVector<QString>& options, int& correctAnswer) {
    options = this->questionsAndOptions[index].sliced(1);
    QString correctOption = options[0];

    std::random_device rd;
    std::mt19937 gen(rd());

    std::shuffle(options.begin(), options.end(), gen);

    for (int i = 0; i < options.size(); ++i) {
        if (options[i] == correctOption) {
            correctAnswer = i;
            break;
        }
    }

    return this->questionsAndOptions[index][0];
}

int Quiz::size(){
    return quizSize;
}

void Quiz::shuffle(){
    std::random_device rd;
    std::mt19937 gen(rd());

    std::shuffle(questionsAndOptions.begin(), questionsAndOptions.end(), gen);
}
