#include "hash_table.h"

void hash_table::print(QTreeWidget* treeWidget) const {
    treeWidget->clear(); // Очищаем виджет перед заполнением

    for (int i = 0; i < size; ++i) {
        if (status[i] == 1) {
            table[i]->print(treeWidget->invisibleRootItem()); // Передаем корневой элемент QTreeWidget
        }
    }
}
