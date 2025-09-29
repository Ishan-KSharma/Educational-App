#include <QObject>
#include <QPushButton>
#include <QSoundEffect>

/**
 * This class provides a way to add custom sound effects to the application.
 * Eevry single interaction with the UI with go through this class, which makes
 * this class act as a "filter". The class will then check what kind of object
 * was interacted with (e.g. a button) and based on that, a sound effect will be played.
 *
 * @author Rishabh Saini
 * @version 24th April, 2025
 */
class SoundEffect : public QObject {
    Q_OBJECT
public:
    explicit SoundEffect(QObject *parent = nullptr);

protected:
    /**
     * Acts as a "filter" for every UI interaction.
     * Based on what kind of UI interaction it was, this method
     * can play different sound effects.
     * @param obj The UI object interacted with
     * @param event The UI event
     * @return true/false depending on whether the event was passed through.
     */
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QSoundEffect clickSound;
};
