#include "kickvideoselector.h"
#include "ui_kickvideoselector.h"
#include "videopopup.h"

KickVideoSelector::KickVideoSelector(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::KickVideoSelector)
{
    ui->setupUi(this);
    connect(ui->cornerKickButton, &QPushButton::clicked, this, &KickVideoSelector::playCornerKickVideo);
    connect(ui->goalKickButton, &QPushButton::clicked, this, &KickVideoSelector::playGoalKickVideo);
}

KickVideoSelector::~KickVideoSelector()
{
    delete ui;
}

void KickVideoSelector::playCornerKickVideo() {
    VideoPopUp v( "qrc:/videos/Images/cornerKick.mp4");
    v.exec();
}

void KickVideoSelector::playGoalKickVideo() {
    VideoPopUp v("qrc:/new/prefix1/Images/goalKick.mp4");
    v.exec();
}
