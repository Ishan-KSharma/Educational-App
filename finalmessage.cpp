/**
 * Final screen is a coaching license, this handles that entire screen
 * @author Ishan Sharma
 * @date 4/24/2025
 */
#include "finalmessage.h"
#include "ui_finalmessage.h"
#include <QPixmap>
#include <QDate>

FinalMessage::FinalMessage(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FinalMessage)
{
    ui->setupUi(this);

    QPixmap background(":/new/prefix1/Images/finalPicture.jpg");
    ui->imageLabel->setPixmap(background);
    ui->imageLabel->setScaledContents(true);

    // Style the name display
    ui->nameDisplay->setStyleSheet("QLabel { background-color: white; color: black; font-size: 24px; }");
    ui->nameDisplay->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);

    ui->dateDisplay->setStyleSheet("QLabel { background-color: white; color: black; font-size: 14px; }");
    QDate today = QDate::currentDate();

    ui->dateDisplay->setText(today.toString("MM/dd/yyyy"));
}

FinalMessage::~FinalMessage()
{
    delete ui;
}

void FinalMessage::on_submitButton_clicked()
{
    QString name = ui->nameInput->text();
    ui->nameDisplay->setText(name);
    ui->nameDisplay->raise();
}
