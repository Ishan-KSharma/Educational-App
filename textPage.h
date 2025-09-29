#ifndef TEXTPAGE_H
#define TEXTPAGE_H

#include <QWidget>
#include <QString>
#include <QVector>

/**
 * This class creates and auto formats the page fo the text for the user
 *
 * @author John Laird
 * @version 24th April 2025
 */

class TextPage : public QWidget{
    Q_OBJECT
public:
    /**
     * Creates the Page of text and fills it with all the content needed. NOTE: The subHeaders and subTexts vectors
     * must be of the same length, or not all text will display
     * @param headerText - The text of the large header at the top of the page
     * @param subHeaders - A QVector containing all the subheaders, in the order you want them to appear
     * @param subTexts - A QVector containing all the text that will appear under
     * the subheader with the same index
     * @param parent
     */
    explicit TextPage(const QString& headerText,
                      const QVector<QString>& subHeaders,
                      const QVector<QString>& subTexts,
                      QWidget* parent = nullptr);

private:
    /**
     * Sets up the layout of the page on the screen
     * @param headerText - The text of the large header at the top of the page
     * @param subHeaders - A QVector containing all the subheaders, in the order you want them to appear
     * @param subTexts - A QVector containing all the text that will appear under
     * the subheader with the same index
     */
    void setupLayout(const QString& headerText,
                     const QVector<QString>& subHeaders,
                     const QVector<QString>& subTexts);
};

#endif // TEXTPAGE_H
