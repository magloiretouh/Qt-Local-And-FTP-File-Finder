#ifndef CUSTOMCOMBOBOX_H
#define CUSTOMCOMBOBOX_H

#include <QComboBox>
#include <QMenu>
#include <QContextMenuEvent>
#include <QDebug>

class customComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit customComboBox(QWidget *parent = 0);
    QAction *getAddAction();
    QAction *getAlterAction();
    QAction *getDeleteAction();
signals:

public slots:

protected:
    void contextMenuEvent(QContextMenuEvent *event);
private:
    QMenu *menu;
    QAction *add;
    QAction *modify;
    QAction *supp;
    QList<QAction *> list;
    
};

#endif // CUSTOMCOMBOBOX_H
