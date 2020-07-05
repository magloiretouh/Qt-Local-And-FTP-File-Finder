#include "filethread.h"

FileThread::FileThread()
{
    file = new QFile();
    createdFile = new QFile();
    m_tableResult = new QTableWidget();
}

void FileThread::setList(QList<QModelIndex> &selectionList)
{
    this->list = selectionList;
}

void FileThread::setOperation(QString operation)
{
    this->fileOperation = operation;
}

void FileThread::setTable(QTableWidget *tableResult)
{
    this->m_tableResult = tableResult;
}

void FileThread::setNewEmplacement(QString &newEmplacement)
{
    this->fileNewEmplacement = newEmplacement;
}

void FileThread::run()
{
    float moyen = (1/(float)list.size())*100;
    for (int i = 0; i < list.size(); ++i) {
        createdFile->setFileName(fileNewEmplacement+QDir::separator()+m_tableResult->item(list.at(i).row(),1)->text());
        file->setFileName(m_tableResult->item(list.at(i).row(),5)->text());
        if(this->fileOperation == "0" &&
                file->exists())
        {
            copy(file,createdFile,moyen,i);
        }
        else if(this->fileOperation == "1" &&
                file->exists())
        {
            copy(file,createdFile,moyen,i);
            file->remove();
        }
     }
}

void FileThread::copy(QFile *fromFile, QFile *toFile, float moyen, qint64 current)
{
    qint64 nCopySize = fromFile->size();
    fromFile->open(QIODevice::ReadOnly);
    toFile->open(QIODevice::WriteOnly);
    int i(1024*1024),u(1);
    fromFile->seek(0);
    toFile->seek(0);
    do {
         if(toFile->write(fromFile->read(i)) == -1){
             break;
         }
         fromFile->seek(i*u);
         toFile->seek(i*u);
         u++;
         emit progressValue((moyen*((toFile->size()/(float)nCopySize)*100))/100+(current*moyen));
    } while(toFile->size() < nCopySize);

    fromFile->close();
    toFile->close();
}
