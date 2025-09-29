#ifndef SUBSTITUTIONS_H
#define SUBSTITUTIONS_H

#include <QWidget>

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
