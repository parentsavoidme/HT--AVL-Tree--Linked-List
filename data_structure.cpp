#include "mainwindow.h"
#include "data_structure.h"
#include "ui_data_structure.h"

data_structure::data_structure(hash_table* hasht, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::data_structure)
    , ht(hasht)
{
    ui->setupUi(this);

    // Установка количества строк и столбцов для таблицы
    ui->tableWidget->setRowCount(0); // Начальное количество строк - 0
    ui->tableWidget->setColumnCount(4); // Четыре столбца

    // Установка заголовков столбцов
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Артикул" << "Наименование товара" << "ИНН" << "Дата поставки");

    // Соединяем сигнал из MainWindow с соответствующим слотом в data_structure
    connect(parent, SIGNAL(elementAddedToDataStructure(const Pole&)), this, SLOT(addElementToTable(const Pole&)));
}

data_structure::~data_structure()
{
    delete ui;
}

void data_structure::addElementToTable(const Pole& p)
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    QTableWidgetItem* codeItem = new QTableWidgetItem(QString::number(p.code));
    QTableWidgetItem* productItem = new QTableWidgetItem(QString::fromStdString(p.product));
    QTableWidgetItem* innItem = new QTableWidgetItem(QString::number(p.INN));
    QTableWidgetItem* dateItem = new QTableWidgetItem(QString("%1.%2.%3").arg(p.data.day).arg(p.data.month).arg(p.data.year));

    ui->tableWidget->setItem(row, 0, codeItem);
    ui->tableWidget->setItem(row, 1, productItem);
    ui->tableWidget->setItem(row, 2, innItem);
    ui->tableWidget->setItem(row, 3, dateItem);
}
