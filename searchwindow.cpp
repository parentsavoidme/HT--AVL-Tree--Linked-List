#include "searchwindow.h"
#include "ui_searchwindow.h"
#include "QMessageBox"

searchwindow::searchwindow(hash_table *hasht,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::searchwindow)
    , ht(hasht)
{
    ui->setupUi(this);
}

searchwindow::~searchwindow()
{
    delete ui;
}

void searchwindow::on_pushButton_clicked()
{
    bool check;
    QString code = ui->lineEdit->text();
    QString product = ui->lineEdit_2->text();
    //QString inn = ui->lineEdit_3->text();
    QString data = ui->lineEdit_4->text();
    unsigned long long code_number = code.toULongLong(&check);
   // unsigned long long inn_number = inn.toULongLong(&check);

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
    Pole p(code_number, product_pole, 100000000000, data_post);
    if(p.control_pole()==1 &&  firstDotPos != -1 && secondDotPos != -1 )
    {
        for_search s = ht->search(p);
        if(s.found == 1)
            QMessageBox::information(this, "Window",
                                     QString("Такой элемент есть. Количество шагов поиска: %1").arg(s.count));
        if(s.found == 0)
            QMessageBox::warning(this, "Window",
                                 QString("Такого элемента нет. Количество шагов поиска: %1").arg(s.count));
    }
    else if(p.control_pole()==0)
        QMessageBox::critical(this, "Error", "Неверный формат артикула");
    else if(p.control_pole()==-1)
        QMessageBox::critical(this, "Error", "Неверное формат наименования товара");
    else if(p.control_pole()==-3 && (firstDotPos != 1 || secondDotPos != 1))
        QMessageBox::critical(this, "Error", "Неверный формат даты");

    ui->lineEdit->clear();    ui->lineEdit_2->clear();     ui->lineEdit_4->clear();
}

