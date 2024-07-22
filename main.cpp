#include "mainwindow.h"
#include "setsizewindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    int htSize = -1;

    while (htSize <= 0 || htSize > 1000000) {
        setsizewindow s;


        QObject::connect(&s, &setsizewindow::sizeht, [&](int size) {
            htSize = size;
        });

        if (s.exec() != QDialog::Accepted) {
            return 0;
        }
    }

    MainWindow w(htSize);
    w.show();
    return a.exec();
}
