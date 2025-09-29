#ifndef OUTOFBOUNDS_H
#define OUTOFBOUNDS_H

#include <QWidget>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QVBoxLayout>

/**
 * This class represents the "Ball Out Of Bounds" module for the SoccerTacticsTeacher.
 *
 * @author Rishabh Saini
 * @version TODO
 */
namespace Ui {
class outofbounds;
}

class OutOfBounds : public QWidget
{
    Q_OBJECT

public:
    explicit OutOfBounds(QWidget *parent = nullptr);
    ~OutOfBounds();

private:
    Ui::outofbounds *ui;
    QMediaPlayer* player;
    QVideoWidget* videoWidget;

public slots:
    /**
     * Plays the video demonstrating a corner kick.
     */
    void playGoalLineVideos();

    /**
     * Plays the video demonstrating a throw in.
     */
    void playThrowInVideo();

};

#endif // OUTOFBOUNDS_H
