#ifndef WELCOMEDIALOG_H
#define WELCOMEDIALOG_H

#include <QDialog>
#include <Box2D/Box2D.h>
#include <QTimer>

namespace Ui {
class WelcomeDialog;
}

/**
 * Represents a welcome dialog popup for the Soccer Tactics Teacher,
 * with Box2D used for animating the soccer balls.
 * @author Danny Provo
 * @date 4/7/2025
 */
class WelcomeDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::WelcomeDialog *ui;
    b2World world;
    b2Body* ball1;
    b2Body* ball2;
    QSize widgetSize;
    QTimer timer;

private slots:
    /**
     * Handles the timeout of our timer for progressing the balls animation
     */
    void handleAnimationTimeout();

public:
    explicit WelcomeDialog(QWidget *parent = nullptr);
    ~WelcomeDialog();

};

#endif // WELCOMEDIALOG_H
