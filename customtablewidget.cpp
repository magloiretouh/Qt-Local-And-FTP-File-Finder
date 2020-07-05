#include "customtablewidget.h"

customTableWidget::customTableWidget(QWidget *parent) :
    QTableWidget(parent)
{
    file = new QFile();
    menu = new QMenu(this);
    fileThread = new FileThread();
    copyAction = new QAction(tr("Copier vers ..."), this);
    cutAction = new QAction(tr("Déplacer vers ..."), this);
    openFileFolderAction = new QAction(tr("Ouvrir l'emplacement du fichier"), this);
    suppAction = new QAction(tr("Supprimer"), this);
    lireAction = new QAction(tr("Lire"), this);
    renommerAction = new QAction(tr("Renommer"), this);
    progressDialog = new QProgressDialog();
    progressDialog->setModal(true);
    progressDialog->setRange(0,100);
    progressDialog->setValue(100);
    progressDialog->setWindowTitle("Advanced File Search");
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copy()));
    connect(openFileFolderAction, SIGNAL(triggered()), this, SLOT(openFileFolder()));
    connect(cutAction, SIGNAL(triggered()), this, SLOT(cut()));
    connect(suppAction, SIGNAL(triggered()), this, SLOT(supp()));
    connect(lireAction, SIGNAL(triggered()), this, SLOT(lire()));
    connect(renommerAction, SIGNAL(triggered()), this, SLOT(renommer()));
    connect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(liredbClick(QModelIndex)));
    connect(fileThread,SIGNAL(progressValue(float)),this,SLOT(updateProgressBar(float)));
}

void customTableWidget::cut()
{
    QString value = QFileDialog::getExistingDirectory(this,tr("Selectionnez le dossier de destination"),"");
    if( value != "")
    {
        QList<QModelIndex> list = this->selectionModel()->selectedRows();
        if(this->item(list.at(0).row(),5)->text().split(QDir::separator()).first() == "ftp:")
        {
            QStack<QString> pile;
            for (int i = 0; i < list.size(); ++i) {
                pile.push_back(this->item(list.at(i).row(),5)->text());
            }
            emit sendFtpFileForCut(pile,value);
        } else {
            progressDialog->setLabel(new QLabel("Déplacement de "+QString::number(list.size())+" éléments"));
            progressDialog->setValue(0);
            progressDialog->show();
            fileThread->setList(list);
            fileThread->setTable(this);
            fileThread->setOperation(QString::number(1));
            fileThread->setNewEmplacement(value);
            fileThread->moveToThread(this->thread());
            fileThread->start();
        }
    }
}

void customTableWidget::copy()
{
    QString value = QFileDialog::getExistingDirectory(this,tr("Selectionnez le dossier de destination"),"");
    if( value != "")
    {
        QList<QModelIndex> list = this->selectionModel()->selectedRows();
        if(this->item(list.at(0).row(),5)->text().split(QDir::separator()).first() == "ftp:")
        {
            QStack<QString> pile;
            for (int i = 0; i < list.size(); ++i) {
                pile.push_back(this->item(list.at(i).row(),5)->text());
            }
            emit sendFtpFileForCopy(pile,value);
        } else {
            progressDialog->setLabel(new QLabel("Copie de "+QString::number(list.size())+" éléments"));
            progressDialog->setValue(0);
            progressDialog->show();
            fileThread->setList(list);
            fileThread->setTable(this);
            fileThread->setOperation(QString::number(0));
            fileThread->setNewEmplacement(value);
            fileThread->moveToThread(this->thread());
            fileThread->start();
        }
    }
}

void customTableWidget::supp()
{
    QList<QModelIndex> list = this->selectionModel()->selectedRows();
    QMessageBox msgBox;
    if(list.size() == 1)
    {
        msgBox.setText(tr("Voulez vous vraiment supprimer cet élément ?"));
    }
    else
    {
        msgBox.setText("Voulez vous vraiment supprimer ces "+QString::number(list.size())+" éléments ?");
    }
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Ok)
    {
        if(this->item(list.at(0).row(),5)->text().split(QDir::separator()).first() == "ftp:")
        {
            QStack<QString> pile;
            for (int i = 0; i < list.size(); ++i) {
                pile.push_back(this->item(list.at(i).row(),5)->text());
            }
            emit sendFtpFileForRemove(pile);
        } else {
            for (int i = 0; i < list.size(); ++i) {
                file->setFileName(this->item(list.at(i).row(),5)->text());
                if(file->exists())
                {
                    file->remove();
                }
             }
        }
    }
}

void customTableWidget::lire()
{
    QList<QModelIndex> list = this->selectionModel()->selectedRows();
    if(this->item(list.at(0).row(),5)->text().split(QDir::separator()).first() == "ftp:")
    {
        QStack<QString> pile;
        for (int i = 0; i < list.size(); ++i) {
            pile.push_back(this->item(list.at(i).row(),5)->text());
        }
        emit sendFtpFileForReading(pile);
    } else {
        for (int i = 0; i < list.size(); ++i) {
            file->setFileName(this->item(list.at(i).row(),5)->text());
            if(file->exists())
            {
                QDesktopServices::openUrl(QUrl::fromLocalFile(file->fileName()));
            }
        }
    }
}

void customTableWidget::openFileFolder()
{
    QFileInfo info;
    QList<QModelIndex> list = this->selectionModel()->selectedRows();
    for (int i = 0; i < list.size(); ++i) {
        file->setFileName(this->item(list.at(i).row(),5)->text());
        if(file->exists())
        {
            info.setFile(this->item(list.at(i).row(),5)->text());
            QDesktopServices::openUrl(QUrl::fromLocalFile(info.absolutePath()));
        }
     }
}

void customTableWidget::contextMenuEvent(QContextMenuEvent *event)
{
    list.clear();
    list.append(lireAction);
    list.append(renommerAction);
    list.append(copyAction);
    list.append(cutAction);
    list.append(suppAction);
    list.append(openFileFolderAction);

    menu->addActions(list);
    menu->exec(event->globalPos());
}

void customTableWidget::liredbClick(QModelIndex index)
{
    lire();
}

void customTableWidget::renommer()
{
    QList<QModelIndex> list = this->selectionModel()->selectedRows();
    if(this->item(list.at(0).row(),5)->text().split(QDir::separator()).first() == "ftp:")
    {
        emit sendFtpFileForRename(list.at(0).row());
    }
    else
    {
        file->setFileName(this->item(list.at(0).row(),5)->text());
        if(file->exists())
        {
            QStringList pathTemp = file->fileName().split(QDir::separator());
            bool ok;
            QStringList filenameTemp = pathTemp.last().split(".");
            filenameTemp.removeLast();
            QString trueFileName = filenameTemp.join(".");
            QString text = QInputDialog::getText(this, tr("Advanced File Search : Renommer"),
                                                       tr("Nouveau nom"), QLineEdit::Normal,
                                                       trueFileName, &ok);
            if(ok && !text.isEmpty() && text!=trueFileName)
            {
                pathTemp.removeLast();
                if(file->rename(pathTemp.join(QDir::separator())+QDir::separator()+text+"."+file->fileName().split(QDir::separator()).last().split(".").last()))
                {
                    QMessageBox::information(this,"Advanced File Search","Modification éffectuée",QMessageBox::Ok);
                    this->setItem(list.at(0).row(),1,new QTableWidgetItem(file->fileName().split(QDir::separator()).last()));
                    this->setItem(list.at(0).row(),5,new QTableWidgetItem(file->fileName()));
                } else
                {
                    QMessageBox::critical(this,"Advanced File Search","Erreur lors de la modification",QMessageBox::Ok);
                }
            }
        }
    }
}

void customTableWidget::updateProgressBar(float value)
{
    progressDialog->setValue(ceil(value));
}

void customTableWidget::setActionOpenParentDirectoryActive(bool ftpIsActive)
{
    this->openFileFolderAction->setEnabled(!ftpIsActive);
}

QAction* customTableWidget::getActionOpenParentDirectory()
{
    return openFileFolderAction;
}

void customTableWidget::setActionRenommer(bool rowSelectedEqualToOne)
{
    this->renommerAction->setEnabled(rowSelectedEqualToOne);
}

