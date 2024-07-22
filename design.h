#ifndef DESIGN_H
#define DESIGN_H

#include <QDialog>

namespace Ui {
class design;
}

class design : public QDialog
{
    Q_OBJECT

public:
    explicit design(QWidget *parent = nullptr);
    ~design();

private:
    Ui::design *ui;
};

#endif // DESIGN_H
