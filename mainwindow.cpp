#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "playerandposition.h"
#include "quizwidget.h"
#include "textpage.h"
#include "QVector"
#include "outofbounds.h"
#include "substitutions.h"
#include "lockerroom.h"
#include <Qdebug>
#include <QMessageBox>
#include <Box2D/Box2D.h>
#include "helpers.h"
#include <QPainter>
#include "formationsPage.h"
#include "formation.h"
#include "finalMessage.h"
#include <QFile>
#include <QDir>
#include "mediatraining.h"
#include "medialesson.h"
#include <QJsonArray>
#include <QJsonDocument>

using Box2DHelpers::pixelXToWorldX;
using Box2DHelpers::pixelYToWorldY;
using Box2DHelpers::worldXToPixelX;
using Box2DHelpers::worldYToPixelY;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , world(b2Vec2(0.0f, -10.0f)), animationTimer(QTimer(this))
    , currentIndex(0)
{
    ui->setupUi(this);

    setWindowTitle("Soccer Tactics Teacher");
    setWindowIcon(QIcon(":/videos/Images/ball.png"));
    // Setup animation stuff
    QPixmap preScaledImage = QPixmap(":/new/prefix1/Images/ball.png");
    originalBallImage = preScaledImage.scaled(ui->ball->width(), ui->ball->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->ball->setPixmap(originalBallImage);
    ui->ball->setScaledContents(false);
    ui->ball->setVisible(false);
    connect(&animationTimer, &QTimer::timeout, this, &MainWindow::handleAnimationTimeout);
    setupBallWorld();

    QString basicLogistics = "The field is required to be between 110 and 120 yards long and 70 to 80 yards in width. "
                             "The normal game length is 90 minutes, split between 2 halves that are 45 minutes each. "
                             "Each half, the referee can add additional time on the end equivalent to the time lost during "
                             "regular play for injuries or VAR(video assistant referee) reviews. In the event of overtime, "
                             "there are two halves, 15 minutes each, followed by a penalty shootout if needed.";

    QString refereesAndSubs = "Each game will have 6 referees. The three on the field are the center referee and two assistant "
                              "referees. The center referee is responsible for calling fouls, handing out cards, stopping and "
                              "restarting the play for substitutions, handling the game time, and determining the amount of "
                              "extra time each half will have. The assistant referees, also known as linesmen, are responsible "
                              "for informing the referee which team has the ball after it goes out of bounds and calling offside. "
                              "The 4th referee stands on the sideline between the two teams' benches and controls the two benches "
                              "when tempers flare and holding up the substitution sign when needed. They also replace the on-field "
                              "referees in the case of an injury. The 5th and 6th referees handle the VAR(video assistant referee) "
                              "aspect of the game. They review crucial calls made by the center referee and have him look over his "
                              "call again if they believe he made an error when calling his foul.\n \n Each team is allowed up to "
                              "5 substitutes in 3 different substitution windows, meaning they can make up to 5 subs during at most"
                              " 3 different points in the game. If you do a substitution during halftime, it does not count as using "
                              "a substitution window. If the game goes into overtime, each team gets an additional substitution and substitution window ";

    QString cardsFoulsSuspensions = "During the game, players can receive yellow or red cards for severe fouls committed during the game. "
                                    "If a player gets a yellow card, they can continue to play. However, if they receive a second yellow "
                                    "card, it is equivalent to a red card, and they are forced to leave the field, and their team must now "
                                    "play with 10 players. In league play, if a player receives a red card via two yellow cards, they are "
                                    "suspended for 1 game following this match. If they receive a straight red card, they are suspended for "
                                    "the 3 following games. In tournament play, if a player gets two yellow cards or a red card, they are "
                                    "banned for just the following game. Furthermore, in tournament play, yellow cards carry over to the next"
                                    " match, meaning that if players pick up 1 yellow card in 2 back to back-to-back matches, they are "
                                    "suspended for the next game but they do not have to leave the team down to 10 players.";

    // Add all the basics ui widgets
    uiVector.append(new TextPage(QString("Basics"), QVector<QString>({"Basic Logistics","Referees and Substitutions", "Cards, fouls, and suspensions"}),
                                 QVector<QString>({basicLogistics, refereesAndSubs, cardsFoulsSuspensions})));

    uiVector.append(new PlayerandPosition(this));
    uiVector.append(new OutOfBounds(this));

    setupQuizes();

    // Create basics quiz and add it to the ui widgets
    QuizWidget* basicsQuizWidget = new QuizWidget(&basicsQuiz, this);
    connect(basicsQuizWidget, &QuizWidget::quizCompleted, this, &MainWindow::onBasicsQuizCompleted);
    basicsQuizWidget->show();
    uiVector.append(basicsQuizWidget);
    basicsQuizPassed = false;

    //Add all the formations ui widgets
    FormationsPage* formationsPage = new FormationsPage(this);
    uiVector.append(formationsPage);

    setupFormationMappings();
    addAllTheFormationsPages();

    //Connect the formations buttons to the mainwindow
    connect(formationsPage, &FormationsPage::fergusonClicked, this, [this]() { handleFormationClicked("fergusonClicked"); });
    connect(formationsPage, &FormationsPage::kloppClicked, this, [this]() { handleFormationClicked("kloppClicked"); });
    connect(formationsPage, &FormationsPage::guardiolaClicked, this, [this]() { handleFormationClicked("guardiolaClicked"); });
    connect(formationsPage, &FormationsPage::ancelottiClicked, this, [this]() { handleFormationClicked("ancelottiClicked"); });
    connect(formationsPage, &FormationsPage::mourinhoClicked, this, [this]() { handleFormationClicked("mourinhoClicked"); });
    connect(formationsPage, &FormationsPage::simeoneClicked, this, [this]() { handleFormationClicked("simeoneClicked"); });
    connect(formationsPage, &FormationsPage::cruyffClicked, this, [this]() { handleFormationClicked("cruyffClicked"); });
    connect(formationsPage, &FormationsPage::zidaneClicked, this, [this]() { handleFormationClicked("zidaneClicked"); });
    connect(formationsPage, &FormationsPage::alonsoClicked, this, [this]() { handleFormationClicked("alonsoClicked"); });
    connect(formationsPage, &FormationsPage::conteClicked, this, [this]() { handleFormationClicked("conteClicked"); });

    connect(formationsPage, &FormationsPage::firstBarcaClicked, this, [this]() { handleFormationClicked("firstBarcaClicked"); });
    connect(formationsPage, &FormationsPage::madridClicked, this, [this]() { handleFormationClicked("madridClicked"); });
    connect(formationsPage, &FormationsPage::interClicked, this, [this]() { handleFormationClicked("interClicked"); });
    connect(formationsPage, &FormationsPage::ajaxClicked, this, [this]() { handleFormationClicked("ajaxClicked"); });
    connect(formationsPage, &FormationsPage::liverpoolClicked, this, [this]() { handleFormationClicked("liverpoolClicked"); });
    connect(formationsPage, &FormationsPage::leicesterClicked, this, [this]() { handleFormationClicked("leicesterClicked"); });
    connect(formationsPage, &FormationsPage::secondBarcaClicked, this, [this]() { handleFormationClicked("secondBarcaClicked"); });

    // Add all the man management ui widgets
    uiVector.append(new Substitutions(this));
    uiVector.append(new LockerRoom(this));

    // Create man management quiz and add it to the ui widgets
    QuizWidget* manManagementQuizWidget = new QuizWidget(&manManagementQuiz, this);
    connect(manManagementQuizWidget, &QuizWidget::quizCompleted, this, &MainWindow::onManManagementQuizCompleted);
    manManagementQuizWidget->show();
    uiVector.append(manManagementQuizWidget);
    manManagementQuizPassed = false;

    // Do all the necessary work for mediaTraining module
    MediaTraining* mediaTraining = new MediaTraining(this);
    uiVector.append(new mediaLesson(this));
    uiVector.append(mediaTraining);
    connect(mediaTraining, &MediaTraining::mediaQuizPassed, this, &MainWindow::mediaQuizPassed);

    // Starting state for all the menu buttons
    ui->basicsSubMenu->setVisible(false);
    ui->formationsSubMenu->setVisible(false);
    ui->formationsButton->setEnabled(false);
    ui->manManageSubMenu->setVisible(false);
    ui->manManageButton->setEnabled(false);
    ui->mediaTrainingSubMenu->setVisible(false);
    ui->mediaTrainingButton->setEnabled(false);

    uiVector.append(new FinalMessage());
    //MediaTraining* mediaTraining = new MediaTraining(this);


    // Add all ui widgets to the stacked widget, to easily cycle through.
    for (QWidget* page : uiVector) {
        ui->stackedWidget->addWidget(page);
    }

    // Start on the basics lesson
    ui->stackedWidget->setCurrentIndex(0);

    //Setting up the components for playing the audio
    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    player->setAudioOutput(audioOutput);

    //fetching the bg music audio file
    player->setSource(QUrl("qrc:/videos/Images/wakaWakaBgMusic.mp3"));

    audioOutput->setVolume(0.2f);  // range is 0–100
    player->play(); //play audio

    player->setLoops(QMediaPlayer::Infinite); //loop audio infinitely

    // Load previous data (if any)
    loadProgress();

    //setCentralWidget(mediaTraining); // TODO remove this?
}

void MainWindow::setupBallWorld() {
    // Create the ground Body
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(70.0f, 0.0f);
    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    b2Body* groundBody = world.CreateBody(&groundBodyDef);
    // Define the ground box shape.
    b2PolygonShape groundBox;
    // The extents are the half-widths of the box.
    groundBox.SetAsBox(70.0f, 10.0f);
    // Add the ground fixture to the ground body.
    groundBody->CreateFixture(&groundBox, 0.0f);

    // Get the size of this widget for use in calculations converting from pixel to world coordinates
    widgetSize = this->size();

    // Define the fixuture that the balls will use
    // Define another box shape for our dynamic body.
    b2CircleShape circle;
    circle.m_radius = pixelXToWorldX(ui->ball->width()); // Ball widths are same as height and equal to each other
    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circle;
    // Set the ball density to be non-zero, so it will be dynamic.
    fixtureDef.density = 1.0f;
    // Override the default friction.
    fixtureDef.friction = 2.0f;
    // Set restitution (bounciness)
    fixtureDef.restitution = 0.55f;

    // Create ball 1
    b2BodyDef body1Def;
    body1Def.type = b2_dynamicBody;
    // Set the position based on its positon in ui
    QPoint initialPOS = ui->ball->pos();
    float ball1X = pixelXToWorldX(initialPOS.x());
    float ball1Y = pixelYToWorldY(initialPOS.y(), widgetSize.height());
    body1Def.position.Set(ball1X, ball1Y);
    ball = world.CreateBody(&body1Def);

    // Add the ball shape to the balls.
    ball->CreateFixture(&fixtureDef);
}

void MainWindow::loadProgress() {
    QFile file(savePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // If error occurs, simply return and use default progress
        return;
    }

    // Try to load file data into a QJsonDocument
    QByteArray loadedData = file.readAll();
    file.close();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(loadedData);
    if(jsonDoc.isNull()) {
        return; // If error occurs, no loading will occur
    }
    QJsonArray unlockedModules = jsonDoc.array();
    if(unlockedModules.contains(QJsonValue("Formations"))) {
        basicsQuizPassed = true;
        ui->formationsButton->setEnabled(true);
    }
    if(unlockedModules.contains(QJsonValue("Man Management"))) {
        formationsQuizPassed = true;
        ui->manManageButton->setEnabled(true);
    }
    if(unlockedModules.contains(QJsonValue("Media Training"))) {
        manManagementQuizPassed = true;
        ui->mediaTrainingButton->setEnabled(true);
    }
    if(unlockedModules.contains(QJsonValue("Press Training"))) {
        finalQuizPassed = true;
    }
}

MainWindow::~MainWindow()
{
    // save user progress before exiting
    saveProgress();

    qDeleteAll(uiVector);
    delete ui;
    delete player;
    delete audioOutput;
}

void MainWindow::saveProgress() {
    QFile file(savePath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        // Just return if error occurs, we want process to be invisible to user
        return;
    }

    // Get data to write to file
    QJsonArray unlockedModules;
    unlockedModules.append(QJsonValue("Basics")); // Always unlocked
    if(ui->formationsButton->isEnabled())
        unlockedModules.append(QJsonValue("Formations"));
    if(ui->manManageButton->isEnabled())
        unlockedModules.append(QJsonValue("Man Management"));
    if(ui->mediaTrainingButton->isEnabled())
        unlockedModules.append(QJsonValue("Media Training"));
    if(finalQuizPassed)
        unlockedModules.append(QJsonValue("Press Training"));

    // Convert to json and write to file
    QJsonDocument jsonDoc(unlockedModules);
    QByteArray dataToSave = jsonDoc.toJson();
    QTextStream outputWriter(&file);
    outputWriter << dataToSave;
    file.close();
}

// Go forward or back buttons
void MainWindow::goForward() {
    if (currentIndex + 1 < uiVector.size()) {
        ++currentIndex;
        ui->stackedWidget->setCurrentIndex(currentIndex);
    }
    currentIndex = ui->stackedWidget->currentIndex(); // sync

    bool disableBasicsNext = currentIndex == 3 && !basicsQuizPassed;
    bool disableFormationsNext = currentIndex == 22 && !formationsQuizPassed;
    bool disableManManageNext = currentIndex == 25 && !manManagementQuizPassed;
    bool disableMediaTrainingNext = currentIndex == 27 && !finalQuizPassed;
    bool onFinalIndex = currentIndex == 28;
    if (disableBasicsNext || disableFormationsNext || disableManManageNext || disableMediaTrainingNext || onFinalIndex) {
        ui->goNext->setEnabled(false);
    }
}

void MainWindow::goBack() {
    if (currentIndex > 0) {
        --currentIndex;
        ui->stackedWidget->setCurrentIndex(currentIndex);
    }
}

// Seeing the sub menus of all the modules
void MainWindow::on_basicsButton_clicked() {
    bool currentlyVisible = ui->basicsSubMenu->isVisible();
    ui->basicsSubMenu->setVisible(!currentlyVisible);
}

void MainWindow::on_formationsButton_clicked() {
    bool currentlyVisible = ui->formationsSubMenu->isVisible();
    ui->formationsSubMenu->setVisible(!currentlyVisible);
}

void MainWindow::on_manManageButton_clicked() {
    bool currentlyVisible = ui->manManageSubMenu->isVisible();
    ui->manManageSubMenu->setVisible(!currentlyVisible);
}

void MainWindow::on_mediaTrainingButton_clicked() {
    bool currentlyVisible = ui->mediaTrainingSubMenu->isVisible();
    ui->mediaTrainingSubMenu->setVisible(!currentlyVisible);
}

// Setting the current page based on what button is clicked
void MainWindow::on_basics_gettingStarted_clicked() {
    currentIndex = 0;
    ui->stackedWidget->setCurrentIndex(0);
    ui->goNext->setEnabled(true);
}

void MainWindow::on_basics_playersButton_clicked() {
    currentIndex = 1;
    ui->stackedWidget->setCurrentIndex(currentIndex);
    ui->goNext->setEnabled(true);
}

void MainWindow::on_basics_OOB_clicked() {
    currentIndex = 2;
    ui->stackedWidget->setCurrentIndex(currentIndex);
    ui->goNext->setEnabled(true);
}

void MainWindow::on_basics_quiz_clicked() {
    currentIndex = 3;
    basicsQuiz.shuffle();
    ui->stackedWidget->setCurrentIndex(currentIndex);
    if(basicsQuizPassed)
        ui->goNext->setEnabled(true);
    else
        ui->goNext->setEnabled(false);
}

void MainWindow::on_formationsQuizButton_clicked() {
    qDebug() << currentIndex; // TODO remove
    currentIndex = 22;
    formationsQuiz.shuffle();
    ui->stackedWidget->setCurrentIndex(currentIndex);
    if(formationsQuizPassed)
        ui->goNext->setEnabled(true);
    else
        ui->goNext->setEnabled(false);
}

void MainWindow::on_manManageQuizButton_clicked() {
    currentIndex = 25;
    manManagementQuiz.shuffle();
    ui->stackedWidget->setCurrentIndex(currentIndex);
    if(manManagementQuizPassed)
        ui->goNext->setEnabled(true);
    else
        ui->goNext->setEnabled(false);
}

void MainWindow::on_mediaTrainingQuiz_clicked() {
    currentIndex = 27;
    ui->stackedWidget->setCurrentIndex(currentIndex);
}

void MainWindow::on_goBack_clicked() {
    goBack();
    ui->goNext->setEnabled(true);

    startBallAnimation(widgetSize.width() + 50, widgetSize.height() - 200, b2Vec2(-60, 0));
}

void MainWindow::on_goNext_clicked() {
    goForward();

    startBallAnimation(100, widgetSize.height() - 200, b2Vec2(60, 0));
}

void MainWindow::startBallAnimation(int ballXStartPOS, int ballYStartPOS, b2Vec2 initBallVelocity) {
    // Move the ball to the specified equivalent within the world
    b2Vec2 ballStartPOS(pixelXToWorldX(ballXStartPOS), pixelYToWorldY(ballYStartPOS, widgetSize.height()));
    ball->SetTransform(ballStartPOS, ball->GetAngle());

    // Set the velocity and angular velocity of the ball
    ball->SetLinearVelocity(initBallVelocity);
    ball->SetAngularVelocity(0);

    // Update ui to show ball and start animation timer
    ui->ball->move(ballXStartPOS, ballYStartPOS);
    ui->ball->setVisible(true);
    animationTimer.start(10);
}

void MainWindow::on_formationsPageButton_clicked(){
    currentIndex = 4;
    ui->stackedWidget->setCurrentIndex(currentIndex);
    ui->goNext->setEnabled(true);
}

void MainWindow::on_subsButton_clicked() {
    currentIndex = 23;
    ui->stackedWidget->setCurrentIndex(currentIndex);
    ui->goNext->setEnabled(true);
}

void MainWindow::on_lockerRoom_clicked() {
    currentIndex = 24;
    ui->stackedWidget->setCurrentIndex(currentIndex);
    ui->goNext->setEnabled(true);
}

void MainWindow::on_mediaTrainingLesson_clicked() {
    currentIndex = 26;
    ui->stackedWidget->setCurrentIndex(currentIndex);
    ui->goNext->setEnabled(true);
}



void MainWindow::onBasicsQuizCompleted(int score, int total) {
    if (static_cast<double>(score) / total >= 0.8) {
        basicsQuizPassed = true;
        ui->formationsButton->setEnabled(true);
        ui->goNext->setEnabled(true);
        QMessageBox::information(this, "Quiz Passed", "Congratulations! You've unlocked the Formations section.");
    } else {
        QMessageBox::warning(this, "Quiz Not Passed", "You need to score at least 80% to unlock the next section.");
        basicsQuiz.shuffle();
        QuizWidget* basicsQuizWidget = new QuizWidget(&basicsQuiz, this);
        connect(basicsQuizWidget, &QuizWidget::quizCompleted, this, &MainWindow::onBasicsQuizCompleted);
        ui->stackedWidget->removeWidget(uiVector[3]);
        ui->stackedWidget->insertWidget(3, basicsQuizWidget);

        basicsQuizPassed = false;
        ui->stackedWidget->setCurrentIndex(3);
    }
}
void MainWindow::onFormationsQuizCompleted(int score, int total) {
    if (static_cast<double>(score) / total >= 0.8) {
        formationsQuizPassed = true;
        ui->manManageButton->setEnabled(true);
        ui->goNext->setEnabled(true);
        QMessageBox::information(this, "Quiz Passed", "Congratulations! You've unlocked the Man Management section.");
    } else {
        QMessageBox::warning(this, "Quiz Not Passed", "You need to score at least 80% to unlock the next section.");
        formationsQuiz.shuffle();
        QuizWidget* formationsQuizWidget = new QuizWidget(&formationsQuiz, this);
        connect(formationsQuizWidget, &QuizWidget::quizCompleted, this, &MainWindow::onFormationsQuizCompleted);
        ui->stackedWidget->removeWidget(uiVector[22]);
        ui->stackedWidget->insertWidget(22, formationsQuizWidget);
        formationsQuizPassed = false;
        ui->stackedWidget->setCurrentIndex(22);
    }
}
void MainWindow::onManManagementQuizCompleted(int score, int total) {
    if (static_cast<double>(score) / total >= 0.8) {
        manManagementQuizPassed = true;
        ui->mediaTrainingButton->setEnabled(true);
        ui->goNext->setEnabled(true);
        QMessageBox::information(this, "Quiz Passed", "Congratulations! You've unlocked the Media Training section.");
    } else {
        QMessageBox::warning(this, "Quiz Not Passed", "You need to score at least 80% to unlock the next section.");
        manManagementQuiz.shuffle();
        QuizWidget* manManagementQuizWidget = new QuizWidget(&manManagementQuiz, this);
        connect(manManagementQuizWidget, &QuizWidget::quizCompleted, this, &MainWindow::onManManagementQuizCompleted);
        ui->stackedWidget->removeWidget(uiVector[25]);
        ui->stackedWidget->insertWidget(25, manManagementQuizWidget);
        manManagementQuizPassed = false;
        ui->stackedWidget->setCurrentIndex(25);
    }
}

void MainWindow::handleAnimationTimeout() {

    // Prepare for simulation. Typically we use a time step of 1/60 of a
    // second (60Hz) and 10 iterations. This provides a high quality simulation
    // in most game scenarios.
    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    // Instruct the world to perform a single step of simulation.
    // It is generally best to keep the time step and iterations fixed.
    world.Step(timeStep, velocityIterations, positionIterations);

    // update angle
    float32 angle = ball->GetAngle();
    QTransform rotation;
    rotation.rotateRadians(-angle); // negate angle because coordinate system is upside down
    QPixmap rotatedImage = originalBallImage.transformed(rotation);
    ui->ball->setPixmap(rotatedImage);
    ui->ball->setFixedSize(rotatedImage.size());

    // Now set the position of the ball to the new positon
    b2Vec2 ballPOS = ball->GetPosition();
    int ballPixelXPOS = worldXToPixelX(ballPOS.x);
    int ballPixelYPOS = worldYToPixelY(ballPOS.y, widgetSize.height());
    ui->ball->move(ballPixelXPOS - rotatedImage.width() / 2, ballPixelYPOS - rotatedImage.height() / 2); // repositioning based on center of image (to account for rotation jittering)

    // Stop animation once ball is off screen (plus some wiggle room)
    if(ballPixelXPOS < - 75 || ballPixelXPOS > widgetSize.width() + 75) {
        animationTimer.stop(); // Don't keep animating after ball leaves screen
        ui->ball->setVisible(false);
    }
}

void MainWindow::setupFormationMappings() {
    formationIndices = {
        {"fergusonClicked", 5},
        {"kloppClicked", 6},
        {"guardiolaClicked", 7},
        {"ancelottiClicked", 8},
        {"mourinhoClicked", 9},
        {"simeoneClicked", 10},
        {"cruyffClicked", 11},
        {"zidaneClicked", 12},
        {"alonsoClicked", 13},
        {"conteClicked", 14},
        {"firstBarcaClicked", 15},
        {"madridClicked", 16},
        {"interClicked", 17},
        {"ajaxClicked", 18},
        {"liverpoolClicked", 19},
        {"leicesterClicked", 20},
        {"secondBarcaClicked", 21}
    };
}

void MainWindow::handleFormationClicked(const QString &signalName) {
    currentIndex = formationIndices[signalName];
    ui->stackedWidget->setCurrentIndex(currentIndex);
    ui->goNext->setEnabled(true);
}
void MainWindow::mediaQuizPassed(){
    ui->goNext->setEnabled(true);
    finalQuizPassed = true;
}

void MainWindow::setupQuizes(){
    basicsQuiz = Quiz({
                    // Basic Gameplay Questions
                    {"How long are soccer games (regulation time)?", "90 mins", "80 mins", "70 mins", "60 mins"},
                    {"How many substitutions are a team allowed in regular time?", "5", "3", "6", "0"},
                    {"How many players are allowed on the field (maximum)?", "11", "12", "13", "9"},
                    {"Following a red card, how many games is a player suspended for in league play?", "3", "5", "10", "1"},
                    {"What is one role of the 4th referee?", "Handle substitutions", "Hand out cards", "Make offside decisions", "Recommend the referee to review a play"},
                    {"How long is each half in overtime?", "15 mins", "30 mins", "20 mins", "7 mins"},
                    {"What happens if a player receives 2 yellow cards in the same game?", "They get a red card", "They have to be replaced by another player", "They get a 20-minute suspension", "They get a 20-game suspension"},
                    {"What happens if a player receives a yellow card in 2 back-to-back games in a tournament?", "They're suspended for 1 game", "They're suspended for 2 games", "They get a red card during the 2nd game", "Their team has to play with 10 men in the next game"},
                    {"How many substitution windows are there (not including halftime)?", "3", "5", "2", "4"},
                    {"How many referees are on the field?", "3", "4", "5", "6"},

                    // Position Questions
                    {"What position is a 6?", "CDM", "CM", "CAM", "ST"},
                    {"What position is an 8?", "CM", "CDM", "CAM", "ST"},
                    {"What position is a 10?", "CAM", "CDM", "CM", "ST"},
                    {"What position is a 9?", "ST", "CDM", "CAM", "CM"},
                    {"Where is the goalkeeper allowed to use their hands?", "In their box", "In their half", "Anywhere as long as it was the opposing team that kicked it", "Anywhere during the first half"},
                    {"How many central defenders are typically used alongside wing backs?", "3", "2", "5", "4"},
                    {"Pick the correct order from furthest back to furthest forward.", "GK, LB, LM, LW", "GK, LB, LW, LM", "GK, LM, LB, LW", "GK, LW, LM, LB"},
                    {"Pick the correct order from most likely to score to least likely to score.", "Striker, Winger, Midfielder, Defender", "Striker, Defender, Midfielder, Winger", "Defender, Winger, Midfielder, Striker", "Midfielder, Winger, Striker, Defender"},
                    {"What number is usually reserved for goalkeepers?", "1", "4", "5", "9"},
                    {"What position could be played in a 'sweeper' style?", "GK", "CB", "CM", "ST"},

                    //Ball out of bounds questions
                    {"What happens when the ball crosses the goal line and was last touched by the attacking team, without a goal being scored?", "Goal kick", "Corner kick", "Throw-in", "Penalty kick"},
                    {"What happens when the ball crosses the goal line and was last touched by the defending team, without a goal being scored?", "Corner kick", "Goal kick", "Throw-in", "Indirect free kick"},
                    {"What type of restart is used when the ball goes out on the sideline?", "Throw-in", "Goal kick", "Corner kick", "Drop ball"},
                    {"If the ball touches the referee, what happens?", "Drop ball", "Throw in", "Free kick", "Nothing, play continues"},
                    {"If the ball crosses the touchline in the air but curves back in without touching the ground, what happens?", "Throw-in", "Play continues", "Goal kick", "Drop ball"}
          }, 10);
    formationsQuiz = Quiz({
                  {"Which team managed by Pep Guardiola is considered by many as the greatest of all time?", "2008-09 Barcelona", "2010-11 Barcelona", "2012-13 Bayern Munich", "2018-19 Manchester City"},
                  {"What was unique about the 2008-09 Barcelona team's trophy haul?", "They won all six major trophies in a single season.", "They won the Champions League without conceding a goal.", "They were the first Spanish team to win a treble.", "They scored the most goals in La Liga history."},
                  {"How many goals did Barcelona score across all competitions in the 2008-09 season?", "158", "128", "145", "172"},
                  {"What was the scoreline of the historic El Clásico win for Barcelona in the 2008-09 season?", "6-2", "5-0", "4-1", "3-0"},
                  {"Which two individuals are considered to have had a breakout season with the 2008-09 Barcelona team?", "Messi and Pep Guardiola", "Ronaldinho and Rijkaard", "Xavi and Iniesta", "Eto'o and Henry"},
                  {"What tactical system implemented by Pep Guardiola was a key to the 2008-09 Barcelona's success?", "Positional Play", "Catenaccio", "Total Football", "Gegenpressing"},
                  {"What geometric shape did Pep Guardiola emphasize in his positional play system?", "Triangle", "Square", "Pentagon", "Circle"},
                  {"According to Thierry Henry's anecdote, what happened when he drifted out of position and scored?", "He was substituted off.", "He was praised by Pep Guardiola.", "He received a yellow card.", "The goal was disallowed."},
                  {"In the 2008-09 Barcelona's formation, what was the role of Messi?", "False 9", "Out and out striker", "Right winger", "Attacking midfielder"},
                  {"Which player did Magnus Carlsen compare Pep Guardiola's system to?", "His own playing style", "Garry Kasparov", "Bobby Fischer", "Anatoly Karpov"},
                  {"Who were the central defenders for Barcelona in the 2008-09 season?", "Pique and Puyol", "Puyol and Marquez", "Abidal and Alves", "Milito and Chivu"},
                  {"Which player often played as a defensive midfielder (number 6) for the 2008-09 Barcelona team?", "Busquets", "Xavi", "Iniesta", "Deco"},
                  {"Who were the three midfielders often starting for Barcelona in the 2008-09 season?", "Xavi, Iniesta, Busquets", "Busquets, Mascherano, Keita", "Deco, Xavi, Iniesta", "Alonso, Fabregas, Thiago"},
                  {"Who made up Barcelona's front three in the 2008-09 season?", "Henry, Messi, Eto'o", "Messi, Suarez, Neymar", "Villa, Messi, Pedro", "Ronaldinho, Eto'o, Messi"},
                  {"What was the '6 second rule' implemented by Pep Guardiola?", "If possession was lost, the team would try to win it back with full intensity for 6 seconds.", "Players had 6 seconds to celebrate a goal.", "Substitutions had to be made within 6 seconds.", "Players had 6 seconds to take a shot on goal."},
                  {"Which manager led the 2014-15 Barcelona team?", "Luis Enrique", "Pep Guardiola", "Tito Vilanova", "Frank Rijkaard"},
                  {"What historic achievement did the 2014-15 Barcelona team accomplish?", "They became the first club to win the treble twice.", "They won the Champions League three times in a row.", "They were the first club to go undefeated in La Liga.", "They scored 200 goals in a single season."},
                  {"What is Barcelona's club slogan?", "Més Que Un Club", "Visca Barca", "Blaugrana Al Vent", "Tots Units Fem Força"},
                  {"How many goals did Barcelona score in all competitions during the 2014-15 season?", "175", "150", "165", "190"},
                  {"What was the name of the tactical idea that originated with Spain’s 2006 team and was prominent in Luis Enrique's Barcelona?", "Tiki-Taka", "Total Football", "Positional Play", "Catenaccio"},
                  {"What was a key difference in Luis Enrique's approach to passing triangles compared to Pep Guardiola's?", "Enrique gave players more freedom and fluid positions.", "Enrique emphasized stricter positioning.", "Enrique focused on long diagonal passes.", "Enrique did not utilize passing triangles."},
                  {"Who were Barcelona's front three in the 2014-15 season?", "Messi, Suarez, Neymar", "Henry, Messi, Eto'o", "Messi, Villa, Pedro", "Ronaldinho, Eto'o, Ibrahimovic"},
                  {"Who played the holding midfield role (number 6) for Barcelona in the 2014-15 season?", "Busquets", "Xavi", "Rakitic", "Mascherano"},
                  {"Which two players played as the '8s' or makeshift '10s' in Barcelona's 2014-15 midfield?", "Iniesta and Rakitic", "Busquets and Iniesta", "Rakitic and Mascherano", "Xavi and Busquets"},
                  {"What was a key characteristic of the players in Luis Enrique's 2014-15 Barcelona system?", "Ability to play multiple positions.", "Rigid adherence to their assigned positions.", "Limited tactical flexibility.", "Emphasis on individual dribbling skills."},
                  {"What was a potential drawback of traditional Tiki-Taka that Luis Enrique's side aimed to overcome?", "Tendency to pass for the sake of passing without creating chances.", "Lack of defensive stability.", "Difficulty in controlling possession.", "Over-reliance on long passes."},
                  {"What was a key defensive tactic employed by Luis Enrique's Barcelona when not in possession?", "High pressing for 6 seconds.", "Deep defensive block.", "Pressing in waves with three players at a time.", "Aggressive man-marking."},
                  {"Who managed the 1971-72 Ajax team?", "Ștefan Kovács", "Rinus Michels", "Johan Cruyff", "Louis van Gaal"},
                  {"What treble did the 1971-72 Ajax team achieve?", "Eredivisie, KNVB Cup, European Cup", "League, FA Cup, Champions League", "League, Copa del Rey, UEFA Cup", "League Cup, Champions League, Club World Cup"},
                  {"How many goals did Ajax score in the league during the 1971-72 season?", "104", "87", "95", "112"},
                  {"What Italian defensive system did Ajax outclass in the 1972 European Cup Final?", "Catenaccio", "Zonal marking", "Libero system", "Pressing man-to-man"},
                  {"What football philosophy reached its peak with the 1971-72 Ajax team?", "Total Football", "Positional Play", "Tiki-Taka", "Gegenpressing"},
                  {"What was the core idea behind Total Football?", "Every outfield player should be able to play in any position.", "Strict adherence to fixed positions.", "Emphasis on long direct passes.", "Primarily focusing on defensive stability."},
                  {"Which player is considered the heart and philosopher of the Total Football system at Ajax?", "Johan Cruyff", "Johan Neeskens", "Piet Keizer", "Sjaak Swart"},
                  {"What was the focus of Total Football beyond just possession?", "Space - how to create, manipulate, and exploit it", "Individual skill", "Defensive strength", "Physical dominance"},
                  {"Which two fullbacks for Ajax in the 1971-72 season were key to creating width?", "Krol and Suurbier", "Hulshoff and Blankenburg", "Haan and Mühren", "Keizer and Swart"},
                  {"What was a key characteristic of the Ajax midfielders in the Total Football system?", "Constant rotation, forming diamonds and triangles.", "Specialized roles with limited movement.", "Primarily focused on defensive duties.", "Emphasis on individual dribbling."},
                  {"In the Total Football system, where did Johan Cruyff typically play on paper?", "Center forward", "Center back", "Defensive midfielder", "Right winger"},
                  {"What were the two core principles that Total Football was built upon?", "Fluidity and intelligence", "Discipline and physicality", "Speed and power", "Strict formation and set pieces"},
                  {"What was a remarkable aspect of the lack of specialization in Total Football?", "Cruyff could play CB and still dictate the game.", "Goalkeepers often played in midfield.", "Defenders were not allowed to cross the halfway line.", "Strikers were primarily responsible for defending."},
                  {"What tactical formation is associated with Johan Cruyff's successful Barcelona teams?", "3-4-3 diamond", "4-4-2 flat", "3-5-2", "4-3-3"},
                  {"How many consecutive La Liga titles did Cruyff win with his 3-4-3 diamond formation at Barcelona?", "4", "2", "3", "5"},
                  {"What was a significant long-term contribution of Cruyff to Barcelona beyond trophies?", "Establishing a dominant youth academy (La Masia).", "Developing a strong scouting network.", "Implementing a strict financial fair play policy.", "Modernizing the Camp Nou stadium."},
                  {"In Cruyff's 3-4-3 diamond, what was the role of the central center back?", "Needed to be good on the ball when pushing into midfield.", "Primarily a stopper and tackler.", "Marking the opposition's main striker.", "Staying deep to cover counter-attacks."},
                  {"In the 3-4-3 diamond, how many midfielders are typically positioned in the center?", "4", "2", "3", "5"},
                  {"What is a benefit of having 4 midfielders in the center in a 3-4-3 diamond against a 4-3-3 formation?", "Natural dominance in central areas.", "Better defensive coverage on the wings.", "More attacking threats in wide positions.", "Ability to play more direct long balls."},
                  {"What is a potential drawback of the 3-4-3 diamond in defending against wide threats?", "The three defenders can be easily overloaded.", "The central midfielders tend to drift inside.", "The wingers are positioned too high up the pitch.", "The CDM often pushes too high."},
                  {"Which manager is known for winning Manchester City's first ever UEFA Champions League?", "Pep Guardiola", "Roberto Mancini", "Manuel Pellegrini", "Manuel Pellegrini"},
                  {"What was the historic treble won by Pep Guardiola's Manchester City?", "Premier League, Champions League, FA Cup", "Premier League, FA Cup, League Cup", "Premier League, Champions League, League Cup", "Champions League, FA Cup, Community Shield"}
              }, 10);
    manManagementQuiz = Quiz({
                 {"Which one of these is NOT a good coaching tactic:", "Berate players often", "Understand the Sport", "Lead By Example", "Communicate Effectively"},
                 {"True or false: Steamrolling player leaders is a good idea", "False", "True"},
                 {"Who said “I will forgive if the players cannot get it right, but not if they do not try hard”?", "Pep Guardiola", "Mackelmore", "Billie Jean King", "Diego Simeone"},
                 {"True or false: A subbed out player can come back in the game", "False", "True"},
                 {"If you have used all five subs and an injury occurs:", "You’re out of luck and have to rely on the 10 uninjured players already on the pitch.", "You have another player replace the injured player", "The opposing team provides a player for your team to use", "The goalkeepers on both sides are taken out of the game to level the playing field"},
                 {"True or false: Subbing a player at half does NOT count as a substitution window:", "True", "False"},
                 {"How many sub windows does a manager have?", "Three", "Two", "One", "Four"},
                 {"Who do you have to signal to get a sub in?", "The sub referee", "Nobody! Run on in there champ!", "The fans", "FIFA HQ"},
                 {"What was the number of subs allowed before 2020?", "3", "2", "4", "5"},
                 {"Do all leagues and levels of play allow five subs?", "No", "Yes"}
             }, 3);
}

void MainWindow::addAllTheFormationsPages(){
    QVector<Formation*> formations = QVector<Formation*>();
    //Ferguson
    formations.append(new Formation(":/new/prefix1/Images/4-4-2.png",
                                    ":/new/prefix1/Images/fergusonTrophy.png",
                                    "The 4-4-2 formation is one of the most iconic and successful systems in football history, and no one used it better than Sir Alex Ferguson during his legendary reign at Manchester United. With this setup, Ferguson won 13 Premier League titles, 2 Champions League trophies, and built multiple dominant teams across different eras. The system consists of 4 defenders, 4 midfielders, and 2 strikers. What made Ferguson’s 4-4-2 so effective was its perfect blend of structure, width, and direct attacking threat. It was simple, adaptable, and ruthlessly efficient.",
                                    "4-4-2",
                                    {"Ferguson’s teams were built around pace, crossing, and lethal strike partnerships. He had wingers like Ryan Giggs and David Beckham stretching the field and whipping in crosses, while deadly duos like Dwight Yorke and Andy Cole or Rooney and Van Nistelrooy tore through defenses. The 4-4-2 under Ferguson wasn’t passive—it was aggressive, fluid, and highly coordinated. It was football that punched you in the mouth.",
                                     "The two banks of four provide a tight, compact defensive shape. It’s hard to break down, especially when defending a lead or sitting in a low block. The two strikers up front keep the opposing center-backs occupied and give you constant vertical threat. One could hold the ball up, the other could run in behind—Ferguson mastered this balance with his forward pairings. The wide midfielders (wingers) offer width both offensively and defensively. They stretch the play going forward and drop back to help defend when needed. The midfield duo is tasked with shielding the defense, winning second balls, and distributing quickly. Roy Keane, Paul Scholes, and Michael Carrick were perfect examples of the engine room in this shape. In attack, it’s fast and direct: win the ball, get it to the wings, cross it in, or play into the feet of a striker who’s making a run. Under Ferguson, United were devastating on the counter because of this. Defensively, it’s organized and compact. Each player knows their role, and the spacing between lines makes it easy to absorb pressure and then hit on the break",
                                     "The biggest issue with the 4-4-2 is midfield control. When facing modern formations like 4-3-3 or 4-2-3-1, your two central midfielders can easily get overrun by a trio. This can make it hard to maintain possession or control the tempo of the game, especially against teams with technically gifted midfielders. Because the wingers are responsible for both attacking and defending, they need to be extremely fit and disciplined. If they get caught too far forward, your fullbacks can get exposed. The system can become predictable if you don’t have high-quality players out wide or up front. Without individual brilliance or tactical tweaks, it can be hard to break down compact defenses."},
                                    this));

    //Klopp
    formations.append(new Formation(":/new/prefix1/Images/4-3-3.png",
                                      ":/new/prefix1/Images/kloppTrophy.png",
                                      "Jürgen Klopp’s 4-3-3 system at Liverpool became one of the most feared and influential tactical setups in modern football. With this formation, Klopp turned Liverpool into European and domestic giants, winning the Premier League, Champions League, FA Cup, and Club World Cup. His 4-3-3 wasn't just about formation—it was a philosophy centered around intensity, pressing, and vertical attacking. This version of the 4-3-3 was Klopp’s signature style of “Gegenpressing”, a high-octane system where the team presses immediately after losing the ball, aiming to win it back before the opponent can settle or counter.",
                                      "4-3-3",
                                      {"Unlike more possession-based 4-3-3 setups, Klopp’s was about chaos and control—overwhelming teams with pressure, pace, and quick transitions. With a front three of Salah, Mané, and Firmino, a tireless midfield, and attacking fullbacks like Trent Alexander-Arnold and Andy Robertson, Liverpool’s 4-3-3 was perfectly built for intensity and width.",
                                       "The front three are constantly rotating and pressing. Firmino, often dropping deep to link play, allowed Salah and Mané to make diagonal runs inside, almost playing as inside forwards. The midfield trio (typically Henderson, Fabinho, and Wijnaldum during their peak) provided an engine room of pressing, covering, and recycling possession. Their job wasn’t flashy—it was to win the ball, keep the tempo, and feed the front line. The fullbacks are key playmakers. Rather than traditional wingers providing width, it was Robertson and Trent pushing high and wide to deliver crosses and create overloads. Klopp essentially redefined the role of the fullback. Defensively, the 4-3-3 allowed for a compact midfield block and immediate pressing traps. As soon as the ball was lost, Liverpool would swarm the ball carrier to either win it back or force a long clearance. The system is perfect for transition football—winning the ball and immediately turning defense into attack. Liverpool were one of the most lethal teams in the world in those first 5-10 seconds after a turnover.",
                                       "Against low blocks, the 4-3-3 can struggle. When teams sit deep and absorb pressure, Liverpool can be forced into crossing without much central presence—especially if Firmino drops deep and there's no clear No. 9 in the box. The midfield isn't always creative. While industrious, the midfield three don’t always offer incisive passes or dribbling, which can limit options when trying to break through tight defenses."},
                                      this));

    //Guardiola
    formations.append(new Formation(":/new/prefix1/Images/4-2-3-1.png",
                                      ":/new/prefix1/Images/guardiolaTrophy.png",
                                      "With his 4-2-3-1 Pep Guardiola won City’s first ever UEFA Champions League, their first ever Treble, and won 4 Premier Leagues in a row (never done before). A 4-2-3-1 has two 6’s, a 10, and two wingers making up the midfield. There is one sole striker and four defenders (2 centerbacks and two fullbacks). The two 6’s help provide a lot of defensive stability and a lot of creative passing triangles. Because of this defensive solidarity, the 10 has a lot of freedom to move around and create chances. Meanwhile, the wingers and fullbacks can provide width.",
                                      "4-2-3-1",
                                      {"It plays with 4 defenders, with the fullbacks often tucking into midfield or pushing high depending on the phase of play. The two holding midfielders, usually a mix of a destroyer and a deep-lying playmaker, control the tempo and dictate build-up. In front of them is a 10 who floats between the lines, linking midfield to attack and creating overloads. The wide players stay high and wide to stretch the opposition, opening space inside for the 10 and overlapping fullbacks. Under Pep Guardiola at Man City, the 4-2-3-1 becomes a shape-shifting system—fluid in possession, aggressive in pressing, and built around positional play. The striker drops in or pins defenders depending on the buildup, and every player contributes to both phases, creating a controlled chaos that overwhelms opponents.",
                                       "The 2 CDMs drop deep to receive the ball from defenders, which helps progress play quickly. The defensive solidarity that those CDMs provide allows the full-backs to push up and help attack. The 10/CAM drops in and out of the midfield, picking up the ball and feeding it to the forwards. The wingers and fullbacks overlap and underlap to stretch the defensive team. Usually, when defending, you want to stay compact and narrow; being forced to stretch out wide provides holes in the center for the isolated 9 to exploit. When defending, the wingers can help cover opposing wide midfielders, turning the system into more of a 4-4-1-1. Overall, this system provides a lot of flexibility; if the wingers push up, it turns into a 4-3-3, if the CAM pushes up, it turns into a 4-4-2, etc. Pep is known for using these fluid formations, and these complex systems require players who can play multiple different positions well in the same game. For example, John Stones is a CB, but often he would push up and become a CDM, turning it into a 3-4-3. He also had Kyle Walker, a RB, also invert into a CDM to turn it into a similar 3-4-3 system. ",
                                       "If the wingers are being defended against properly, the ST will have no one providing width which will effectively cut the forward out of the game. This system requires a really gifted 10 because it needs to be someone who is exceptional on the ball and has gifted vision. Pep understood that Kevin De Bruyne is arguably the best midfielder to have ever played in the Premier League and so he designed this system to get the most out of De Bruyne."},
                                      this));

    //Ancelotti
    formations.append(new Formation(":/new/prefix1/Images/4-3-2-1.png",
                                      ":/new/prefix1/Images/ancelottiTrophy.jpg",
                                      "With his 4-3-2-1 (Christmas Tree), Carlo Ancelotti won 2 UEFA Champions Leagues (2003, 2007)and 1 Scudetto (2004) at AC Milan. This system was designed to maximize the talents of Kaká, Andrea Pirlo, and Clarence Seedorf while maintaining defensive stability. Ancelotti’s Milan is remembered as one of the greatest club sides of the 2000s, dominating Europe with this unique structure.",
                                      "4-3-2-1",
                                      {"The 4-3-2-1 consists of four defenders, three central midfielders (a deep-lying playmaker, a box-to-box, and a destroyer), two attacking midfielders (shadow strikers), and a lone striker. The two AMs operate in the half-spaces, linking midfield and attack, while the midfield trio controls possession. The full-backs provide width, allowing the AMs to drift inside.",
                                       "Overloads in central areas: The two AMs and striker create a narrow attacking trio, overwhelming opposing center-backs.\nMidfield dominance: The three-man midfield ensures control against both 4-4-2 and 4-2-3-1systems.\nFluid movement: The AMs interchange positions, making it hard for markers to track them.\nPirlo’s role as a regista: The deep-lying playmaker dictates tempo, spraying long balls to the AMs and striker.",
                                       "Lack of natural width: If full-backs don’t push high, the attack becomes too predictable.\nDefensive vulnerability on the flanks: Opponents can exploit the space behind the attacking full-backs.\nHeavy reliance on creative AMs: If the two attacking midfielders are marked out of the game, the striker becomes isolated.\nRequires a world-class regista: Without a player like Pirlo, the buildup can stagnate."},
                                      this));

    //Mourinho
    formations.append(new Formation(":/new/prefix1/Images/3-5-2.png",
                                      ":/new/prefix1/Images/mourinhoTrophy.png",
                                      "With his 3-5-2, José Mourinho won the Europa League with Roma (2022), the Serie A title with Inter Milan (2010), and the UEFA Champions League with Inter (2010)—completing a historic treble. He also used this system to great effect at Chelsea (in his second stint) and Manchester United, where he won the Europa League (2017) and League Cup (2017). Mourinho’s pragmatic yet defensively solid approach made the 3-5-2 a formidable system against possession-heavy teams.",
                                      "3-5-2",
                                      {"The 3-5-2 consists of three center-backs, two wing-backs, three central midfielders (typically a destroyer, a box-to-box, and a playmaker), and two strikers. The wing-backs provide width in attack but must track back to form a back five in defense. The midfield trio must be disciplined—one holding midfielder shields the defense while the other two balance between attack and defense. The two strikers work in tandem, with one often dropping deep to link play while the other stays high.",
                                       "When defending, the LM and RM turn into a LWB and a RWB which adds immense defensive stability to the structure. When attacking, that same LM and RM turn into a RW and LW which means you get width and 4 attacking players which is hard to defend against. Because there are so many players in the center, Mourinho used this system against Spanish teams who liked to have passing lanes in the center, forcing them to play solely along the wing which they aren’t comfortable doing.",
                                       "If those LM and RM aren’t really good at both attacking and defending or if they don’t have the stamina to play the whole game, then this system will not work. If the opposing full backs like to play really high then the LM/RM have to cover both the opposing wingers and the full backs which they can struggle to do if the rest of the team isn’t helping cover defensively. If you push the LM/RM up too high, than you can sometimes be exposed on the counter attack."},
                                      this));

    //Simeone
    formations.append(new Formation(":/new/prefix1/Images/4-1-2-1-2.png",
                                      ":/new/prefix1/Images/simeoneTrophy.jpg",
                                      "Diego Simeone built his legacy at Atlético Madrid on discipline, unity, and tactical sacrifice—and no formation better represents his philosophy than the 4-1-2-1-2 (narrow diamond). Under Simeone, Atlético became one of the toughest sides to break down in world football, emphasizing defensive compactness, structured pressing, and lethal counter-attacks. His teams didn’t need 70% possession—they needed 90 minutes of focus. The narrow diamond allowed him to clog the midfield, break up opposition play, and launch swift, direct attacks through two strikers. This wasn’t about flair—it was about grit, balance, and clarity of purpose. The 4-1-2-1-2 gave Simeone the perfect tactical structure to demand maximum commitment from every player on the pitch.",
                                      "4-1-2-1-2",
                                      {"It plays with a narrow midfield diamond and two strikers up top. The single 6 sits just in front of the back line, acting as the primary shield and deep-lying distributor. Ahead of them are two box-to-box 8s who cover ground, link play, and support both defense and attack. At the tip of the diamond is the 10, who operates in the pockets, connecting with the front two and dictating the final third. The fullbacks are key—they provide all the width, overlapping aggressively since there are no traditional wingers. This shape thrives on central overloads and quick vertical play, creating tight passing triangles through the middle while relying on dynamic movement and support from the outside backs.",
                                       "Central control: With four players occupying central midfield spaces, the formation dominates the middle of the pitch, making it difficult for the opposition to create chances through the center. This creates a natural density that’s great for winning second balls and cutting off passing lanes.\nDefensive solidity: The lone holding midfielder protects the back four and acts as a pivot in both defense and buildup. The compact shape allows for quick shifting as a unit and is excellent at absorbing pressure.\nTwo strikers: Unlike many modern systems with one central forward, the 4-1-2-1-2 uses a strike partnership. This allows for diverse attacking patterns like one striker dropping deep while the other runs in behind, or combination play in tight areas.\nDirect transitions: The system naturally favors verticality. With fewer players wide, the team can transition quickly from defense to attack through the center, using short, sharp passing and movement.\nCohesion and simplicity: Players in this setup have clearly defined roles and stay close to each other. That helps maintain structure, press cohesively, and prevent isolation in both offensive and defensive phases.",
                                       "Lack of width: Without natural wingers, the system relies heavily on fullbacks for width. If the fullbacks can’t push forward effectively—or are pinned back—the attack can become narrow and predictable.\nVulnerable to wide overloads: Opposing teams with strong wing play can exploit the flanks, stretching the diamond and pulling it apart, especially during defensive transitions.\nAttacking predictability: Because it emphasizes central play and vertical passes, teams that sit deep and defend compactly can force the system into a stalemate, especially if creativity is lacking in the final third.\nHeavy physical demands: The central midfielders in this system must cover wide areas defensively and support both attack and buildup. This makes it physically taxing, especially if the press isn’t well-coordinated.\nSpacing in build-up: When facing high pressing teams, the narrow formation can struggle to progress the ball wide, leading to congestion in central areas and risky short passing under pressure."},
                                      this));

    //Cruyff
    formations.append(new Formation(":/new/prefix1/Images/3-4-3.png",
                                      ":/new/prefix1/Images/cruyffTrophy.png",
                                      "With his 3-4-3 diamond, Cruyff won 4 La Liga’s in a row, Barcelona’s first ever European Cup, one Copa del Rey, and multiple Supercopas. A lot of the players who played for Cruyff went on to become elite managers themselves like Gaurdiola and Koeman. He also developed some elite stars like Michael Laudrup, Stoichkov, and Romario. Cruyff is also attributed with being one of the first managers to really bolster Barcelona’s academy, La Masia, which is easily the best academy historically and presently across the sport.",
                                      "3-4-3",
                                      {"It plays with 3 defenders. The central center back needs to be good on the ball when they push up and pass into the midfield. There is one 6, two 8’s, and a 10. The two wingers play very wide and provide the width for the team, this allows the 10 to push up with the 9 to create an offensive threat.",
                                       "Having 4 midfielders in the center means you will naturally dominate 4-3-3 teams. The diamond creates triangles and passing lanes. The wingers get to stay wide which allows the CMs to drift inside and control that key central space. The CMs and wingers can also help defend on the wings. This fluidity helps teach youngsters the foundations of managing space and learning the tactics of the game.",
                                       "Because the CMs like to drift centrally, it’s hard to defend against wide threats quickly. Oftentimes, it is the wingers who defend against those, which means you need to have defensively solid wingers (rare). If the CDM doesn’t have good awareness, the defenders can get caught in no man’s land during counterattacks. This system relies on a CDM who knows when to sit back and when to push up. And if the midfielders aren’t good technically, high-pressing teams can punish their diamond very quickly."},
                                      this));

    //Zidane
    formations.append(new Formation(":/new/prefix1/Images/4-2-2-2.png",
                                      ":/new/prefix1/Images/zidaneTrophy.png",
                                      "The 4-2-2-2 is a compact and aggressive system built on vertical passing, central overloads, and fullback-driven width—and Zinedine Zidane used it to devastating effect during his legendary run as Real Madrid manager. With this shape, Zidane led the club to 3 consecutive Champions League titles, 2 La Liga trophies, 2 Spanish Super Cups, 2 UEFA Super Cups, and 2 FIFA Club World Cups—making him one of the most decorated managers in modern football history. The formation relied on a double pivot—often Casemiro and Kroos—for defensive stability and ball progression, while two narrow attacking midfielders, like Modrić and Isco, operated between the lines to feed a fluid front two. The strikers, typically including Benzema and Ronaldo, thrived on quick combinations and intelligent movement. With fullbacks like Marcelo and Carvajal providing the width, Zidane’s 4-2-2-2 created a team that was compact defensively but explosive in transition. It wasn’t just about tactics—it was Zidane’s ability to manage egos, rotate intelligently, and win when it mattered most that turned this system into a dynasty.",
                                      "4-2-2",
                                      {"The 4-2-2-2 is a very unpopular system also referred to as the chimney stack or the magic rectangle. This system is usually played to fit very specific player profiles that are not suited for a traditional flat 4-4-2. Zinedine Zidane is one of the greatest managers of his generation. He’s the only manager ever to have won 3 UEFA Champions Leagues back to back. Due to injuries, he sometimes implemented this system. Real Madrid at that time had a lot of quality 10’s like Asensio and Modric. They also had good holding midfielders like Casemiro and Toni Kroos. Their attackers preferred playing centrally like Ronaldo and Benzema. Because of this awkward mix of player profiles, Zidane sometimes used a 4-2-2-2. There are plenty of other teams that have used this structure due to similar reasons (more CAMs and CDMs than wingers). ",
                                       "The midfield is very balanced. There’s a lot of defensive support provided by the 6s and a lot of offensive support provided by the 10s. If your 10s are the kind of CAMs that like having flexibility to drift in and out of the center and wide lanes, this system is very well suited for them. Having 4 players in the midfield is also considered a tactical advantage against 5-3-2 and 4-3-3 systems because you can often dominate possession and press high up the pitch when out of possession. And because you have 4 attacking players on the pitch and 6 defensive players, it is also very well suited to play good counterattacking soccer.",
                                       "This system is highly reliant on the fullbacks being able to provide offensive width and also defend the entirety of the wings. When playing against other systems with 4 midfielders this formation can become very congested in the center, which effectively takes the 2 attackers out of the game. Basically, this system is only really used when you don’t have fit wingers."},
                                      this));

    //Alonso
    formations.append(new Formation(":/new/prefix1/Images/5-4-1.png",
                                      ":/new/prefix1/Images/alonsoTrophy.jpg",
                                      "The 2023–24 Bayer Leverkusen side, led by Xabi Alonso in just his first full season as manager, didn’t just make history—they shattered expectations. A 5-4-1 alongside Alonso’s calm authority and cerebral approach, Leverkusen went unbeaten through the Bundesliga season, clinching the title for the first time in their 120-year existence. But it wasn’t just the silverware—it was the style. Blending Spanish control with German efficiency, Alonso crafted a team that pressed with intelligence, built with purpose, and attacked with flair. They swept aside giants like Bayern Munich with a swagger rarely seen outside Europe's elite. More than a title, the season marked the emergence of Alonso not just as a manager of promise, but as the next great mind in football. In one season, he transformed Leverkusen from nearly-men to model club—and he’s only just begun.",
                                      "5-4-1",
                                      {"Xabi Alonso brought a distinctive, calculated approach to management, blending discipline with his deep understanding of the game’s nuances. His version of the 5-4-1 formation, most notably used at Real Sociedad and later during his time at Bayer Leverkusen, is centered on defensive solidity and possession-based control. Unlike traditional counter-attacking systems, Alonso’s 5-4-1 was designed to dominate the midfield and protect the defensive line while still maintaining the ability to attack with pace. By relying on a compact back five and a highly disciplined midfield, Alonso's system enabled his teams to absorb pressure, control possession, and launch rapid counters when opportunities arose.",
                                       "Defensive compactness: The 5-4-1 system is inherently defensive, with five players in the backline and a four-man midfield that can sit deep or press when needed. This setup makes it extremely difficult for opponents to break through the middle or exploit spaces between the lines.\nCentral midfield control: The four midfielders (often including a double pivot) work together to control the game in central areas. The deep-lying midfielders provide a solid foundation for recycling possession, while the more advanced midfielder is tasked with linking up play and supporting the lone striker.\nCounter-attacking potential: With the defensive solidity of five at the back and the midfielders’ ability to intercept passes and quickly transition, the system is well-suited for swift counter-attacks. The lone striker has support from the midfield runners, and the wide midfielders can exploit space quickly.\nWide overloads: The wing-backs in this system are key to stretching the pitch. They can push up to provide width in attack, allowing the team to exploit space on the flanks, or drop deep when defending to create a solid back five.\nStability during transitions: Alonso’s teams typically pressed intelligently, aiming to regain possession quickly. Once the ball was won back, the compact midfield and back five provided stability, ensuring quick transitions without losing balance or leaving gaps at the back.",
                                       "Limited attacking options: With only one forward, the 5-4-1 can often leave the lone striker isolated, particularly against well-organized defenses. If the midfield doesn’t link effectively with the attack, it can become difficult to create sustained pressure on the opponent.\nOver-reliance on wing-backs: The wing-backs are crucial for creating width and support in attack. However, they also have defensive responsibilities and must track back quickly. If the wing-backs are unable to provide attacking support or are caught too high up the pitch, the team becomes too narrow and predictable.\nLack of natural width in attacking phases: Since the system plays with a lone striker, there is less natural width in offensive transitions, making it harder to stretch defenses and create overloads in the final third. This can especially become a problem when facing teams that sit deep and congest the center of the pitch.\nDefensive vulnerability on the counter: Although the system is well-structured defensively, it can leave spaces on the counter, especially if the team is committed too high up the pitch. The wing-backs need to recover quickly, and the lone central defender can sometimes become exposed if the midfield fails to track back effectively.\nLimited creativity in midfield: With the focus on defensive stability, the midfield can occasionally lack the creativity needed to break down well-organized opposition. The players in the central midfield must be technically proficient, but this system may sometimes lack the dynamic attacking movement seen in more open formations."},
                                      this));

    //Conte
    formations.append(new Formation(":/new/prefix1/Images/5-3-2.png",
                                      ":/new/prefix1/Images/conteTrophy.jpg",
                                      "Antonio Conte is one of modern football’s most influential tacticians when it comes to mastering the back five system—and his 5-3-2 at Inter Milan was peak Conte. When he took charge in 2019, Inter hadn't won a league title in over a decade. By the end of the 2020-21 season, they were Serie A champions, breaking Juventus’ nine-year stranglehold on the Scudetto. ",
                                      "5-3-2",
                                      {"Conte’s 5-3-2 wasn’t just a defensive formation—it was an engineered machine designed for both stability and lightning-fast transitions. Every player had a defined role, every movement was drilled, and the structure allowed Inter to be disciplined without the ball and devastating with it. This was a system built on shape, steel, and surgical execution.",
                                       "Defensive stability: With three center-backs and two wing-backs, the back five provides excellent coverage across the width of the pitch. It’s especially hard to break down in central areas and allows teams to sit deeper without sacrificing structure.\nWide overloads: The wing-backs provide width both offensively and defensively. They stretch the pitch going forward and can double up with central midfielders or forwards in wide areas to isolate defenders.\nClear passing lanes: The three-man midfield can form a triangle that connects smoothly with both defense and attack. It allows for short, secure buildup play while also enabling quick switches of play.\nStrike partnership: The two strikers up front are key to Conte’s system. One often plays as a target man while the other runs the channels or drops into space. Their coordinated movement allows for fast, direct counter-attacks and link-up play.\nStructured transitions: With five defenders behind the ball and three midfielders ready to pounce, the team is well-equipped to win the ball and immediately break forward. The system creates natural vertical passing lanes and often catches opponents off guard during transitions",
                                       "Overreliance on wing-backs: The system lives and dies by the wing-backs. If they can’t contribute both offensively and defensively, the formation can become passive or disjointed. They must be extremely fit, tactically aware, and technically sound.\nLimited attacking width in higher zones: Although wing-backs provide width, they start from deeper positions. Against low blocks, this can make it hard to create overloads in the final third, especially if the midfield lacks creativity.\nCan be passive without the ball: When pinned deep, the system can lead to a low block that struggles to escape pressure. If the forwards aren’t supported quickly, transitions break down and possession is lost.\nRequires high tactical discipline: Every player must understand their role and positioning to keep the shape intact. A single mistake—especially from a wing-back or center-back—can expose large spaces and lead to dangerous chances.\nMidfield can be overrun: With only three central midfielders, teams that play with a four- or five-man midfield can outnumber them and dominate possession if pressing and positioning aren’t perfectly timed."},
                                      this));

    //First Barca
    formations.append(new Formation(":/new/prefix1/Images/firstBarca.png",
                                      ":/new/prefix1/Images/firstBarcaTrophy.jpg",
                                      "Considered by many as the greatest team of all time, the 2008-09 Barcelona team managed by Pep Guardiola were a joy to watch. They were the first team in history to win all six major trophies in a single season (La Liga, Copa del Rey, UEFA Champions League, Supercopa de España, UEFA Super Cup, and Club World Cup). Barcelona scored 158 goals across all competitions that season, including 6 goals scored against Real Madrid in a historic 6-2 El Clásico win. That season was considered a breakout season for both Messi, considered by most as the greatest player of all time, and Pep, who is also considered by many as the greatest manager of all time. ",
                                      "POSITIONAL PLAY",
                                      {"Overview", "The players", "The system"},
                                      {"This team was regarded is the greatest of all time not because of what they won, but because of how they won it. Pep’s Barca were a joy to watch because of the positional play system that Pep implemented. You may have heard that triangles are the strongest shape in geometry; they’re also the strongest shape in soccer. The idea of passing triangles, or having 3 players that are going to pass the ball amongst themselves to create a more advantageous position, is a common trend you’ll see amongst all formations and teams. The way Pep did this is by ensuring that players stick to their positions. Thierry Henry once told a story about how he drifted out of position and scored a goal and Pep subbed him off for not sticking to the game plan. This helped create direct passing lanes and overloads. Let’s examine how:",
                                       "Although on paper it seemed like Barca played a flat 4-3-3, they actually played more of a narrow 4-1-2-1-2. This was Pep’s revolutionary system. The idea of the False 9. The 4 defenders were Abidal (LB), Pique (CB), Puyol (CB), and Alves (RB). Sometimes Slyvinho played as a RB and Rafa Marquez played instead of Pique at CB. The midfield and the front line is where the tactical advantage comes from. Magnus Carlsen once compared Pep’s system to his own playing style in chess: The one who controls the center controls the game. The way Pep’s team did this is by not playing Messi as an out and out striker but rather a false 9. He would drop much deeper into the midfield. This would mean that Barca were playing with 4 midfielders instead of just 3. Having that extra player meant that Barca could very easily create triangles in the midfield, pass around, and create an attacking chance. To combat this, teams started having a CB follow Messi into the midfield; in this situation, Dani Alves (the RB) would push up creating a 3v3 situation which is usually considered to be advantageous to the attacking team. Especially here where Barca’s midfield consisted of Xavi (8), Iniesta (10), and Busquets (6) the greatest midfielders of all time. Barca’s front three was Thiery Henry (LW), Messi (CF), and Samuel Eto’o (RW).",
                                       "Pep played a strict positional system. It was rare to find 2 players in the same line vertically or horizontally when Barca had possession (passing triangles). Certain players knew not to push too high like Abidal and Busquets because they had to press if the more attacking players lost possession. This was often nicknamed the 6 second rule where if the team lost possession, then for 6 minutes they would try to win the ball back with full intensity. Pep considered this the sweetspot because Barca’s revolutionary system was designed to draw the opposition out of position so they would try winning the ball immediately before they could get back into shape, if they couldn’t win the ball back within 6 seconds, they would return to their defensive shape because there was no point wearing out the players and adding fatigue by maintaining that constant pressure. "},
                                      this));

    //Madrid
    formations.append(new Formation(":/new/prefix1/Images/MadridFormation.png",
                                      ":/new/prefix1/Images/madridTrophy.jpg",
                                      "The 2011-12 Real Madrid team coached by José Mourinho wasn’t just a great side—it was a record-breaking machine. They won La Liga with a historic 100 points, scoring a mind-blowing 121 goals, the most in Spanish top-flight history at the time. They also finished the season with a goal difference of +89 and 32 wins from 38 matches. Most importantly, they ended Pep Guardiola’s Barcelona era of dominance domestically, snatching the title from one of the greatest teams ever with a completely opposite philosophy. This team didn’t try to control possession. They controlled moments. It wasn’t about having the ball—it was about what they did when they won it back. And what they did was lethal.",
                                      "Counter-Attack",
                                      {"Overview", "The players", "The system", "The mentality", "The Legacy"},
                                      {"Where Pep’s Barcelona strangled teams with slow, positional play, Mourinho’s Madrid exploded into attack. Their philosophy centered on transitions—turning defense into offense within seconds. Their counter-attacks weren’t just fast—they were orchestrated at lightning pace with devastating precision. It was like a track team that had studied football tactics. In many ways, this Madrid was a hybrid between tactical rigidity and sheer athleticism. They didn’t need to dominate the ball. They needed you to lose it—and then they’d destroy you before you could react.",
                                       "On paper, the formation was a 4-2-3-1, but the identity of the team came from its personnel. The backline: Arbeloa (RB), Pepe (CB), Sergio Ramos (CB), and Marcelo (LB). Marcelo, in particular, was key in breaking forward during transitions. Xabi Alonso and Khedira played as the double pivot—Xabi being the deep-lying passer, Khedira the engine. Then came the real weapons. Mesut Özil as the No. 10—the magician of space. On the wings: Ángel Di María (right) and Cristiano Ronaldo (left), arguably in his most explosive form ever. Up top was Karim Benzema, who acted more like a facilitator than a traditional striker, often drifting deep to combine and drag defenders.",
                                       "Mourinho didn’t park the bus like he did at Inter. At Madrid, he weaponized pace and verticality. The team would often sit in a mid-block, inviting pressure. The moment they intercepted or regained possession, they wouldn’t waste time building up—they’d go straight for the throat.\nA typical counter looked like this: Xabi Alonso wins the ball → plays it to Özil in between the lines → Özil turns and immediately looks for Ronaldo or Di María running into space → within 3-4 touches, Madrid is in the opponent's box.\nTheir counters could start from their own box and end in a goal within 10 seconds. There was no overcomplication—just pure speed, instinct, and coordination.",
                                       "Mourinho called it “vertical football.” Others called it “ruthless.” This team didn’t care about tiki-taka or pretty patterns. They cared about results—and intimidation. The pace at which they played was suffocating. Teams were afraid to commit bodies forward because they knew Madrid would punish them in transition. And when they needed to press, they did—especially in big games. Mourinho gave them tactical flexibility: sometimes pressing high, sometimes absorbing pressure, always with a clear plan on how to break when the ball was won.",
                                       "This team wasn’t just successful—they were revolutionary. They showed that you could beat the best possession-based teams in the world not by matching them at their own game, but by playing your own better. They proved that verticality, athleticism, and sharp tactical discipline could outclass elegance. The 11/12 Real Madrid team is often overshadowed by Pep’s Barca, but they were arguably the perfect counter-weight—a team built to destroy possession-based systems with sheer power and pace. Mourinho didn’t just end an era—he created a blueprint for how to beat it."},
                                      this));

    //Inter
    formations.append(new Formation(":/new/prefix1/Images/InterFormation.png",
                                      ":/new/prefix1/Images/interTrophy.png",
                                      "José Mourinho’s 2009-10 Inter Milan team may not be the most beloved side by purists, but they are one of the most iconic and effective teams in football history. That season, Inter achieved the Treble—winning Serie A, the Coppa Italia, and the UEFA Champions League, becoming the first Italian team to ever do so. Their UCL run was legendary, culminating in a 2-0 victory over Bayern Munich in the final. However, the true defining moment of this team came in the semifinal against Barcelona, where Mourinho’s men delivered one of the most famous \"park the bus\" masterclasses in football history, especially during the second leg at the Camp Nou where they played with ten men for over an hour and still managed to hold on.",
                                      "Park the Bus",
                                      {"Overview", "The players", "The system", "The mentality", "The legacy"},
                                      {"This team wasn’t admired for flair or possession—it was feared for its discipline, tactical rigidity, and unwavering commitment to defending as a unit. Mourinho’s Inter didn’t seek control of the ball—they sought control of space. Where Pep's Barcelona sought to outpass and outplay opponents, Mourinho's Inter sought to out-think, out-work, and outlast them. Their style was based on deep blocks, low defensive lines, aggressive man-marking, and counterattacks that were brutally efficient.",
                                       "On paper, Inter often lined up in a 4-2-3-1 or 4-3-1-2, but what made the system click was the understanding between the players. The backline was a wall: Maicon (RB), Lucio (CB), Walter Samuel (CB), and Cristian Chivu (LB). The double pivot of Esteban Cambiasso and Javier Zanetti offered pure grit and intelligence, while Wesley Sneijder served as the creative outlet in transition. On the flanks, Eto’o and Pandev often played as auxiliary fullbacks, sacrificing their attacking instincts to support the defense. Diego Milito, the lone striker, thrived on minimal touches but maximum efficiency, famously scoring both goals in the final. Mourinho turned Eto’o, a legendary striker, into a hardworking wide midfielder. His discipline and work rate in helping Chivu deal with Dani Alves in the semifinal was a tactical masterclass. Mourinho didn't ask his players to just defend—he demanded that they suffer together.",
                                       "The term \"park the bus\" doesn’t quite do justice to the tactical sophistication Mourinho employed. Inter would retreat into a 6-3-1 or 5-4-1 block in big games, prioritizing compactness over possession. In the famous second leg against Barcelona at the Camp Nou, Inter completed only 67 passes in the entire match—Barcelona completed over 500—yet it was Inter who progressed. Their structure was all about denying space. The midfield three often sat just in front of the back four, creating layers of defense. Pressing was done situationally—not with high intensity like Pep’s 6-second rule, but rather with cold calculation. If you beat one man, the next would be waiting, and you still had to break through another three lines. Despite their ultra-defensive setup, they were devastating on the break. Their transitions from defense to attack were quick and vertical. A single interception from Cambiasso could immediately turn into a counter with Sneijder playing Milito through. The idea was never to dominate the game but to dictate its terms.",
                                       "What defined this team wasn't just tactics—it was belief. Mourinho fostered a siege mentality, convincing his players that the world was against them. They embraced it. He once said, \"You may have the ball, but we’ll decide the result.\" That mental toughness was on full display when Inter played with 10 men for over an hour against Barcelona and still defended with ferocity and precision.",
                                       "While 08/09 Barca is remembered for how they played, 09/10 Inter is remembered for how they refused to play the way others wanted. Mourinho’s Inter proved that there’s more than one way to win—and that winning ugly can be just as legendary. Their journey was not just a footballing triumph, but a tactical and psychological masterclass that redefined what defensive football could achieve at the highest level."},
                                      this));

    //Ajax
    formations.append(new Formation(":/new/prefix1/Images/AjaxFormation.png",
                                      ":/new/prefix1/Images/ajaxTrophy.jpg",
                                      "The 1971-72 Ajax team, managed by Ștefan Kovács after Rinus Michels laid the groundwork, is considered one of the most influential teams in football history. That season, Ajax won the Eredivisie, KNVB Cup, and the European Cup, completing a treble long before it became a household term. They didn’t just win—they redefined the game. They scored 104 goals in the league and defeated Inter Milan 2-0 in the European Cup Final, completely outclassing the rigid Italian catenaccio system. That season marked the peak of Total Football, and at the heart of it all was Johan Cruyff, a player who wasn’t just the best on the pitch, but also a philosopher of the game. Ajax’s 71/72 season laid the tactical blueprint for everything that came after.",
                                      "Total Football",
                                      {"Overview", "The players", "The system"},
                                      {"Total Football was more than just a formation or system—it was a philosophy. The idea was simple: every outfield player should be able to play in any position. If a fullback pushed forward, a midfielder or winger would drop back. If the striker dropped into midfield, someone else filled the space up front. It was a system of constant movement and rotation to draw the opposition out of their positions, with an almost telepathic understanding between players. Total Football wasn’t just tactics—it was music, it was jazz. Free-flowing, improvisational, but rooted in a shared rhythm. The focus wasn’t just possession—it was space. How to create it, manipulate it, and ultimately, exploit it.",
                                       "On paper, Ajax often lined up in a 4-3-3, but in reality, it was anything but fixed. The back line featured Ruud Krol (LB), Barry Hulshoff (CB), Horst Blankenburg (CB), and Wim Suurbier (RB). But at any moment, these defenders could be seen pushing into midfield or wide areas, depending on the shape of the game. Krol and Suurbier, especially, were key to creating width and overloads, almost like inverted wingbacks before the term existed. In midfield, Arie Haan, Johan Neeskens, and Gerrie Mühren offered energy, creativity, and versatility. They rotated constantly, forming diamonds and triangles to maintain possession and shift the opposition out of shape. And then there was Cruyff. Officially a center forward, Cruyff played everywhere. False 9, playmaker, winger—wherever the game needed him. His freedom was the engine of Total Football. His intelligence was unmatched. He would drop deep, drag defenders out, play quick one-twos, and dictate the tempo like a conductor. Alongside him, Piet Keizer (LW) and Sjaak Swart (RW) were wide forwards who combined flair with tactical awareness. Keizer, in particular, was crucial in stretching defenses and providing crosses when needed. But like everyone else, they weren’t bound to their flank—they drifted, inverted, and switched sides constantly.",
                                       "Total Football was built on two core principles: fluidity and intelligence. Every player needed technical ability, tactical awareness, and the stamina to keep moving. You’d rarely see Ajax players stuck in straight lines. Instead, they formed diamonds and triangles, creating passing lanes and numerical advantages. The team pressed high, suffocated opponents in possession, and instantly transitioned when they lost the ball. What made it even more incredible was the lack of specialization. Cruyff could play CB and still dictate the game. Neeskens could score goals as easily as he could intercept the ball. Every player was comfortable on the ball. Every player could cover for another. That’s what made them so dangerous—you couldn’t press or mark them traditionally, because there was no “position” to target, only space. And the Dutch players knew how to command that space. Their vision and understanding of the game was so far superior to their opponents that they were untouchable."},
                                      this));

    //Liverpool
    formations.append(new Formation(":/new/prefix1/Images/LiverpoolFormation.png",
                                      ":/new/prefix1/Images/liverpoolTrophy.jpg",
                                      "After 30 long years, Liverpool finally reclaimed the Premier League title in 2019-20 under Jürgen Klopp. They didn’t just win it—they dominated. The team finished with 99 points, the second-highest tally in Premier League history at the time, and clinched the title with 7 games to spare, the earliest anyone has ever done it. They also had a 44-game unbeaten run stretching across two seasons and finished the campaign with the best defense and second-most goals scored. This wasn’t just a team that played well. It was a team that suffocated opponents. And the heartbeat of it all was Klopp’s weaponized version of gegenpressing—turning defense into attack immediately after losing the ball.",
                                      "Gegenpress",
                                      {"Overview", "The players", "The system", "The mentality", "The legacy"},
                                      {"“Gegenpressing” (German for \"counter-pressing\") is more than just pressing high. It’s about winning the ball back the second you lose it, ideally in advanced areas. Klopp once said, \"No playmaker in the world can be as good as a good counter-pressing situation.\" This team was built to prove that true. The idea is simple: when you lose the ball, don’t retreat—hunt. Don’t let the opposition settle or pick a pass. Instead, trap them near their own goal, win it back quickly, and exploit the chaos. The 19/20 Liverpool team was the most well-oiled gegenpressing machine the Premier League has seen.",
                                       "The system was typically a 4-3-3, but like most great teams, it was fluid in execution:\nAlisson in goal: calm under pressure and capable of distributing quickly.\nThe back four: Alexander-Arnold (RB), Van Dijk (CB), Joe Gomez (CB), and Andrew Robertson (LB). The fullbacks were essentially wingers, especially Trent, who was a playmaker in disguise.\nIn midfield: Jordan Henderson, Gini Wijnaldum, and Fabinho. Each had a role—Fabinho the anchor and destroyer, Wijnaldum the press-resistant carrier, Henderson the energy and leadership.\nUp front: Sadio Mané (LW), Roberto Firmino (CF), and Mohamed Salah (RW). Firmino, in particular, was crucial—dropping deep, pressing intelligently, and linking play.",
                                       "Here’s how Liverpool’s gegenpress worked in practice:\nLiverpool presses high up the pitch, using the front three to cut off passing lanes and push the ball wide.\nOnce the opponent tries to play out, the midfield joins in—swarming the ball carrier with two or three players.\nAs soon as they win it back, the transition is instantaneous. Trent or Robbo are already in advanced positions, Salah and Mané are sprinting in behind, and Firmino drops to connect the play.\nIt wasn’t chaos—it was choreographed chaos. Every player knew when to press, where to press, and who to press. The key was in trapping opponents in wide areas and forcing turnovers near their own box. And once Liverpool won the ball back, the front three needed just seconds to create a scoring chance.\nAnother wrinkle? The fullbacks as playmakers. Instead of relying on midfielders to pull strings, Klopp had Trent and Robertson whip in crosses from deep or deliver cutbacks from the byline. This meant that while teams were busy dealing with Salah and Mané inside, they were constantly being pulled apart by the width.",
                                       "Klopp called it “heavy metal football”—intense, relentless, and emotionally charged. The team didn’t just play to win—they played to overwhelm you. Their energy levels were off the charts, their work rate unmatched. And the belief? Unshakeable. Even when down a goal, this team never looked rattled. They trusted the system. Trusted the press. Trusted that their work off the ball would eventually create chances on it.",
                                       "The 19/20 Liverpool side was the culmination of Klopp’s vision: intensity over possession, collective effort over individual brilliance, and structure that feeds chaos. They didn’t win the league by controlling every match—they won it by controlling the moments that matter most through suffocating counter-pressing and lightning-fast transitions. It wasn’t just a title-winning team—it was a machine. A system so synchronized and sharp, it made pressing an art form."},
                                      this));

    //Leicester
    formations.append(new Formation(":/new/prefix1/Images/LeicesterFormation.png",
                                      ":/new/prefix1/Images/leicesterTrophy.jpg",
                                      "One of the greatest underdog stories in all of sports history. Leicester City, a team that was battling relegation the season before, won the Premier League in 2015-16 at 5000-to-1 odds. Under Claudio Ranieri, they shocked the world, finishing the season with 81 points, 10 points clear of second-place Arsenal. This wasn’t a fluke. Leicester beat big teams, went on unbeaten runs, and showed tactical maturity that outclassed even the most expensively built squads in England. But what made this team legendary wasn't just that they won—it was how they won. This wasn’t tiki-taka. This wasn’t gegenpressing. This was pure, unapologetic, direct football—long balls, low possession, and lightning-fast counters.",
                                      "LONG BALL",
                                      {"Overview", "The players", "The system", "The mentality", "The legacy"},
                                      {"While most modern teams build out from the back and try to dominate possession, Leicester leaned into what many considered a “primitive” approach: long balls over the top, quick transitions, and minimal touches. It was basic. It was old school. And it worked. Their average possession was around 42%—one of the lowest in the league. But they were efficient. Their average number of passes per goal was the fewest in the Premier League. Leicester weren’t trying to create the perfect goal. They were trying to catch you off guard before you even got back in shape. This was football boiled down to its rawest form: win the ball, go forward, finish the chance.",
                                       "The formation was a rigid and well-drilled 4-4-2, and every player knew their job.\nKasper Schmeichel in goal, capable of launching precise long kicks into space.\nThe backline: Simpson (RB), Morgan (CB), Huth (CB), and Fuchs (LB)—not the fastest, but physical, aggressive, and no-nonsense defenders.\nThe midfield duo of Danny Drinkwater and N'Golo Kanté was the engine. Drinkwater had one of the best long-passing ranges in the league, constantly hitting diagonal balls or lofted passes into space for the forwards. Kanté? He just ran the show. Ball-winner. Space-eater. Transition trigger.\nOn the wings, Albrighton and Mahrez. Albrighton was industrious and precise with crosses; Mahrez was the magician—a left-footed maestro who could cut inside and win games on his own.\nAnd up top: the duo that defined the season—Jamie Vardy and Shinji Okazaki. Vardy was the perfect outlet—rapid, relentless, and ruthless. Okazaki pressed, harried, and created space.",
                                       "Leicester’s system wasn’t about controlling the game—it was about controlling the moments that mattered. Their defensive block was tight, narrow, and disciplined. Opponents were invited to have the ball. Once they committed too many men forward, boom—Leicester struck. It was so effective because it required no buildup, no risk of losing the ball deep. One good pass and one good run—goal. Their pressing was smart too. They didn’t press high all game but would pounce when the time was right. The trigger? A loose touch. A bad pass. A misjudged bounce. Vardy and Okazaki would go in like sharks. And they were clinical. Leicester didn’t create the most chances, but they converted at one of the highest rates in the league.",
                                       "Ranieri gave them belief, but also simplicity. He didn’t try to make them play like Barcelona. He doubled down on what they were good at: defending deep, transitioning fast, and trusting their instinct. They didn’t just survive without the ball—they thrived without it. The team was built on trust, work rate, and discipline. They didn’t try to out-pass you. They tried to out-fight you, outrun you, and outscore you with half the chances.",
                                       "The 15/16 Leicester team proved that football isn’t just about money or possession stats—it’s about knowing your strengths and sticking to them. They took the fundamentals—long balls, two strikers, compact defense—and elevated them to a title-winning level. They didn’t play beautiful football by traditional standards. But they played winning football. And in doing so, they inspired an entire generation of underdogs. Sometimes the simplest ideas are the hardest to beat."},
                                      this));

    //Second Barca
    formations.append(new Formation(":/new/prefix1/Images/secondBarca.png",
                                      ":/new/prefix1/Images/secondBarcaTrophy.jpg",
                                      "The 2014-15 Barcelona team, managed by Luis Enrique, marked a new chapter in not just Barcelona’s legacy, but the history of the sport. The team managed to win La Liga, Copa del Rey, and the UEFA Champions League, becoming the first club in football history to win the treble twice. Barcelona’s club slogan is Mes Que un Club, or more than a club. This is because they don’t just win, they dominate. Barca’s identity isn’t about the trophies they win, it’s always about how they win them. The style of play. They didn’t just win; they dominated. They scored 175 goals in all competitions that season. Establishing their front 3 as the greatest front 3 of all time.",
                                      "TIKI-TAKA",
                                      {"Overview", "The players", "The system"},
                                      {"The idea of the Tiki-Taka originated with Spain’s 2006 team. The idea is simple: the ball travels faster than players can, so pass and don’t risk dribbling too much. Luis Enrique is known for giving his players complete and utter freedom because he trusts them to have the skill to know what they’re doing. This separated his idea of passing triangles from Pep’s. Players like Messi (RW), Suarez (ST), and Neymar (LW) were told to maintain fluid positions so they could hunt for triangles with the midfielders and fullbacks. The midfield was a holding midfield, meaning that there was one 6 and two 8s/makeshift 10s. Busquets played the 6; Iniesta and Rakitic played the 8s. Alba (LB), Macherano (CB), Pique (CB), and Dani Alves (RB) made up the defense.",
                                       "Although on paper they played a 4-3-3, these players were all told be able to play any position. During games you would see Macherano pushing up to play as a CDM, or Alba and Dani Alves playing as outside midfielders or even wingers. Messi would cut in and play CAM sometimes. The players maintained full freedom and flexibility to roam in to be the third player in any passing triangle.",
                                       "Because the players were cutting in and out so often, there were never any rigid players. No one ever stood in their place. This idea of combining constant runs with tiki-taka is what separated Luis Enrique’s side from other Spanish teams. Often times, Tiki Taka meant you controlled possession but it was hard to create chances because it felt like you were just passing for the sake of passing. Because of these constant third man runs, the ball was always progressing and moving forward. It also really helped to have the best striker of his generation, Suarez, and two all-time great wingers, Messi and Neymar, which is why they produced those insane goalscoring numbers. Defensively, they also pressed in waves. Often, the Tiki Taka when not in possession is played like this. You have three players press at a time to force mistakes. "},
                                      this));

    for (Formation* formation : formations) {
        uiVector.append(formation);
    }

    QuizWidget* formationsQuizWidget = new QuizWidget(&formationsQuiz, this);
    connect(formationsQuizWidget, &QuizWidget::quizCompleted, this, &MainWindow::onFormationsQuizCompleted);
    formationsQuizWidget->show();
    uiVector.append(formationsQuizWidget);
    formationsQuizPassed = false;
}
