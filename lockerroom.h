#ifndef LOCKERROOM_H
#define LOCKERROOM_H

#include <QWidget>

/**
 * The LockerRoom class is for the locker room page that tells the user how to handle a locker room.
 *
 * @authors Heiffel Tower
 * @version April 24th, 2025
 */

namespace Ui {
class LockerRoom;
}

class LockerRoom : public QWidget
{
    Q_OBJECT

public:
    /**
     * Constructor for the locker room class
     * @param parent
     */
    explicit LockerRoom(QWidget *parent = nullptr);
    /**
     * Destructor for the locker room class
     */
    ~LockerRoom();

private:
    /**
     * @brief ui - A pointer to the ui of the locker room page
     */
    Ui::LockerRoom *ui;
};

#endif // LOCKERROOM_H
