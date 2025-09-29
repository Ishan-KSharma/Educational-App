#include "quizwidget.h"
#include <QGroupBox>
#include <QRadioButton>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QVBoxLayout>

QuizWidget::QuizWidget(Quiz* quiz, QWidget* parent)
    : QWidget(parent), quiz(quiz), score(0)
{
    // Main layout for this widget
    QVBoxLayout* outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);

    // Create scroll area
    scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    outerLayout->addWidget(scrollArea);

    // Create content widget
    contentWidget = new QWidget;
    mainLayout = new QVBoxLayout(contentWidget);
    mainLayout->setAlignment(Qt::AlignTop);

    createQuestionWidgets();

    checkButton = new QPushButton("Check Answers");
    connect(checkButton, &QPushButton::clicked, this, &QuizWidget::checkAnswers);
    mainLayout->addWidget(checkButton);

    scrollArea->setWidget(contentWidget);
}

QuizWidget::~QuizWidget()
{
    qDeleteAll(answerGroups);
}

void QuizWidget::createQuestionWidgets()
{
    for (int i = 0; i < quiz->size(); i++) {
        QGroupBox* questionBox = new QGroupBox;
        QVBoxLayout* boxLayout = new QVBoxLayout(questionBox);

        QVector<QString> options;
        int correctIndex;
        QString question = quiz->getQuestionAndOptions(i, options, correctIndex);
        correctIndices.append(correctIndex);

        // Add question text
        QLabel* questionLabel = new QLabel(question);
        boxLayout->addWidget(questionLabel);

        // Create button group for options
        QButtonGroup* buttonGroup = new QButtonGroup(questionBox);
        answerGroups.append(buttonGroup);

        // Add radio buttons for options
        for (int j = 0; j < options.size(); ++j) {
            QRadioButton* radio = new QRadioButton(options[j]);
            buttonGroup->addButton(radio, j); // Store option index as ID
            boxLayout->addWidget(radio);
        }

        mainLayout->addWidget(questionBox);
    }
}

void QuizWidget::checkAnswers()
{
    score = 0;

    for (int i = 0; i < quiz->size(); ++i) {
        QButtonGroup* group = answerGroups[i];
        int selectedId = group->checkedId();

        if (selectedId == correctIndices[i]) {
            score++;
            // Visual feedback for correct answer
            group->button(selectedId)->setStyleSheet("color: green;");
        } else if (selectedId != -1) {
            // Visual feedback for incorrect answer
            group->button(selectedId)->setStyleSheet("color: red;");
        }
    }

    // Disable further changes
    for (QButtonGroup* group : answerGroups) {
        group->setExclusive(false);
        for (QAbstractButton* button : group->buttons()) {
            button->setEnabled(false);
        }
    }

    // Show results
    QString result = QString("You scored %1 out of %2!").arg(score).arg(quiz->size());
    QMessageBox::information(this, "Quiz Results", result);

    emit quizCompleted(score, quiz->size());

    // Change button text
    checkButton->setText("Done");
    disconnect(checkButton, &QPushButton::clicked, this, &QuizWidget::checkAnswers);
}

int QuizWidget::getScore() const {
    return score;
}
