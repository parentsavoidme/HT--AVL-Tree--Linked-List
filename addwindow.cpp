    #include "addwindow.h"
#include "ui_addwindow.h"
#include <QMessageBox>

addwindow::addwindow(hash_table *hasht, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addwindow)
    , ht(hasht)
{
    ui->setupUi(this);
}

addwindow::~addwindow()
{
    delete ui;
}

void addwindow::on_pushButton_clicked()
{
    bool check;
    QString code = ui->lineEdit->text();
    QString product = ui->lineEdit_2->text();
    QString inn = ui->lineEdit_3->text();
    QString data = ui->lineEdit_4->text();
    unsigned long long code_number = code.toULongLong(&check);
    unsigned long long inn_number = inn.toULongLong(&check);

    int firstDotPos = data.indexOf('.');
    int secondDotPos = data.indexOf('.', firstDotPos + 1);


    QString dayStr = data.left(firstDotPos);
    QString monthStr = data.mid(firstDotPos + 1, secondDotPos - firstDotPos - 1);
    QString yearStr = data.mid(secondDotPos + 1);


    size_t data_day = dayStr.toUInt();
    size_t data_month = monthStr.toUInt();
    size_t data_year = yearStr.toUInt();

    std::string product_pole = product.toStdString();
    Date data_post(data_day, data_month, data_year);
    Pole p(code_number, product_pole, inn_number, data_post);

    if (p.control_pole() == 1 && firstDotPos != -1 && secondDotPos != -1)
    {
        bool t = ht->add(p);
        if (t == 1)
        {
            emit elementAdded(p);
            QMessageBox::information(this, "Window", "Добавилось успешно");
        }
        else
        {
            QMessageBox::warning(this, "Window", "Такой элемент есть");
        }
    }
    else if(p.control_pole()==0)
        QMessageBox::critical(this, "Error", "Неверный формат артикула");
    else if(p.control_pole()==-1)
        QMessageBox::critical(this, "Error", "Неверный формат наименования товара");
    else if(p.control_pole()==-2)
        QMessageBox::critical(this, "Error", "Неверный формат ИНН");
    else if(p.control_pole()==-3 && (firstDotPos != 1 || secondDotPos != 1))
        QMessageBox::critical(this, "Error", "Неверный формат даты");


    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
}

