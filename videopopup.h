#ifndef VIDEOPOPUP_H
#define VIDEOPOPUP_H

#include <QDialog>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QVBoxLayout>

/**
 * This class creates a pop-up that plays a video from a passed in video URL.
 *
 * @author Rishabh Saini
 * @version 4/22/25
 */
namespace Ui {
class VideoPopUp;
}

class VideoPopUp : public QDialog
{
    Q_OBJECT

public:
    /**
     * Constructor for the VideoPopUp.
     * @param videoPath the URL of the video to play.
     */
    explicit VideoPopUp(QString videoURL, QWidget *parent = nullptr);
    ~VideoPopUp();

private:
    Ui::VideoPopUp *ui;
    QString videoToPlay;
    QMediaPlayer* player;
    QVideoWidget* videoWidget;
};

#endif // VIDEOPOPUP_H
