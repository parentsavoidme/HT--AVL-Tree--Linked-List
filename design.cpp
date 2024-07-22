#include "design.h"
#include "ui_design.h"

design::design(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::design)
{
    ui->setupUi(this);
}

design::~design()
{
    delete ui;
}
