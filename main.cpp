#include "mainwindow.h"
#include "welcomedialog.h"
#include "soundeffect.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SoundEffect *filter = new SoundEffect(&a);
    a.installEventFilter(filter);
    MainWindow w;
    WelcomeDialog welcome;
    w.show();
    if(welcome.exec() == QDialog::Rejected)
        return QDialog::Rejected;

    return a.exec();
}
