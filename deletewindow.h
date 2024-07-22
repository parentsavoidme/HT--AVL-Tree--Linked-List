#ifndef DELETEWINDOW_H
#define DELETEWINDOW_H
#include "hash_table.h"
#include <QDialog>
#include <QPlainTextEdit>
namespace Ui {
class deletewindow;
}

class deletewindow : public QDialog
{
    Q_OBJECT

public:
    deletewindow(hash_table *hasht,QWidget *parent = nullptr);
    ~deletewindow();

signals:
    void elementDeleted(const Pole& p);
private slots:
    void on_pushButton_clicked();

private:
    Ui::deletewindow *ui;
    hash_table *ht;
    QPlainTextEdit* consoleTextEdit;
};

#endif // DELETEWINDOW_H
