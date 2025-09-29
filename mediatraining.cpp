#include "mediatraining.h"
#include "ui_mediatraining.h"
#include "quiz.h"
#include <QMessageBox>
#include <QVector>
#include <QString>

MediaTraining::MediaTraining(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MediaTraining)
{
    ui->setupUi(this);
    QPixmap pixmap(":/new/prefix1/Images/mediaTraining.jpg");
    QPixmap scaledPixmap = pixmap.scaled(1000, 1000, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->contentBar->setValue(50);
    ui->pressConferenceImage->setPixmap(scaledPixmap);
    setupQuiz();
    previousQuestionIndex = 0;
    askQuestion();

     connect(ui->option1, &QPushButton::clicked, this, &MediaTraining::handleOption1);
     connect(ui->option2, &QPushButton::clicked, this, &MediaTraining::handleOption2);
     connect(ui->option3, &QPushButton::clicked, this, &MediaTraining::handleOption3);
     connect(ui->option4, &QPushButton::clicked, this, &MediaTraining::handleOption4);
}

MediaTraining::~MediaTraining()
{
    delete ui;
}

void MediaTraining::askQuestion(){
    // Shuffle the question bank, then store an int of the previous question, then just ask the next question in the bank using
    // quiz.getQuestion and Answer, then just put the answer in the corresponding quiz box (0-3) and then check to see if they answered
    // right

    if (previousQuestionIndex >= quiz.size()) {
        quiz.shuffle();
        previousQuestionIndex = 0;
    }

    QVector<QString> options;
    QString questionText = quiz.getQuestionAndOptions(previousQuestionIndex, options, correctIndex);
    previousQuestionIndex++;

    ui->questionText->setText(questionText);

    ui->option1->setText(options[0]);
    ui->option2->setText(options[1]);
    ui->option3->setText(options[2]);
    ui->option4->setText(options[3]);
}

void MediaTraining::handleOption1(){
    if(correctIndex == 0)
        ui->contentBar->setValue(ui->contentBar->value() + 10);
    else{
        if(ui->contentBar->value() < 10)
            ui->contentBar->setValue(0);
        else
            ui->contentBar->setValue(ui->contentBar->value() - 10);
    }
    emitQuizDone();
    changeImage();
    askQuestion();
}

void MediaTraining::handleOption2() {
    if(correctIndex == 1)
        ui->contentBar->setValue(ui->contentBar->value() + 10);
    else{
        if(ui->contentBar->value() < 10)
            ui->contentBar->setValue(0);
        else
            ui->contentBar->setValue(ui->contentBar->value() - 10);
    }
    emitQuizDone();
    changeImage();
    askQuestion();}

void MediaTraining::handleOption3() {
    if(correctIndex == 2)
        ui->contentBar->setValue(ui->contentBar->value() + 10);
    else{
        if(ui->contentBar->value() < 10)
            ui->contentBar->setValue(0);
        else
            ui->contentBar->setValue(ui->contentBar->value() - 10);
    }
    emitQuizDone();
    changeImage();
    askQuestion();
}

void MediaTraining::handleOption4() {
    if(correctIndex == 3)
        ui->contentBar->setValue(ui->contentBar->value() + 10);
    else{
        if(ui->contentBar->value() < 10)
            ui->contentBar->setValue(0);
        else
            ui->contentBar->setValue(ui->contentBar->value() - 10);
    }
    emitQuizDone();
    changeImage();
    askQuestion();
}

void MediaTraining::changeImage(){
    if(ui->contentBar->value() <= 40){
        QPixmap pixmap(":/new/prefix1/Images/respectMourinho.jpg");
        QPixmap scaledPixmap = pixmap.scaled(1000, 1000, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->pressConferenceImage->setPixmap(scaledPixmap);
    }else{
        QPixmap pixmap(":/new/prefix1/Images/mediaTraining.jpg");
        QPixmap scaledPixmap = pixmap.scaled(1000, 1000, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->pressConferenceImage->setPixmap(scaledPixmap);
    }
}
void MediaTraining::emitQuizDone(){
    if(ui->contentBar->value() >= 80){
        QMessageBox::information(this, "Quiz Passed!", "Congratulations! You have successfully made it past the media!!!");
        emit mediaQuizPassed();
    }
}

void MediaTraining::setupQuiz(){
    quiz = Quiz({{
        {"You’ve now lost 4 in a row. Is your job under threat?",
         "I focus on the team and our \nperformance — that’s all I can control.",
         "I’m doing my best, and hopefully \nthat’s enough.",
         "That’s a question for the board,\nnot for me.",
         "I’m here until they tell me \notherwise — that’s football."},

        {"Do you still have the dressing room?",
         "The players have been fully \ncommitted, even through the tough results.",
         "We’ve had some talks — it’s \nnot been easy.",
         "There’s frustration, of course, \nbut the group’s not fractured.",
         "I think respect is still there — \nwe’ll see how this week goes."},

        {"The fans are turning. Can you understand their frustration?",
         "Completely — they care deeply, \nand we haven’t delivered results.",
         "They’re emotional — that’s how \nfans are.",
         "We hear them — and we take that \nseriously.",
         "I understand the noise, but we \nhave to block it out right now."},

        {"Your striker hasn’t scored in 9 games. Why keep starting him?",
         "He’s still creating space and \nchances — he contributes even when he’s not scoring.",
         "We’re hoping he’ll find form \nsometime soon.",
         "He offers more than goals — his \npressing, his hold-up play are crucial.",
         "We trust that the goals will come \nif he keeps working."},

        {"Why did you bench your captain today?",
         "It was purely tactical based on \nhow we wanted to approach this opponent.",
         "He wasn’t at his best lately, \nthat’s all.",
         "He’s still a leader, whether on \nthe pitch or not.",
         "Everyone knows rotation is part of \nmodern football."},

        {"Why does your team struggle to score?",
         "We’re creating the right types \nof chances — the finishing will come.",
         "We’ve been unlucky in some games, \nto be honest.",
         "We need more composure in those \nfinal third moments.",
         "Execution has been an issue, but \nwe’re addressing it in training."},

        {"Are you still the right person for this job?",
         "I believe in the vision and the \ngroup — I’m fully committed.",
         "I’ve been through worse, so \nmaybe I can turn this too.",
         "I wouldn’t be here if I didn’t \nthink I could lead us through this.",
         "That’s for others to decide, \nbut my focus hasn’t changed."},

        {"Why didn’t you go for the win today?",
         "We made choices to control the \ngame based on the state of play.",
         "We played it safe and stuck \nto our shape.",
         "We felt the point was valuable \ngiven recent results.",
         "The structure we had was working \n— no need to force it."},

        {"Your young midfielder looked frustrated coming off. Is he happy?",
         "Young players are passionate — \nit’s good that they care.",
         "He’ll be fine. He’s just young \nand emotional.",
         "We spoke after the match — \nit’s all clear.",
         "I understand his emotions, but \ndecisions are for the team."},

        {"How do you respond to fans calling for your sacking?",
         "They’re passionate and want results \n— I get that completely.",
         "They can say what they want, \nI can’t stop them.",
         "My job is to lead through the noise, \nnot react to it.",
         "That kind of pressure only \nmotivates me more."},

        {"The opposition dominated possession — were you outcoached?",
         "They had more of the ball, but we \nlimited their chances.",
         "They played better — that’s \njust how it went.",
         "We had a plan, and parts of it \nworked, others didn’t.",
         "They were better in some phases — \nthat’s fair to say."},

        {"Why do you keep changing the starting XI?",
         "We make decisions based on performance, \nfitness, and tactical needs.",
         "We’re still figuring out \nour best lineup.",
         "Rotation is essential, especially in \nthis fixture load.",
         "Every selection has a reason \nbehind it."},

        {"Has the board given you a deadline to turn things around?",
         "Our conversations are constructive — \nwe’re aligned in the process.",
         "I haven’t been told anything \nyet, which is good.",
         "I’ve not been given ultimatums — \nit’s a collective push.",
         "I can’t speak for the board, but \nI know the expectations."},

        {"Your goalkeeper made another mistake — will he be dropped?",
         "We win and lose as a team, and we \nsupport our players through tough spells.",
         "He needs to focus better \nnext time.",
         "Everyone has dips in form — it’s \nhow you respond.",
         "We’ll assess things internally and \nmake the right call."},

        {"Are the players still behind your methods?",
         "Yes — the effort and professionalism \nI see daily prove that.",
         "I think so — they’ve not said \notherwise.",
         "There’s always noise, but the group \nis focused.",
         "Doubt exists outside, not inside \nthe dressing room."},

        {"Are you worried about relegation?",
         "We’re aware of the table, but we’re \nfocused on performances.",
         "It’s not ideal — let’s see \nhow it goes.",
         "We’re not in denial — it’s a challenge, \nand we’re meeting it.",
         "We need to earn our way out of \nthis position."},

        {"What did you make of the referee’s performance?",
         "I prefer to focus on what we can control \n— our own performance.",
         "Not great, to be honest — but \nI won’t say more.",
         "The officiating’s not something I \nwant to dwell on.",
         "You’ll always get calls you like \nand ones you don’t."},

        {"Why are you sticking with this formation?",
         "It suits the strengths of the players \nwe have available.",
         "We’ve used it before — not \nmuch to change.",
         "We’ve seen improvement even if \nresults aren’t there yet.",
         "We’re evolving — it’s not rigid, \nbut we have a base."},

        {"Your opponent today had half your budget — and still won. Why?",
         "They played well, and we didn’t \nexecute our plan effectively.",
         "That happens sometimes — it’s \njust football.",
         "They were sharper in key moments, \nsimple as that.",
         "We lacked intensity in the decisive \nphases — they punished us."},

        {"You seemed angry on the touchline. Why?",
         "Emotions run high — I care deeply \nabout what we’re building.",
         "I just don’t like losing.",
         "I live every moment with the players \n— it matters to me.",
         "That was passion — not anger."},

        {"Are there cliques forming in the squad?",
         "The group is united — it’s natural \nto have smaller dynamics, but we’re one team.",
         "Some players stick together more — \nit happens.",
         "There’s nothing disruptive — \njust personalities.",
         "If anything arises, we address it \nimmediately."},

        {"Why is your defense conceding so many goals?",
         "We’ve been working on structure and \ncompactness — it takes time.",
         "We’ve had bad luck lately, \nthat’s all.",
         "Some of it’s individual errors — \nbut that’s football.",
         "We need to manage transitions better \n— that’s the next step."},

        {"Did you lose the tactical battle today?",
         "They executed well, but I stand \nby the way we set up.",
         "They were more organized, \nit’s that simple.",
         "We were outplayed in moments, \nnot in ideas.",
         "Sometimes a plan doesn’t translate \nto the pitch."},

        {"Why isn’t your marquee signing starting?",
         "He’s adapting — we’re not rushing \nthat process.",
         "He’s not match fit yet \nfrom what we see.",
         "He’ll play a big role this season \n— no doubt.",
         "We manage players to protect their \nlong-term impact."},

        {"Will you be active in the January window?",
         "We’ll assess and make smart decisions \nif opportunities arise.",
         "It depends if we’re still in \nthe race then.",
         "We’re always monitoring the market.",
         "Only if it aligns with the project \nand the budget."}
    }}, 25);



    quiz.shuffle();
}
