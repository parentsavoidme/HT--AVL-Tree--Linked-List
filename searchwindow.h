#ifndef SEARCHWINDOW_H
#define SEARCHWINDOW_H
#include "hash_table.h"
#include <QDialog>

namespace Ui {
class searchwindow;
}

class searchwindow : public QDialog
{
    Q_OBJECT

public:
     searchwindow(hash_table *hasht,QWidget *parent = nullptr);
    ~searchwindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::searchwindow *ui;
    hash_table *ht;
};

#endif // SEARCHWINDOW_H
