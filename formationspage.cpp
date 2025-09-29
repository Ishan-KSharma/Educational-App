#include "formationspage.h"
#include "ui_formationspage.h"

FormationsPage::FormationsPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormationsPage)
{
    ui->setupUi(this);



    QPixmap fergusonPixmap(":/new/prefix1/Images/ferguson.jpg");
    QPixmap scaledPixmap = fergusonPixmap.scaled(170, 170, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->ferguson->setPixmap(scaledPixmap);

    QPixmap kloppPixmap(":/new/prefix1/Images/klopp.jpeg");
    scaledPixmap = kloppPixmap.scaled(170, 170, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->klopp->setPixmap(scaledPixmap);

    QPixmap simeonePixmap(":/new/prefix1/Images/simeone.jpg");
    scaledPixmap = simeonePixmap.scaled(170, 170, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->simeone->setPixmap(scaledPixmap);

    QPixmap contePixmap(":/new/prefix1/Images/conte.jpeg");
    scaledPixmap = contePixmap.scaled(170, 170, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->conte->setPixmap(scaledPixmap);

    QPixmap alonsoPixmap(":/new/prefix1/Images/alonso.jpg");
    scaledPixmap = alonsoPixmap.scaled(170, 170, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->alonso->setPixmap(scaledPixmap);

    QPixmap cruyffPixmap(":/new/prefix1/Images/cruyff.jpg");
    scaledPixmap = cruyffPixmap.scaled(170, 170, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->cruyff->setPixmap(scaledPixmap);

    QPixmap guardiolaPixmap(":/new/prefix1/Images/guardiola.jpg");
    scaledPixmap = guardiolaPixmap.scaled(170, 170, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->guardiola->setPixmap(scaledPixmap);

    QPixmap ancelottiPixmap(":/new/prefix1/Images/ancelotti.jpg");
    scaledPixmap = ancelottiPixmap.scaled(170, 170, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->ancelotti->setPixmap(scaledPixmap);

    QPixmap zidanePixmap(":/new/prefix1/Images/zidane.jpg");
    scaledPixmap = zidanePixmap.scaled(170, 170, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->zidane->setPixmap(scaledPixmap);

    QPixmap mourinhoPixmap(":/new/prefix1/Images/mourinho.jpg");
    scaledPixmap = mourinhoPixmap.scaled(170, 170, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->mourinho->setPixmap(scaledPixmap);

    QPixmap firstBarcaPixmap(":/new/prefix1/Images/barca.png");
    scaledPixmap = firstBarcaPixmap.scaled(170, 170, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->firstBarca->setPixmap(scaledPixmap);

    QPixmap madridPixmap(":/new/prefix1/Images/madrid.png");
    scaledPixmap = madridPixmap.scaled(170, 170, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->madrid->setPixmap(scaledPixmap);

    QPixmap interPixmap(":/new/prefix1/Images/inter.png");
    scaledPixmap = interPixmap.scaled(170, 170, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->inter->setPixmap(scaledPixmap);

    QPixmap leicesterPixmap(":/new/prefix1/Images/leicester.png");
    scaledPixmap = leicesterPixmap.scaled(170, 170, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->leicester->setPixmap(scaledPixmap);

    QPixmap liverpoolPixmap(":/new/prefix1/Images/liverpool.png");
    scaledPixmap = liverpoolPixmap.scaled(170, 170, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->liverpool->setPixmap(scaledPixmap);

    QPixmap secondBarcaPixmap(":/new/prefix1/Images/barca.png");
    scaledPixmap = secondBarcaPixmap.scaled(170, 170, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->secondBarca->setPixmap(scaledPixmap);

    QPixmap ajaxPixmap(":/new/prefix1/Images/ajax.png");
    scaledPixmap = ajaxPixmap.scaled(170, 170, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->ajax->setPixmap(scaledPixmap);

}

FormationsPage::~FormationsPage()
{
    delete ui;
}

void FormationsPage::on_fergusonButton_clicked() {
    emit fergusonClicked();
}

void FormationsPage::on_kloppButton_clicked() {
    emit kloppClicked();
}

void FormationsPage::on_guardiolaButton_clicked() {
    emit guardiolaClicked();
}

void FormationsPage::on_ancelottiButton_clicked() {
    emit ancelottiClicked();
}

void FormationsPage::on_mourinhoButton_clicked() {
    emit mourinhoClicked();
}

void FormationsPage::on_simeoneButton_clicked() {
    emit simeoneClicked();
}

void FormationsPage::on_cruyffButton_clicked() {
    emit cruyffClicked();
}

void FormationsPage::on_zidaneButton_clicked() {
    emit zidaneClicked();
}

void FormationsPage::on_alonsoButton_clicked() {
    emit alonsoClicked();
}

void FormationsPage::on_conteButton_clicked() {
    emit conteClicked();
}

void FormationsPage::on_firstBarcaButton_clicked() {
    emit firstBarcaClicked();
}

void FormationsPage::on_madridButton_clicked() {
    emit madridClicked();
}

void FormationsPage::on_interButton_clicked() {
    emit interClicked();
}

void FormationsPage::on_ajaxButton_clicked() {
    emit ajaxClicked();
}

void FormationsPage::on_liverpoolButton_clicked() {
    emit liverpoolClicked();
}

void FormationsPage::on_leicesterButton_clicked() {
    emit leicesterClicked();
}

void FormationsPage::on_secondBarcaButton_clicked() {
    emit secondBarcaClicked();
}
