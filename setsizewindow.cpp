#include "setsizewindow.h"
#include "ui_setsizewindow.h"
#include <QMessageBox>

setsizewindow::setsizewindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::setsizewindow)
{
    ui->setupUi(this);
}

setsizewindow::~setsizewindow()
{
    delete ui;
}

void setsizewindow::on_pushButton_clicked()
{
    bool ok;
    int size = ui->lineEdit->text().toInt(&ok);
    if (ok && size>0 && size<=1000000) {
        emit sizeht(size);
        accept();
    } else {
        QMessageBox::critical(this, "Error", " Число некорректно ");
    }
}

