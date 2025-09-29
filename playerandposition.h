#ifndef PLAYERANDPOSITION_H
#define PLAYERANDPOSITION_H

#include <QWidget>

namespace Ui {
/**
 * This class represents the player and positions page.
 *
 * @author: John Laird
 * @version: 24th April, 2025
 */
class PlayerandPosition;
}

class PlayerandPosition : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerandPosition(QWidget *parent = nullptr);
    ~PlayerandPosition();

private:
    Ui::PlayerandPosition *ui;
};

#endif // PLAYERANDPOSITION_H
