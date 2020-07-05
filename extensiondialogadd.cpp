#include "extensiondialogadd.h"
#include "ui_extensiondialogadd.h"

ExtensionDialogAdd::ExtensionDialogAdd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExtensionDialogAdd)
{
    ui->setupUi(this);
    this->setFixedSize(410,170);
    connect(ui->lineEditNewExtensionGroup,SIGNAL(textChanged(QString)),this,SLOT(enableOkBtn(QString)));
}

ExtensionDialogAdd::~ExtensionDialogAdd()
{
    delete ui;
}

void ExtensionDialogAdd::enableOkBtn(QString value)
{
    if(value!="" && !value.split(",").contains("") && ui->lineEdit->text()!="")
    {
        ui->dialogOkBtn->setEnabled(true);
    }
    else
    {
        ui->dialogOkBtn->setEnabled(false);
    }
}
