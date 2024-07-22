#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include "hash_table.h"
#include <QDialog>
#include "ui_data_structure.h" // Включение сгенерированного заголовочного файла

namespace Ui {
class data_structure;
}

class data_structure : public QDialog
{
    Q_OBJECT

public:
    explicit data_structure(hash_table *hasht, QWidget *parent = nullptr);
    ~data_structure();

public slots:
    void addElementToTable(const Pole& p); // Слот для добавления элемента в таблицу

private:
    Ui::data_structure *ui; // Используйте Ui::data_structure для доступа к элементам интерфейса
    hash_table *ht;
};

#endif // DATA_STRUCTURE_H
