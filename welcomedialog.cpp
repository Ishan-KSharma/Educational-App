/**
 * Represents a welcome dialog popup for the Soccer Tactics Teacher,
 * with Box2D used for animating the soccer balls.
 * @author Danny Provo
 * @date 4/7/2025
 */

#include "welcomedialog.h"
#include "ui_welcomedialog.h"
#include "helpers.h"

using Box2DHelpers::pixelXToWorldX;
using Box2DHelpers::pixelYToWorldY;
using Box2DHelpers::worldXToPixelX;
using Box2DHelpers::worldYToPixelY;

WelcomeDialog::WelcomeDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::WelcomeDialog),
    world(b2Vec2(0.0f, -10.0f)), timer(QTimer(this))
{
    ui->setupUi(this);
    setWindowTitle("Welcome!");
    setWindowIcon(QIcon(":/videos/Images/ball.png"));
    // connect signals and slots
    connect(&timer, &QTimer::timeout, this, &WelcomeDialog::handleAnimationTimeout);

    // --- Code below adapted from lab 12 code to allow adding bouncing soccer balls ---

    // Create the ground Body
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);
    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    b2Body* groundBody = world.CreateBody(&groundBodyDef);
    // Define the ground box shape.
    b2PolygonShape groundBox;
    // The extents are the half-widths of the box.
    groundBox.SetAsBox(50.0f, 10.0f);
    // Add the ground fixture to the ground body.
    groundBody->CreateFixture(&groundBox, 0.0f);

    // Get the size of this widget for use in calculations converting from pixel to world coordinates
    widgetSize = this->size();

    // Define the fixuture that the balls will use
    // Define another box shape for our dynamic body.
    b2CircleShape circle;
    circle.m_radius = pixelXToWorldX(ui->Ball1->width()); // Ball widths are same as height and equal to each other
    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circle;
    // Set the ball density to be non-zero, so it will be dynamic.
    fixtureDef.density = 1.0f;
    // Override the default friction.
    fixtureDef.friction = 0.3f;
    // Set restitution (bounciness)
    fixtureDef.restitution = 0.9f;

    // Create ball 1
    b2BodyDef body1Def;
    body1Def.type = b2_dynamicBody;
    // Set the position based on its positon in ui
    QPoint initialPOS = ui->Ball1->pos();
    float ball1X = pixelXToWorldX(initialPOS.x());
    float ball1Y = pixelYToWorldY(initialPOS.y(), widgetSize.height());
    body1Def.position.Set(ball1X, ball1Y);
    ball1 = world.CreateBody(&body1Def);

    // Create ball 2
    b2BodyDef body2Def;
    body2Def.type = b2_dynamicBody;
    // Set the position based on its positon in ui
    initialPOS = ui->Ball2->pos();
    float ball2X = pixelXToWorldX(initialPOS.x());
    float ball2Y = pixelYToWorldY(initialPOS.y(), widgetSize.height());
    body2Def.position.Set(ball2X, ball2Y);
    ball2 = world.CreateBody(&body2Def);

    // Add the ball shape to the balls.
    ball1->CreateFixture(&fixtureDef);
    ball2->CreateFixture(&fixtureDef);

    // Start the timer to show animation
    timer.setInterval(10);
    timer.start();
}

WelcomeDialog::~WelcomeDialog()
{
    delete ui;
}

void WelcomeDialog::handleAnimationTimeout() {
    // Prepare for simulation. Typically we use a time step of 1/60 of a
    // second (60Hz) and 10 iterations. This provides a high quality simulation
    // in most game scenarios.
    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    // Instruct the world to perform a single step of simulation.
    // It is generally best to keep the time step and iterations fixed.
    world.Step(timeStep, velocityIterations, positionIterations);

    // Now set the position of the balls to the new positon
    b2Vec2 ball1POS = ball1->GetPosition();
    int ball1PixelXPOS = worldXToPixelX(ball1POS.x);
    int ball1PixelYPOS = worldYToPixelY(ball1POS.y, widgetSize.height());
    ui->Ball1->move(ball1PixelXPOS, ball1PixelYPOS);

    b2Vec2 ball2POS = ball2->GetPosition();
    int ball2PixelXPOS = worldXToPixelX(ball2POS.x);
    int ball2PixelYPOS = worldYToPixelY(ball2POS.y, widgetSize.height());
    ui->Ball2->move(ball2PixelXPOS, ball2PixelYPOS);
}
