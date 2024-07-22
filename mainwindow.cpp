#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "addwindow.h"
#include "deletewindow.h"
#include "searchwindow.h"
#include "hash_table.h"
#include "tree.h"
#include <QMessageBox>
#include <QTreeWidget>
#include <QStandardItemModel>

MainWindow::MainWindow(int htsize, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , ht(new hash_table(htsize))
{
    ui->setupUi(this);


    consoleTextEdit = ui->plainTextEdit;



    //tableWidget
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels((QStringList() << "Артикул" << "Наименование товара" << "ИНН" << "Дата поставки"));
    ui->tableWidget->resizeColumnToContents(1);

    // tableWidget2
     int tableWidget2RowCount = ht->get_size();
     ui->tableWidget2->setRowCount(tableWidget2RowCount); // строки
     ui->tableWidget2->setColumnCount(1); // столбец
     ui->tableWidget2->setHorizontalHeaderLabels(QStringList() << "Состояние");
     ui->tableWidget2->resizeColumnToContents(0);// расположение в column
     ui->tableWidget2->verticalHeader()->hide();

     //tableWidget2_1
     ui->tableWidget2_1->setRowCount(tableWidget2RowCount); // строки
     ui->tableWidget2_1->setColumnCount(1);//столбец
     ui->tableWidget2_1->setHorizontalHeaderLabels(QStringList() << "Индекс");
     ui->tableWidget2_1->resizeColumnToContents(0);
     ui->tableWidget2_1->verticalHeader()->hide(); // расположение в column
     for (int i = 0; i < tableWidget2RowCount; ++i)
     {
         QTableWidgetItem* item = new QTableWidgetItem(QString::number(i));
         ui->tableWidget2_1->setItem(i, 0, item);
     }


    // tableWidget иниицилазирует 0
    for (int i = 0; i < tableWidget2RowCount; ++i)
    {
        QTableWidgetItem* item = new QTableWidgetItem("0");
        ui->tableWidget2->setItem(i, 0, item);
    }

    addwindow* addWin = new addwindow(ht, this);
    connect(addWin, &addwindow::elementAdded, this, &MainWindow::onElementAdded);

    ui->tableWidget2->setVisible(true);
    ui->tableWidget2_1->setVisible(true);
    ui->plainTextEdit->setVisible(true);


     connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::on_pushButton_4_clicked);

    connect(ui->tableWidget2->verticalScrollBar(), &QScrollBar::valueChanged, ui->tableWidget2_1->verticalScrollBar(), &QScrollBar::setValue);
    connect(ui->tableWidget2_1->verticalScrollBar(), &QScrollBar::valueChanged, ui->tableWidget2->verticalScrollBar(), &QScrollBar::setValue);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete ht;
}

void MainWindow::on_pushButton_clicked()
{
    window = new addwindow(ht, this);
    connect(window, &addwindow::elementAdded, this, &MainWindow::addElementToTable);
    window->show();
}

void MainWindow::addElementToTable(const Pole& p)
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

    //проверка в консоли
    unsigned int hf = ht->hashFunction1(p);
    qDebug() << "added: ";
    qDebug() << "hash:" << hf;
    qDebug() << "code:" << codeItem->text();
    qDebug() << "product:" << productItem->text();
    qDebug() << "inn:" << innItem->text();
    qDebug() << "date:" << dateItem->text();

    onElementAdded(p);

}
void MainWindow::on_pushButton_2_clicked()
{
    window1 = new deletewindow(ht, this);
    connect(window1, &deletewindow::elementDeleted, this, &MainWindow::removeElementFromTable);
    window1->show();
}

void MainWindow::removeElementFromTable(const Pole& p)
{
    // - элемента из tableWidget
    int rowToRemove = -1;
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        QTableWidgetItem* codeItem = ui->tableWidget->item(row, 0);
        QTableWidgetItem* productItem = ui->tableWidget->item(row, 1);
        QTableWidgetItem* innItem = ui->tableWidget->item(row, 2);
        QTableWidgetItem* dateItem = ui->tableWidget->item(row, 3);

        if (codeItem && productItem && innItem && dateItem) {
            if (codeItem->text().toULongLong() == p.code &&
                productItem->text().toStdString() == p.product &&
                innItem->text().toULongLong() == p.INN &&
                dateItem->text() == QString("%1.%2.%3").arg(p.data.day).arg(p.data.month).arg(p.data.year)) {

                rowToRemove = row;
                break;
            }
        }
    }

    if (rowToRemove != -1) {
        ui->tableWidget->removeRow(rowToRemove);
    } else {
        qDebug() << "Element not found in tableWidget!";
        return;
    }

    unsigned long long index = ht->hashFunction1(p);
    qDebug() << "Removing element at index:" << index;
    qDebug() << "Status before removal:" << ht->status[index];

    QTableWidgetItem* item = new QTableWidgetItem(QString::number(ht->status[index]));
    ui->tableWidget2->setItem(index, 0, item);

    qDebug() << "Status after removal:" << ht->status[index];

    consoleTextEdit->clear();

    ht->print2(consoleTextEdit);
}

void MainWindow::on_pushButton_3_clicked()
{
    window2 = new searchwindow(ht, this);
    window2->show();
}

void MainWindow::on_pushButton_5_clicked() // загрузка данных с файла
{
    std::ifstream file("C://marketplace.txt");
    if (!file.is_open())
    {
        QMessageBox::critical(this, " Window ", " Файл не открылся ");
        return;
    }

    unsigned long long code;
    unsigned long long inn;
    std::string product;
    size_t day, month, year;
    char dot1, dot2;
    int n = 0;

    while (file >> code >> product >> inn >> day >> dot1 >> month >> dot2 >> year)
    {

        Date d(day, month, year);
        Pole p(code, product, inn, d);
        if(p.control_pole()==1 && dot1=='.' && dot2=='.')
        {
            if (ht->add(p))
            {
                addElementToTable(p);
                n++;
            }
        }
    }
    ht->print2(consoleTextEdit);
    if(n>0)
            QMessageBox::information(this, " Window ", QString(" Успешно добавилось %1 элементов ").arg(n));
    else
        QMessageBox::critical(this, " Window ", QString("Уникальные элементы не добавились").arg(n));

    file.close();
}

void MainWindow::on_pushButton_6_clicked() // выгрузка данных в файл
{
    std::ofstream file("C://marketplace.txt", std::ofstream::out | std::ofstream::trunc);
    if (!file.is_open())
    {
        QMessageBox::critical(this, " Window ", "Файл не открылся");
        return;
    }
    ht->write_file(file);
    QMessageBox::information(this, " Window ", " Элементы успешно записались в файл ");
    file.close();
}


void MainWindow::onElementAdded(const Pole& p)
{
    unsigned long long index = ht->hashFunction1(p);
    int value = ht->get_status(index);
    QTableWidgetItem* item = new QTableWidgetItem(QString::number(value));

    if (index < ui->tableWidget2->rowCount())
        ui->tableWidget2->setItem(index, 0, item);
     else
    {
        ui->tableWidget2->setRowCount(index + 1);
        ui->tableWidget2->setItem(index, 0, item);
    }

    consoleTextEdit->clear();

    ht->print2(consoleTextEdit);
}

void MainWindow::onElementRemoved(const Pole& p)
{
    unsigned long long index = ht->hashFunction1(p);
    qDebug() << "Removing element at index:" << index;
    qDebug() << "Status before removal:" << ht->status[index];

    if (ht->status[index] == 0)
    {
        QTableWidgetItem* item = new QTableWidgetItem("0");
        ui->tableWidget2->setItem(index, 0, item);
    }

    qDebug() << "Status after removal:" << ht->status[index];
}

void MainWindow::on_pushButton_4_clicked()
{

    ui->tableWidget2->setVisible(true);
    ui->tableWidget2_1->setVisible(true);
    ui->plainTextEdit->setVisible(true);
}

void MainWindow::on_pushButton_7_clicked()
{
    ui->tableWidget2->setVisible(false);
    ui->tableWidget2_1->setVisible(false);
    ui->plainTextEdit->setVisible(false);
}

