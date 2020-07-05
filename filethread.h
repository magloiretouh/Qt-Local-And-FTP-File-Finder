#ifndef FILETHREAD_H
#define FILETHREAD_H

#include <QThread>
#include <QObject>
#include <QFile>
#include <QTableWidget>
#include <QDebug>
#include <QDir>

class FileThread : public QThread
{
    Q_OBJECT

public:
    FileThread();
    void run();
    void setList(QList<QModelIndex> &selectionList);
    void setTable(QTableWidget *tableResult);
    void setOperation(QString operation);
    void setNewEmplacement(QString &newEmplacement);
    void copy(QFile *fromFile, QFile *toFile, float moyen, qint64 current);

signals:
    void progressValue(float value);

private:
    QList<QModelIndex> list;
    QString fileOperation, fileNewEmplacement;
    QFile *file, *createdFile;
    QTableWidget *m_tableResult;
};

#endif // FILETHREAD_H
