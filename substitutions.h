#ifndef SUBSTITUTIONS_H
#define SUBSTITUTIONS_H

#include <QWidget>

/**
 * This class is for the ui page for the
 * substitution lesson. The actual layout can be
 * found in the substitutions.ui file.
 *
 * @author Brigham Inkley
 * @version 24th April 2025
 */

namespace Ui {
class Substitutions;
}

class Substitutions : public QWidget
{
    Q_OBJECT

public:
    explicit Substitutions(QWidget *parent = nullptr);
    ~Substitutions();

private:
    Ui::Substitutions *ui;
};

#endif // SUBSTITUTIONS_H
