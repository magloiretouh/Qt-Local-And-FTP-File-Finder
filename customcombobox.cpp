#include "customcombobox.h"

customComboBox::customComboBox(QWidget *parent) :
    QComboBox(parent)
{
    add = new QAction(tr("Ajouter"), this);
    modify = new QAction(tr("Modifier"), this);
    supp = new QAction(tr("Supprimer"), this);
    menu = new QMenu(this);
}

void customComboBox::contextMenuEvent(QContextMenuEvent *event)
{
    list.clear();
    list.append(add);
    list.append(modify);
    list.append(supp);
    menu->addActions(list);
    menu->exec(event->globalPos());
}

QAction *customComboBox::getAddAction()
{
    return this->add;
}

QAction *customComboBox::getAlterAction()
{
    return this->modify;
}

QAction *customComboBox::getDeleteAction()
{
    return this->supp;
}
