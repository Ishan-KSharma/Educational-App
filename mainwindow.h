#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/**
 * This class makes the ui look professional and
 * function correctly.
 */

#include <QMainWindow>
#include "quiz.h"
#include <Box2D/Box2D.h>
#include <QTimer>
#include <QMediaPlayer>
#include <QAudioOutput>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * The Main Window of the project. THis is where both the menu on the left hand side, the stacked Widget that holds all of the
 * widgets that will be displayed, and the buttons that move back and forth between menus occurs. Contains all the logic to connect
 * all the moving pieces.
 *
 * @authors Heiffel Tower
 * @version April 24th, 2025
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * Constructor for the MainWindow class
     * @param parent
     */
    MainWindow(QWidget *parent = nullptr);
    /**
     * Destructor for the MainWindow class
     */
    ~MainWindow();

private:
    /**
     * The user interface
     */
    Ui::MainWindow *ui;
    // Animation variables
    /**
     * @brief world - The world that the balls will bounce inside of for Box2D
     */
    b2World world;
    /**
     * @brief ball - The ball that will bounce accross the screen
     */
    b2Body* ball;
    /**
     * @brief widgetSize - size of the widget
     */
    QSize widgetSize;
    /**
     * @brief animationTimer - Timer for how long the ball will bounce across the screen
     */
    QTimer animationTimer;
    /**
     * @brief originalBallImage - Image of the ball
     */
    QPixmap originalBallImage;
    /**
     * @brief player - A media player that plays the background music
     */
    QMediaPlayer* player;
    /**
     * @brief audioOutput - Outputs the audio from the background music
     */
    QAudioOutput* audioOutput;

    /**
     * Constant to hold the save path for the save file.
     */
    const QString savePath = "saveFile.txt";
    /**
     * Houses all the widgets
     */
    QVector<QWidget*> uiVector;
    /**
     * Holds the info for the basics quiz
     */
    Quiz basicsQuiz;
    /**
     * Holds the info for the formations quiz
     */
    Quiz formationsQuiz;
    /**
     * Holds the info for the man management quiz
     */
    Quiz manManagementQuiz;
    /**
     * Current index of widget being displayed
     */
    int currentIndex = 0;
    /**
     * Tells the program if the basics quiz has been passed
     */
    bool basicsQuizPassed = false;
    /**
     * Tells the program if the formations quiz has been passed
     */
    bool formationsQuizPassed = false;
    /**
     * Tells the program if the man management quiz has been passed
     */
    bool manManagementQuizPassed = false;
    /**
     * Tells the program if the final quiz has been passed
     */
    bool finalQuizPassed = false;
    /**
     * This method goes forward in the stack and calls
     * the slot to change the widget shown
     */
    void goForward();
    /**
     * This method goes back in the stack and calls
     * the slot to change the widget shown
     */
    void goBack();
    /**
     * Sets up the world for showing the forward and back button ball animations
     */
    void setupBallWorld();
    /**
     * Starts a ball bouncing animation at the given position and with given velocity
     * @param ballXStartPOS the x coordinate (in pixels) to start the animation at
     * @param ballYStartPOS the y coordinate (in pixels) to start the animation at
     * @param initBallVelocity the initial velocity of the ball
     */
    void startBallAnimation(int ballXStartPOS, int ballYStartPOS, b2Vec2 initBallVelocity);
    /**
     * Adds all the formations to make the constructor look less packed
     */
    void addAllTheFormationsPages();
    /**
     * Maps signal names to indices
     */
    QHash<QString, int> formationIndices;
    /**
     * Helper function to initialize mappings
     */
    void setupFormationMappings();
    /**
     * Helper method that initializes the quizes
     */
    void setupQuizes();
    /**
     * Helper to save the current user progress through app.
     */
    void saveProgress();
    /**
     * Loads the saved user progress.
     */
    void loadProgress();



private slots:
    /**
     * This slot opens the basics menu sub menu
     */
    void on_basicsButton_clicked();

    /**
     * This slot opens the formations sub menu
     */
    void on_formationsButton_clicked();

    /**
     * This slot opens the man management sub menu
     */
    void on_manManageButton_clicked();

    /**
     * This slot puts the getting started lesson in the lesson frame
     */
    void on_basics_gettingStarted_clicked();

    /**
     * This slot puts the players lesson in the lesson frame
     */
    void on_basics_playersButton_clicked();

    /**
     * This slot puts the out of bounds lesson in the lesson frame
     */
    void on_basics_OOB_clicked();

    /**
     * This slot goes back to the lesson before the current lesson
     */
    void on_goBack_clicked();

    /**
     * This slot goes to the next lesson if the user has them open
     */
    void on_goNext_clicked();

    /**
     * This slot goes to the basics quiz
     */
    void on_basics_quiz_clicked();

    /**
     * Handles button unlocking when basics quiz is passed
     * @param score the score the user got on the quiz
     * @param total the highest possible score to get on the quiz
     */
    void onBasicsQuizCompleted(int score, int total);

    /**
     * Handles button unlocking when formations quiz is passed
     * @param score the score the user got on the quiz
     * @param total the highest possible score to get on the quiz
     */
    void onFormationsQuizCompleted(int score, int total);

    /**
     * Handles button unlocking when man management quiz is passed
     * @param score the score the user got on the quiz
     * @param total the highest possible score to get on the quiz
     */
    void onManManagementQuizCompleted(int score, int total);

    /**
     * This slot goes to the substitution lesson in the lesson frame
     */
    void on_subsButton_clicked();

    /**
     * This slot shows the locker room lesson in the lesson frame
     */
    void on_lockerRoom_clicked();

    /**
     * This slot goes to the man management quiz in the lesson frame
     */
    void on_manManageQuizButton_clicked();

    /**
     * Handles the timeout of our timer for progressing the balls animation
     */
    void handleAnimationTimeout();

    /**
     * This slot goes to the managers and great teams page
     */
    void on_formationsPageButton_clicked();

    /**
     * This slot goes to the managers and great teams quiz
     */
    void on_formationsQuizButton_clicked();

    /**
     * This slot goes to the media training lesson
     */
    void on_mediaTrainingLesson_clicked();

    /**
     * This slot goes to the media training quiz
     */
    void on_mediaTrainingQuiz_clicked();

    /**
     * This slot opens the media training sub menu
     */
    void on_mediaTrainingButton_clicked();

    /**
     * How formation page will comunicate with mainwindow
     * @param signalName - which button was clicked
     */
    void handleFormationClicked(const QString &signalName);
    /**
     * Will re-enable the forward button once the media Training quiz is done
     */
    void mediaQuizPassed();
};
#endif // MAINWINDOW_H
