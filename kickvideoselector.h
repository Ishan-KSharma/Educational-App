#ifndef KICKVIDEOSELECTOR_H
#define KICKVIDEOSELECTOR_H

#include <QDialog>
/**
 * This class represents the popup that shows up represeting the options two select
 * between two different videos.
 *
 * @author: Rishabh Saini
 * @version:  April 24th, 2025
 */
namespace Ui {
class KickVideoSelector;
}

class KickVideoSelector : public QDialog
{
    Q_OBJECT

public:
    explicit KickVideoSelector(QWidget *parent = nullptr);
    ~KickVideoSelector();

private:
    Ui::KickVideoSelector *ui;

public slots:

    /**
     * Plays the corner kick video
     */
    void playCornerKickVideo();

    /**
     * Plays the goal kick video.
     */
    void playGoalKickVideo();
};

#endif // KICKVIDEOSELECTOR_H
