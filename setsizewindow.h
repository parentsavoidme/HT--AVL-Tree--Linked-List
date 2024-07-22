#ifndef SETSIZEWINDOW_H
#define SETSIZEWINDOW_H

#include <QDialog>

namespace Ui {
class setsizewindow;
}

class setsizewindow : public QDialog
{
    Q_OBJECT

public:
    explicit setsizewindow(QWidget *parent = nullptr);
    ~setsizewindow();
signals:
    void sizeht(int size);

private slots:
    void on_pushButton_clicked();

private:
    Ui::setsizewindow *ui;
};

#endif // SETSIZEWINDOW_H
