#ifndef EXTENSIONDIALOGADD_H
#define EXTENSIONDIALOGADD_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class ExtensionDialogAdd;
}

class ExtensionDialogAdd : public QDialog
{
    Q_OBJECT
    
public:
    explicit ExtensionDialogAdd(QWidget *parent = 0);
    ~ExtensionDialogAdd();

public slots:
    void enableOkBtn(QString value);
    
private:
    Ui::ExtensionDialogAdd *ui;

};

#endif // EXTENSIONDIALOGADD_H
