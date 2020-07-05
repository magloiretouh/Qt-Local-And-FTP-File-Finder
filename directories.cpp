#include "directories.h"
#include "ui_directories.h"

directories::directories(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::directories)
{
    ui->setupUi(this);
}

directories::~directories()
{
    delete ui;
}
