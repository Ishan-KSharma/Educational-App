#include "outofbounds.h"
#include "ui_outofbounds.h"
#include "videopopup.h"
#include "kickvideoselector.h"

OutOfBounds::OutOfBounds(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::outofbounds)
{
    ui->setupUi(this);

    //connects all the buttons to their respective ball out of bounds videos.
    connect(ui->behindGoalOutOfBounds1, &QPushButton::clicked, this, &OutOfBounds::playGoalLineVideos);
    connect(ui->behindGoalOutOfBounds2, &QPushButton::clicked, this, &OutOfBounds::playGoalLineVideos);
    connect(ui->sideLineOutOfBounds1, &QPushButton::clicked, this, &OutOfBounds::playThrowInVideo);
    connect(ui->sideLineOutOfBounds2, &QPushButton::clicked, this, &OutOfBounds::playThrowInVideo);
}

OutOfBounds::~OutOfBounds()
{
    delete ui;
}

void OutOfBounds::playGoalLineVideos() {
    KickVideoSelector k;
    k.exec();
}

void OutOfBounds::playThrowInVideo() {
    VideoPopUp v( "qrc:/videos/Images/throwIn.mp4");
    v.exec();
}

