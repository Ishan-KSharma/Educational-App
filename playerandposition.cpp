#include "playerandposition.h"
#include "ui_playerandposition.h"

PlayerandPosition::PlayerandPosition(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PlayerandPosition)
{
    ui->setupUi(this);
    QPixmap pixmap(":/new/prefix1/Images/soccerField1.jpg");
    ui->soccerField->setPixmap(pixmap);
    ui->soccerField->setScaledContents(true);
}

PlayerandPosition::~PlayerandPosition()
{
    delete ui;
}
