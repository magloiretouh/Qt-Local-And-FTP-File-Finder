#ifndef TABLEKEYPRESSEVENTLISTNER_H
#define TABLEKEYPRESSEVENTLISTNER_H

#include <QObject>
#include <QEvent>
#include <QKeyEvent>
#include "customtablewidget.h"

class tableKeyPressEventListner : public QObject
{
    Q_OBJECT
public:
    explicit tableKeyPressEventListner(QObject *parent = 0);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

signals:

public slots:
};

#endif // TABLEKEYPRESSEVENTLISTNER_H
