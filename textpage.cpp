#include "textpage.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>

TextPage::TextPage(const QString& headerText,
                   const QVector<QString>& subHeaders,
                   const QVector<QString>& subTexts,
                   QWidget* parent) : QWidget(parent){
    setupLayout(headerText, subHeaders, subTexts);
}

void TextPage::setupLayout(const QString& headerText,
                           const QVector<QString>& subHeaders,
                           const QVector<QString>& subTexts) {
    // QLabel* textArea = new QLabel(this);
    // textArea->setWordWrap(true);
    // textArea->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
    // textArea->setFrameShape(QFrame::NoFrame);

    QWidget* container = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setSpacing(0);
    layout->setAlignment(Qt::AlignTop);

    QLabel* headerLabel = new QLabel(headerText);
    QFont headerFont;
    headerFont.setPointSize(28);
    headerFont.setBold(true);
    headerFont.setUnderline(true);
    headerLabel->setFont(headerFont);
    headerLabel->setAlignment(Qt::AlignHCenter);
    headerLabel->setStyleSheet("padding-bottom: 10px;");

    headerLabel->setWordWrap(true);
    layout->addWidget(headerLabel);

    for (int i = 0; i < subHeaders.size() && i < subTexts.size(); ++i) {
        QLabel* subHeaderLabel = new QLabel(subHeaders[i]);
        QFont subHeaderFont;
        subHeaderFont.setPointSize(18);
        subHeaderFont.setBold(true);
        subHeaderLabel->setFont(subHeaderFont);
        subHeaderLabel->setWordWrap(true);
        subHeaderLabel->setStyleSheet("padding-bottom: 5px;");
        layout->addWidget(subHeaderLabel);

        QLabel* textLabel = new QLabel(subTexts[i]);
        textLabel->setWordWrap(true);
        textLabel->setStyleSheet("padding-bottom: 20px;");
        layout->addWidget(textLabel);
    }

    container->setLayout(layout);
    // textArea->setWidget(container);

    QVBoxLayout* outerLayout = new QVBoxLayout(this);
    outerLayout->addWidget(container);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(outerLayout);
}

