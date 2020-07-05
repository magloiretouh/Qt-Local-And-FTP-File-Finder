#ifndef WINDOWSS_H
#define WINDOWSS_H

#include "search.h"
#include "customcombobox.h"
#include "customtablewidget.h"
#include "extensiondialogadd.h"
#include "settings.h"
#include "tablekeypresseventlistner.h"
#include <QSettings>
#include <QDirIterator>
#include <QDateTime>
#include <QFontDatabase>
#include <QDebug>
#include <QLineEdit>
#include <QInputDialog>
#include <QMessageBox>
#include <QMap>
#include <QCloseEvent>
#include <cmath>
#include <QLabel>
#include <QPushButton>
#include <QMovie>
#include <QPixmap>
#include <QStackedLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QFile>
#include <QStack>
#include <QDesktopServices>
#include <QtFtp/QFtp>
#include <QTemporaryDir>
#include <QMessageBox>
#include <QList>
#include <QtFtp/QUrlInfo>
#include <QQueue>
#include <QItemSelectionModel>
#include <QHeaderView>

namespace Ui {
class WindowsS;
}

class WindowsS : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit WindowsS(QWidget *parent = 0);
    ~WindowsS();
    void createNewExtensionGroup(QString nomGroupe, QString valeurs);
    void getAllFilesFromDir(QString dir, QString groupExtensions);
    void affichageResultats(QVector<QString>);
    void affichageResultatsFtp(QMap<QString, QUrlInfo> &list);
    void initialisationSettings();
    void getActiveSearchPropertyWhenSearch();
    QString getSettingsType(QString searchKey);
    void drawTable();
    QString convertirTaille(int taille);
    void setApplicationStyle();
    void populateComboListExtensions();
    void setGifLoad();
    void displayGifOnTable();
    void removeGifOnTable();
    void enableCheckedFieldForSearch();
    void activeFieldForSearch(QWidget *layout,bool value);
    void signalsAndSlotsConnect();
    QString makeUrl(QString filename);
    void ftpConnect();
    bool isBetween(unsigned int minimumFileZize,unsigned int maximumFileSize, unsigned int value);
    bool isDateBetween(QDate const& minimumDate,QDate const& maximumDate,QDate const& value);
    unsigned int convertirTailleEnOctects(unsigned int tailleEnMo);
    bool matchExtensions(QString filename, QVector<QString> ext);
    bool matchParameters(QUrlInfo info,QMap<QString, QString> &activeSearchProperty);
    void reverseSearchAndStopSearchEnable();

public slots:
    void getRoot();
    void search();
    void addNewExtensionGroup();
    void alterExtensionGroup();
    void deleteExtensionGroup();
    void enableOkBtn_pressed();
    void enableNameFieldForSearch();
    void enableSizeFieldForSearch();
    void enableCreateDateFieldForSearch();
    void enableModifiedDateFieldForSearch();
    void enableLastDateFieldForSearch();
    void enableFtpFieldForSearch();
    void enableSearchAction();
    void settingsAccept();
    void searchThreadStart();
    void searchThreadStop();
    void getResultFromThread(QVector<QString> resultats);
    void showSettingsDialog();
    void on_filterLineEdit_textChanged(QString text);
    void on_rootFolder_textChanged(QString text);
    void rowsSelected(QModelIndex current, QModelIndex previous);
    void downloadFile(QString url, QString receptionDir, QString filename);
    void removeFtpFile(QString filePath);
    void renameFtpFile(QString oldName, QString newName);
    void moveFtpFile(QString filePath, QString receptionDir);
    void writeAFile();
    void writeAndCloseFile();
    void writeAndCloseFileForMove();
    void authentification(QNetworkReply*,QAuthenticator*);
    void downloadError(QNetworkReply::NetworkError code);
    void updateDownloadProgressBar(qint64 bytesReceived, qint64 bytesTotal);
    void handleCommandFinished( int id, bool error );
    void handleDone(bool error);
    void readFtpFile(QString filePath);
    void getAllFilesFromFtpDir(QString dir = "");
    void getFilesFromDir(QUrlInfo info);
    void getFtpFileForReading(QStack<QString> data);
    void getFtpFileForCopy(QStack<QString> data, QString receptionDir);
    void getFtpFileForCut(QStack<QString> data, QString receptionDir);
    void getFtpFileForRemove(QStack<QString> data);
    void getFtpFileForRename(int index);
    void setFtpActionsEnable(bool trigger);
    void stopSearch();
signals:
    void envoiDonnees(QString dir, QVector<QString>& ext,QMap<QString, QString>& activeSearchProperty,
                        QMap<QString, QVariant> values);

protected:
    void closeEvent(QCloseEvent *event);
    
private slots:
    void on_actionQuitter_triggered();

private:
    Ui::WindowsS *ui;
    QMap<QString, QVariant> extensions;
    QMap<QString, QVariant> parametresDeRecherche;
    QMap<QString, QVariant> valeurDeFiltres;
    QSettings *settings;
    QMap<QString, QString> activeSearchProperty;
    int nombreResultats;
    customTableWidget *tableResult;
    ExtensionDialogAdd* dialog;
    Search *searchThread;
    QStackedLayout *stackLayout;
    QLabel *gifLoad;
    QWidget *widgetHelpCenterGifAnimation;
    QHBoxLayout *boxLayout;
    QCheckBox *nameSetting;
    QCheckBox *sizeSetting;
    QHeaderView *headerV;
    QCheckBox *createdDateSetting;
    QCheckBox *ModifiedDateSetting;
    QCheckBox *LastAccessSetting;
    QCheckBox *systemFilterSetting;
    QCheckBox *hideFilterSetting;
    QCheckBox *ftpModeSetting;
    QDialogButtonBox *buttonBox;
    Settings *settingsDialog;
    QLineEdit *lineEdit;
    QLineEdit *lineEditNewExtensionGroup;
    QMessageBox *msgBox;
    QPushButton *dialogOkBtn;
    QMovie *movie;
    QNetworkAccessManager *networkManager;
    QNetworkReply *reply;
    QFile localFile;
    QStack<QString> pile;
    QString ftpUsername = "potos", ftpPassword = "lol", ftpHost = "127.0.0.1";
    int downloadCurrent = 0, downloadAverage = ((1/(float)3)*100), ftpPort = 21;
    QFtp *ftp;
    QTemporaryDir tempDir;
    bool readAfterDownload = false;
    QMetaObject::Connection ftpRemoveConnection;
    QString currentReceptionDir = "D:";
    QMap<QString,QUrlInfo> ftpFilesList;
    QMap<QString,QUrlInfo> resultVec;
    QStack<QString> iterateFolders;
    bool firstList;
    QQueue<QString> ftpFolders;
    QString currentFtpFolder;
    QString currentCommand;
    QString newFilename;
    int currentFileRenameIndex;
};

#endif // WINDOWSS_H
