#ifndef MEDIALESSON_H
#define MEDIALESSON_H

#include <QWidget>
/**
 * The mediaLesson class is for the ui page that teaches users about how to handle the media.
 *
 * @authors Heiffel Tower
 * @version April 24th, 2025
 */

namespace Ui {
class mediaLesson;
}

class mediaLesson : public QWidget
{
    Q_OBJECT

public:
    /**
     * Constructor for the mediaLesson class
     * @param parent
     */
    explicit mediaLesson(QWidget *parent = nullptr);
    /**
     * Destructor for the mediaLesson class
     */
    ~mediaLesson();

private:
    /**
     * @brief ui - the user interactive ui
     */
    Ui::mediaLesson *ui;
};

#endif // MEDIALESSON_H
