#ifndef CUSTOMTABLEWIDGET_H
#define CUSTOMTABLEWIDGET_H

#include "filethread.h"
#include <QTableWidget>
#include <QMainWindow>
#include <QFileDialog>
#include <QDesktopServices>
#include <QAction>
#include <QMenu>
#include <QEvent>
#include <QDebug>
#include <QUrl>
#include <QStack>
#include <QFileInfo>
#include <QMessageBox>
#include <QContextMenuEvent>
#include <QProgressDialog>
#include <QLabel>
#include <QInputDialog>

class customTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit customTableWidget(QWidget *parent = 0);
    void setActionOpenParentDirectoryActive(bool ftpIsActive);
    void setActionRenommer(bool rowSelectedEqualToOne);
    QAction* getActionOpenParentDirectory();
    
signals:
    void sendFtpFileForReading(QStack<QString>);
    void sendFtpFileForCopy(QStack<QString>,QString);
    void sendFtpFileForCut(QStack<QString>,QString);
    void sendFtpFileForRemove(QStack<QString>);
    void sendFtpFileForRename(int);
public slots:
    void copy();
    void cut();
    void openFileFolder();
    void supp();
    void lire();
    void renommer();
    void liredbClick(QModelIndex index);
    void updateProgressBar(float value);

protected:
    void contextMenuEvent(QContextMenuEvent *event);
    QAction *copyAction;
    QAction *cutAction;
    QAction *openFileFolderAction;
    QAction *suppAction;
    QAction *lireAction;
    QAction *renommerAction;
    QProgressDialog *progressDialog;
    QFile *file;
    QList<QAction *> list;
    QMenu *menu;
    FileThread *fileThread;
    
};

#endif // CUSTOMTABLEWIDGET_H
