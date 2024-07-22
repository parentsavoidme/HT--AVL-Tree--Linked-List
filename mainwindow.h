#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include "hash_table.h"
#include "addwindow.h"
#include "deletewindow.h"
#include "searchwindow.h"
#include"tree.h"
#include <QtWidgets>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(int htsize, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void addElementToTable(const Pole& p);
    void removeElementFromTable(const Pole& p);
    void onElementAdded(const Pole& p);
    void onElementRemoved(const Pole& p);
    void on_pushButton_4_clicked();


    void on_pushButton_7_clicked();

private:
    Ui::MainWindow *ui;
    hash_table* ht;
    addwindow* window;
    deletewindow* window1;
    searchwindow* window2;
    QPlainTextEdit *consoleTextEdit;

};

#endif // MAINWINDOW_H
