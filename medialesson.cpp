#include "medialesson.h"
#include "ui_medialesson.h"

mediaLesson::mediaLesson(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mediaLesson)
{
    ui->setupUi(this);
}

mediaLesson::~mediaLesson()
{
    delete ui;
}
