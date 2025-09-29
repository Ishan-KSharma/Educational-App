#include "soundeffect.h"
#include <QEvent>

SoundEffect::SoundEffect(QObject *parent) : QObject(parent) {
        clickSound.setSource(QUrl("qrc:/videos/Images/buttonClick.wav"));
        clickSound.setVolume(0.18f);
}

bool SoundEffect::eventFilter(QObject *obj, QEvent *event) {
        // Check if the event was a button press
        if (event->type() == QEvent::MouseButtonPress) {
            // if it was a button press, then check if the object being passes is a button
            QPushButton *btn = qobject_cast<QPushButton *>(obj);
            if (btn) {
                //if it was a button, then play the click sound effect
                clickSound.play();
            }
        }
        return QObject::eventFilter(obj, event);  // Let the event pass through
}

