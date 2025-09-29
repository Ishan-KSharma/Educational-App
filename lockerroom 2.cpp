#include "lockerroom.h"
#include "ui_lockerroom.h"

LockerRoom::LockerRoom(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LockerRoom)
{
    ui->setupUi(this);
}

LockerRoom::~LockerRoom()
{
    delete ui;
}
