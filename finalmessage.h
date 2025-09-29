/**
 * Final screen is a coaching license, this handles that entire screen
 * @author Ishan Sharma
 * @date 4/24/2025
 */

#ifndef FINALMESSAGE_H
#define FINALMESSAGE_H

#include <QDialog>

namespace Ui {
class FinalMessage;
}

class FinalMessage : public QDialog
{
    Q_OBJECT

public:
    /**
     * Constructor
     */
    explicit FinalMessage(QWidget *parent = nullptr);
    /**
     * Destructor
     */
    ~FinalMessage();

private slots:
    /**
     * Updates the name displayed on the page
     */
    void on_submitButton_clicked();

private:
    /**
     * UI
     */
    Ui::FinalMessage *ui;
};

#endif // FINALMESSAGE_H
