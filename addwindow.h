#ifndef ADDWINDOW_H
#define ADDWINDOW_H
#include "hash_table.h"
#include <QDialog>
#include <QPlainTextEdit>
namespace Ui {
class addwindow;
}

class addwindow : public QDialog
{
    Q_OBJECT

public:
    addwindow(hash_table *hasht, QWidget *parent = nullptr);
    ~addwindow();

signals:
    void elementAdded( Pole& p);

private slots:
    void on_pushButton_clicked();

private:
    Ui::addwindow *ui;
    hash_table *ht;
    QPlainTextEdit* consoleTextEdit;
};

#endif // ADDWINDOW_H
