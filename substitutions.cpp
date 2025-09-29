#include "substitutions.h"
#include "ui_substitutions.h"

Substitutions::Substitutions(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Substitutions)
{
    ui->setupUi(this);
}

Substitutions::~Substitutions()
{
    delete ui;
}
