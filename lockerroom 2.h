#ifndef LOCKERROOM_H
#define LOCKERROOM_H

#include <QWidget>

namespace Ui {
class LockerRoom;
}

class LockerRoom : public QWidget
{
    Q_OBJECT

public:
    explicit LockerRoom(QWidget *parent = nullptr);
    ~LockerRoom();

private:
    Ui::LockerRoom *ui;
};

#endif // LOCKERROOM_H
