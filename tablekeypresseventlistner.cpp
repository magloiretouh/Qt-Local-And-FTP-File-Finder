#include "tablekeypresseventlistner.h"

tableKeyPressEventListner::tableKeyPressEventListner(QObject *parent) : QObject(parent)
{

}

bool tableKeyPressEventListner::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        customTableWidget *sender = static_cast<customTableWidget *>(obj);
        if(sender != NULL)
        {
            if(keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return)
            {
                sender->lire();
            } else if(keyEvent->key() == Qt::Key_Delete)
            {
                sender->supp();
            } else if(keyEvent->key() == Qt::Key_F2)
            {
                sender->renommer();
            }
        }
    }
    return false;
}
