/**
 * Template class to make a formation. It contains an image at the top as a banner,
 * a circular image, a text box, and a text page
 * @author Ishan Sharma
 * @date 4/24/2025
 */
#ifndef FORMATIONPAGE_H
#define FORMATIONPAGE_H

#include <QWidget>
#include <QVector>
#include <QString>
#include <QPainter>

class Formation : public QWidget {
    Q_OBJECT
public:
    /**
     * Gets all the strings and images needed for the picture
     * @param formationImagePath - Image for the top banner
     * @param circleImagePath - Image for the circle
     * @param circleText - Text that goes to the right of the circle
     * @param headerText - Header that goes to the top of the text page
     * @param subHeaders - Text page's sub headers
     * @param subTexts - Sub texts that go with those headers
     * @param parent
     */
    Formation(const QString& formationImagePath,
                  const QString& circleImagePath,
                  const QString& circleText,
                  const QString& headerText,
                  const QVector<QString>& subHeaders,
                  const QVector<QString>& subTexts,
                  QWidget* parent = nullptr);

    /**
     * Constructor that calls the other constructor using default sub headers for the formations section
     * @param formationImagePath - Image for the top banner
     * @param circleImagePath - Image for the circle
     * @param circleText - Text that goes to the right of the circle
     * @param headerText - Header that goes to the top of the text page
     * @param subTexts - Sub texts that go with those headers
     * @param parent
     */
    Formation(const QString& formationImagePath,
              const QString& circleImagePath,
              const QString& circleText,
              const QString& headerText,
              const QVector<QString>& subTexts,
              QWidget* parent = nullptr);

private:
    /**
     * @brief setupLayout
     * @param formationImagePath - puts the image at the top of the screen
     * @param circleImagePath - puts the picture inside a circle
     * @param circleText - puts text next to that circle
     * @param headerText - puts this text at the top
     * @param subHeaders - puts this text as headers
     * @param subTexts - puts this text under those headers
     */
    void setupLayout(const QString& formationImagePath,
                     const QString& circleImagePath,
                     const QString& circleText,
                     const QString& headerText,
                     const QVector<QString>& subHeaders,
                     const QVector<QString>& subTexts);
};

#endif // FORMATIONPAGE_H
