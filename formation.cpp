#include "formation.h"
#include "textpage.h"  // Include the TextPage header
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QPixmap>

Formation::Formation(const QString& formationImagePath,
                             const QString& circleImagePath,
                             const QString& circleText,
                             const QString& headerText,
                             const QVector<QString>& subHeaders,
                             const QVector<QString>& subTexts,
                             QWidget* parent) : QWidget(parent) {
    setupLayout(formationImagePath, circleImagePath, "Achievements:\n" + circleText,
                headerText, subHeaders, subTexts);
}
Formation::Formation(const QString& formationImagePath,
                     const QString& circleImagePath,
                     const QString& circleText,
                     const QString& headerText,
                     const QVector<QString>& subTexts,
                     QWidget* parent)
    : Formation(formationImagePath, circleImagePath, circleText, headerText,
                {"Overview", "Benefits", "Drawbacks"}, subTexts, parent) {}

void Formation::setupLayout(const QString& formationImagePath,
                                const QString& circleImagePath,
                                const QString& circleText,
                                const QString& headerText,
                                const QVector<QString>& subHeaders,
                                const QVector<QString>& subTexts) {
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    QWidget* container = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->setSpacing(20);
    mainLayout->setAlignment(Qt::AlignTop);

    // 1. Formation image at the top
    QLabel* formationImageLabel = new QLabel();
    QPixmap formationPixmap(formationImagePath);
    formationImageLabel->setPixmap(formationPixmap.scaledToWidth(801, Qt::SmoothTransformation));
    formationImageLabel->setAlignment(Qt::AlignHCenter);
    mainLayout->addWidget(formationImageLabel);

    // 2. Circle image with text to the right
    QHBoxLayout* circleLayout = new QHBoxLayout();
    circleLayout->setSpacing(20);
    circleLayout->setContentsMargins(20, 0, 20, 0);

    // Circle image
    QLabel* circleImageLabel = new QLabel();
    QPixmap circlePixmap(circleImagePath);
    // Make the image circular
    QPixmap circularPixmap(circlePixmap.size());
    circularPixmap.fill(Qt::transparent);
    QPainter painter(&circularPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(circlePixmap));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, circlePixmap.width(), circlePixmap.height());
    painter.end();

    circleImageLabel->setPixmap(circularPixmap.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    circleImageLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    circleLayout->addWidget(circleImageLabel);

    // Text next to the circle
    QLabel* circleTextLabel = new QLabel(circleText);
    circleTextLabel->setWordWrap(true);
    circleTextLabel->setStyleSheet("font-size: 16px;");
    circleLayout->addWidget(circleTextLabel, 1); // Take remaining space

    mainLayout->addLayout(circleLayout);

    // 3. TextPage widget at the bottom
    TextPage* textPage = new TextPage(headerText, subHeaders, subTexts, this);
    mainLayout->addWidget(textPage);

    container->setLayout(mainLayout);
    scrollArea->setWidget(container);

    QVBoxLayout* outerLayout = new QVBoxLayout(this);
    outerLayout->addWidget(scrollArea);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(outerLayout);
}
