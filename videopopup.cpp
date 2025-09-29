#include "videopopup.h"
#include "ui_videopopup.h"

VideoPopUp::VideoPopUp(QString videoURL, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::VideoPopUp)
{
    ui->setupUi(this);
    setWindowTitle("Out of bounds video");
     setWindowIcon(QIcon(":/videos/Images/ball.png"));
    videoToPlay = videoURL;

    //Instantiates the video player and the video widget
    player = new QMediaPlayer(this);
    videoWidget = new QVideoWidget(this);

    //connects video player to video widget which allows the widget to display the video the player is playing.
    player->setVideoOutput(videoWidget);

    //sets what video the video player should play
    player->setSource(QUrl(videoToPlay));

    // sets the layout for the widget, ensuring the video widget is properly displayed and managed in the dialog.
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(videoWidget);
    setLayout(layout);

    videoWidget->show();

    // Set the video to repeat indefinitely
    player->setLoops(QMediaPlayer::Infinite);

    player->play();
}

VideoPopUp::~VideoPopUp()
{
    delete ui;
    delete player;
    delete videoWidget;
}
