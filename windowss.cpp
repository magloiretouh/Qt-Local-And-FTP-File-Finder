#include "windowss.h"
#include "ui_windowss.h"

WindowsS::WindowsS(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WindowsS)
{
    ui->setupUi(this);
    tableKeyPressEventListner *keyPressEvent = new tableKeyPressEventListner(this);
    settingsDialog = new Settings();
    msgBox = new QMessageBox();
    ui->filterLayout->setVisible(false);
    ui->actionRechercher->setEnabled(false);
    systemFilterSetting = settingsDialog->findChild<QCheckBox *>("settingsCheckBoxSystemFile");
    hideFilterSetting = settingsDialog->findChild<QCheckBox *>("settingsCheckBoxHideFile");
    nameSetting = settingsDialog->findChild<QCheckBox *>("settingsCheckBoxName");
    sizeSetting = settingsDialog->findChild<QCheckBox *>("settingsCheckBoxSize");
    createdDateSetting = settingsDialog->findChild<QCheckBox *>("settingsCheckBoxCreatedDate");
    ModifiedDateSetting = settingsDialog->findChild<QCheckBox *>("settingsCheckBoxModifiedDate");
    LastAccessSetting = settingsDialog->findChild<QCheckBox *>("settingsCheckBoxLastAccess");
    ftpModeSetting = settingsDialog->findChild<QCheckBox *>("settingsCheckBoxNetworkMode");
    buttonBox = settingsDialog->findChild<QDialogButtonBox *>("buttonBox");

    settings = new QSettings("MySoft","Advanded File Search");
    initialisationSettings();
    gifLoad = new QLabel();
    movie = new QMovie(":/icones/Preloader.gif");
    widgetHelpCenterGifAnimation = new QWidget();
    boxLayout = new QHBoxLayout();
    setGifLoad();
    dialog = new ExtensionDialogAdd();
    tableResult = new customTableWidget(this);
    tableResult->installEventFilter(keyPressEvent);
    QFont font("Roboto Cn",10);
    dialog->setFont(font);
    dialogOkBtn = dialog->findChild<QPushButton *>("dialogOkBtn");
    lineEdit = dialog->findChild<QLineEdit *>("lineEdit");
    lineEditNewExtensionGroup = dialog->findChild<QLineEdit *>("lineEditNewExtensionGroup");
    stackLayout = new QStackedLayout();
    stackLayout->setStackingMode(QStackedLayout::StackAll);
    stackLayout->addWidget(tableResult);
    ui->verticalLayout->addSpacing(20);
    ui->verticalLayout->addLayout(stackLayout);
    headerV = new QHeaderView(Qt::Horizontal);
    drawTable();
    setApplicationStyle();
    populateComboListExtensions();
    searchThread = new Search();
    networkManager = new QNetworkAccessManager(this);
    ftp = new QFtp();
    signalsAndSlotsConnect();
}

WindowsS::~WindowsS()
{
    delete ui;
}

void WindowsS::signalsAndSlotsConnect()
{
    connect(ui->actionRechercher,SIGNAL(triggered(bool)), this, SLOT(search()));
    connect(ui->actionAnnuler_recherche,SIGNAL(triggered(bool)), this, SLOT(stopSearch()));
    connect(ui->comboListType->getAddAction(),SIGNAL(triggered()), this, SLOT(addNewExtensionGroup()));
    connect(ui->actionAjouter_extensions,SIGNAL(triggered()), this, SLOT(addNewExtensionGroup()));
    connect(ui->comboListType->getAlterAction(),SIGNAL(triggered()), this, SLOT(alterExtensionGroup()));
    connect(ui->actionModifierExtensions,SIGNAL(triggered()), this, SLOT(alterExtensionGroup()));
    connect(ui->comboListType->getDeleteAction(),SIGNAL(triggered()), this, SLOT(deleteExtensionGroup()));
    connect(ui->actionSupprimer_extensions,SIGNAL(triggered()), this, SLOT(deleteExtensionGroup()));
    connect(dialogOkBtn,SIGNAL(clicked()), this, SLOT(enableOkBtn_pressed()));
    connect(nameSetting,SIGNAL(clicked()), this, SLOT(enableNameFieldForSearch()));
    connect(sizeSetting,SIGNAL(clicked()), this, SLOT(enableSizeFieldForSearch()));
    connect(createdDateSetting,SIGNAL(clicked()), this, SLOT(enableCreateDateFieldForSearch()));
    connect(ModifiedDateSetting,SIGNAL(clicked()), this, SLOT(enableModifiedDateFieldForSearch()));
    connect(LastAccessSetting,SIGNAL(clicked()), this, SLOT(enableLastDateFieldForSearch()));
    connect(ftpModeSetting,SIGNAL(clicked()), this, SLOT(enableFtpFieldForSearch()));
    connect(buttonBox,SIGNAL(accepted()),this,SLOT(settingsAccept()));
    connect(ui->actionParam_tres,SIGNAL(triggered(bool)), this, SLOT(showSettingsDialog()));
    connect(searchThread,SIGNAL(started()),this,SLOT(searchThreadStart()));
    connect(searchThread,SIGNAL(finished()),this,SLOT(searchThreadStop()));
    connect(searchThread,SIGNAL(donneesResultatDeRecherche(QVector<QString>)),this,SLOT(getResultFromThread(QVector<QString>)));
    connect(this,SIGNAL(envoiDonnees(QString,QVector<QString>&,QMap<QString,QString>&,QMap<QString,QVariant>)),searchThread,
            SLOT(recevoirDonnees(QString,QVector<QString>&,QMap<QString,QString>&,QMap<QString,QVariant>)));
    connect(networkManager,SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)), this, SLOT(authentification(QNetworkReply*,QAuthenticator*)));
    connect(ftp, SIGNAL(listInfo(QUrlInfo)), this, SLOT(getFilesFromDir(QUrlInfo)));
    connect(ftp, SIGNAL(done(bool)), this, SLOT(handleDone(bool)));
    connect(ftp, SIGNAL(commandFinished(int,bool)), this, SLOT(handleCommandFinished(int,bool)));
    connect(tableResult, SIGNAL(sendFtpFileForReading(QStack<QString>)), this, SLOT(getFtpFileForReading(QStack<QString>)));
    connect(tableResult, SIGNAL(sendFtpFileForCopy(QStack<QString>,QString)), this, SLOT(getFtpFileForCopy(QStack<QString>,QString)));
    connect(tableResult, SIGNAL(sendFtpFileForCut(QStack<QString>,QString)), this, SLOT(getFtpFileForCut(QStack<QString>,QString)));
    connect(tableResult, SIGNAL(sendFtpFileForRemove(QStack<QString>)), this, SLOT(getFtpFileForRemove(QStack<QString>)));
    connect(tableResult, SIGNAL(sendFtpFileForRename(int)), this, SLOT(getFtpFileForRename(int)));
    connect(tableResult->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(rowsSelected(QModelIndex,QModelIndex)));
    connect(ui->actionAjouter_repertoire_de_recherche, SIGNAL(triggered(bool)), this, SLOT(getRoot()));
    connect(ui->actionActiver_mode_FTP, SIGNAL(triggered(bool)), this, SLOT(setFtpActionsEnable(bool)));
    connect(ui->actionD_sactiver_mode_FTP, SIGNAL(triggered(bool)), this, SLOT(setFtpActionsEnable(bool)));
    connect(ui->actionLire, SIGNAL(triggered(bool)), tableResult, SLOT(lire()));
    connect(ui->actionCopier, SIGNAL(triggered(bool)), tableResult, SLOT(copy()));
    connect(ui->actionD_placer, SIGNAL(triggered(bool)), tableResult, SLOT(cut()));
    connect(ui->actionSupprimer, SIGNAL(triggered(bool)), tableResult, SLOT(supp()));
    connect(ui->actionRenommer, SIGNAL(triggered(bool)), tableResult, SLOT(renommer()));
    connect(ui->actionOuvrir_l_emplacement, SIGNAL(triggered(bool)), tableResult, SLOT(openFileFolder()));
    connect(ui->hostLineEdit, SIGNAL(textEdited(QString)), this, SLOT(enableSearchAction()));
    connect(ui->username, SIGNAL(textEdited(QString)), this, SLOT(enableSearchAction()));
    connect(ui->password, SIGNAL(textEdited(QString)), this, SLOT(enableSearchAction()));
    connect(ui->portEdit, SIGNAL(valueChanged(QString)), this, SLOT(enableSearchAction()));
}

void WindowsS::rowsSelected(QModelIndex current, QModelIndex previous)
{
    ui->actionRenommer->setEnabled((tableResult->selectionModel()->selectedRows(2).size() == 1));
    ui->actionLire->setEnabled(true);
    ui->actionCopier->setEnabled(true);
    ui->actionD_placer->setEnabled(true);
    ui->actionSupprimer->setEnabled(true);
    ui->actionOuvrir_l_emplacement->setEnabled(!valeurDeFiltres["ftp"].toBool());
    tableResult->setActionOpenParentDirectoryActive(valeurDeFiltres["ftp"].toBool());
    tableResult->setActionRenommer((tableResult->selectionModel()->selectedRows(2).size() == 1));
}

void WindowsS::alterExtensionGroup()
{
    lineEdit->setText(ui->comboListType->currentText());
    lineEditNewExtensionGroup->setText(extensions[ui->comboListType->currentText()].toString());
    dialog->show();
}

void WindowsS::deleteExtensionGroup()
{
    int ret = msgBox->critical(this,tr("Advanced File Search"),"Voulez-vous vraiment supprimer le \ngroupe d'extensions \""+ui->comboListType->currentText()+"\"",QMessageBox::Ok | QMessageBox::Cancel);
    if(ret == msgBox->Ok)
    {
        extensions.remove(ui->comboListType->currentText());
        populateComboListExtensions();
    }
}
void WindowsS::enableNameFieldForSearch()
{
    activeFieldForSearch(ui->nameLayout_2,nameSetting->isChecked());
}

void WindowsS::enableSizeFieldForSearch()
{
    activeFieldForSearch(ui->sizeLayout,sizeSetting->isChecked());
}

void WindowsS::enableCreateDateFieldForSearch()
{
    activeFieldForSearch(ui->createdDateLayout,createdDateSetting->isChecked());
}

void WindowsS::enableModifiedDateFieldForSearch()
{
    activeFieldForSearch(ui->modifiedDateLayout,ModifiedDateSetting->isChecked());
}

void WindowsS::enableLastDateFieldForSearch()
{
    activeFieldForSearch(ui->lastAccessLayout_2,LastAccessSetting->isChecked());
}

void WindowsS::enableFtpFieldForSearch()
{
    activeFieldForSearch(ui->ftpLayout,ftpModeSetting->isChecked());
    ui->actionAjouter_repertoire_de_recherche->setEnabled(!ftpModeSetting->isChecked());
    ui->actionSupprimer_repertoire->setEnabled(!ftpModeSetting->isChecked());
    (ftpModeSetting->isChecked()) ? ui->rootFolder->setText(QDir::separator()) : ui->rootFolder->setText("");
}

void WindowsS::enableCheckedFieldForSearch()
{
    activeFieldForSearch(ui->lastAccessLayout_2,parametresDeRecherche["interval_readAt"].toBool());
    activeFieldForSearch(ui->modifiedDateLayout,parametresDeRecherche["interval_modifyAt"].toBool());
    activeFieldForSearch(ui->ftpLayout,parametresDeRecherche["bool_ftp"].toBool());
    activeFieldForSearch(ui->createdDateLayout,parametresDeRecherche["interval_createAt"].toBool());
    activeFieldForSearch(ui->sizeLayout,parametresDeRecherche["interval_sizeFile"].toBool());
    activeFieldForSearch(ui->nameLayout_2,parametresDeRecherche["contains_basename"].toBool());
}

void WindowsS::activeFieldForSearch(QWidget *layout, bool value)
{
    layout->setVisible(value);
}

void WindowsS::settingsAccept()
{
    parametresDeRecherche["bool_hideFile"] = hideFilterSetting->isChecked();
    parametresDeRecherche["bool_systemFile"] = systemFilterSetting->isChecked();
    parametresDeRecherche["bool_ftp"] = ftpModeSetting->isChecked();

    parametresDeRecherche["contains_basename"] = nameSetting->isChecked();
    parametresDeRecherche["interval_modifyAt"] = ModifiedDateSetting->isChecked();
    parametresDeRecherche["interval_readAt"] = LastAccessSetting->isChecked();
    parametresDeRecherche["interval_createAt"] = createdDateSetting->isChecked();
    parametresDeRecherche["interval_sizeFile"] = sizeSetting->isChecked();
}

void WindowsS::addNewExtensionGroup()
{
    dialog->show();
}


void WindowsS::search()
{
    displayGifOnTable();
    getAllFilesFromDir(ui->rootFolder->text(),ui->comboListType->currentText());
}

void WindowsS::getRoot()
{
    if(!ftpModeSetting->isChecked())
    {
        QString value = QFileDialog::getExistingDirectory(this,"Selectionner un dossier à ajouter",ui->rootFolder->text().split("; ").last());
        if( value != "")
        {
            if(!ui->rootFolder->text().split("; ").contains(value, Qt::CaseInsensitive))
            {
                if(ui->rootFolder->text().isEmpty())
                {
                    ui->rootFolder->setText(value);
                }
                else
                {
                    ui->rootFolder->setText(ui->rootFolder->text()+"; "+value);
                }
            }
        }
    }
}

void WindowsS::createNewExtensionGroup(QString nomGroupe, QString valeurs)
{
    extensions[nomGroupe.toLower()] = valeurs.toLower();
}

void WindowsS::populateComboListExtensions()
{
    ui->comboListType->clear();
    ui->comboListType->addItems(extensions.keys());
}

void WindowsS::removeGifOnTable()
{
    stackLayout->removeWidget(widgetHelpCenterGifAnimation);
    stackLayout->setCurrentIndex(0);
}

void WindowsS::searchThreadStart()
{
    reverseSearchAndStopSearchEnable();
}

void WindowsS::searchThreadStop()
{
    removeGifOnTable();
    reverseSearchAndStopSearchEnable();
    ui->actionRenommer->setEnabled(false);
    ui->actionLire->setEnabled(false);
    ui->actionCopier->setEnabled(false);
    ui->actionD_placer->setEnabled(false);
    ui->actionSupprimer->setEnabled(false);
    ui->actionOuvrir_l_emplacement->setEnabled(false);
}

void WindowsS::getAllFilesFromDir(QString dir, QString groupExtensions)
{
    activeSearchProperty.clear(); //pour pouvoir ajouter les nouvelles proprietes de fichiers sur lesquels on veut faire les recherches
    nombreResultats=0;
    //remplir la variable "activeSearchProperty" pour savoir quel genre de filtre faire
    getActiveSearchPropertyWhenSearch();

    //vecteur pour pouvoir avoir la liste des extensions concernees
    QVector<QString> vec;
    QStringList list = extensions[groupExtensions].toString().split(",");

    //remplir le vecteur d'extensions
    for (int var = 0; var < list.size(); ++var) {
        vec.push_back(list.value(var));
    }

    //manipulation du thread pour la recherche afin d'éviter le plantage de l'application
    searchThread->moveToThread(this->thread());

    valeurDeFiltres["hideFile"] = parametresDeRecherche["bool_hideFile"];
    valeurDeFiltres["systemFile"] = parametresDeRecherche["bool_systemFile"];
    valeurDeFiltres["ftp"] = parametresDeRecherche["bool_ftp"];
    valeurDeFiltres["nameValue"] = ui->nameField->text();
    valeurDeFiltres["minimumSize"] = ui->minimumSize->text();
    valeurDeFiltres["maximumSize"] = ui->maximumSize->text();
    valeurDeFiltres["createdDateMinimum"] = ui->minimumCreatedDate->date();
    valeurDeFiltres["createdDateMaximum"] = ui->maximumCreatedDate->date();
    valeurDeFiltres["modifiedDateMinimum"] = ui->minimumModifiedDate->date();
    valeurDeFiltres["modifiedDateMaximum"] = ui->maximumModifiedDate->date();
    valeurDeFiltres["lastAccessDateMinimum"] = ui->minimumLastAccessDate->date();
    valeurDeFiltres["lastAccessDateMaximum"] = ui->maximumLastAccessDate_2->date();

    if(valeurDeFiltres["ftp"].toBool()){
        if(ftp->state() == QFtp::Connected || ftp->state() == QFtp::LoggedIn)
        {
            ftp->close();
        }
        ftpConnect();
        getAllFilesFromFtpDir(QDir::separator());
    } else{
        emit envoiDonnees(dir,vec,activeSearchProperty,valeurDeFiltres);
        searchThread->start();
    }
}

void WindowsS::displayGifOnTable()
{
    stackLayout->addWidget(widgetHelpCenterGifAnimation);
    stackLayout->setCurrentIndex(1);
}

void WindowsS::setGifLoad()
{
    gifLoad->setWindowFlags(Qt::FramelessWindowHint);
    gifLoad->setMovie(movie);
    movie->start();

    boxLayout->addStretch();
    boxLayout->addWidget(gifLoad);
    boxLayout->addStretch();
    widgetHelpCenterGifAnimation->setLayout(boxLayout);
}

void WindowsS::getResultFromThread(QVector<QString> resultats)
{
    nombreResultats = resultats.count();
    affichageResultats(resultats);
}

void WindowsS::affichageResultats(QVector<QString> resultats)
{
    tableResult->setRowCount(0);
    tableResult->setRowCount(nombreResultats);
    QFileInfo infos;
    for (int var = 0; var < resultats.size(); ++var) {
        infos.setFile(resultats.value(var));
        tableResult->setItem(var,1,new QTableWidgetItem(infos.fileName()));
        tableResult->setItem(var,2,new QTableWidgetItem(infos.lastModified().toString()));
        tableResult->setItem(var,3,new QTableWidgetItem(convertirTaille(infos.size())));
        tableResult->setItem(var,4,new QTableWidgetItem(infos.created().toString()));
        tableResult->setItem(var,5,new QTableWidgetItem(infos.filePath()));
    }

    headerV->setSectionResizeMode(QHeaderView::Interactive);
    QMessageBox::information(this,"Advanced File Search",QString::number(nombreResultats)+" éléments(s) trouvé(s).");
    ui->filterLayout->setVisible(resultats.size()!=0);
}

void WindowsS::showSettingsDialog()
{
    settingsDialog->show();
}

void WindowsS::drawTable()
{
    headerV->setSectionResizeMode(QHeaderView::Stretch);
    headerV->setHighlightSections(true);
    headerV->setSectionsMovable(true);
    headerV->setSectionsClickable(true);
    QStringList header;
    header << "#" <<  "Nom" << "Modifié le" << "Taille" << "Date de création" << "Chemin d'accès";
    tableResult->setColumnCount(6);
    tableResult->setHorizontalHeader(headerV);
    tableResult->setHorizontalHeaderLabels(header);
    tableResult->setAlternatingRowColors(true);
    tableResult->setColumnHidden(0,true);
    tableResult->setSortingEnabled(true);
    tableResult->setDragEnabled(true);
    tableResult->setShowGrid(false);
    tableResult->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableResult->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableResult->setDragDropMode(QAbstractItemView::DragOnly);
    tableResult->verticalHeader()->setVisible(false);
}

QString WindowsS::convertirTaille(int taille)
{
    if(taille < 1024){
        return QString::number(taille)+" Octects";
    }
    else if(taille >= 1024 && taille < 1024*1024){
        return QString::number((int)(taille/1024))+" Ko";
    } else if(taille >= 1024*1024 && taille < (double)1024*(double)1024*(double)1024){
        return QString::number((int)((double)taille/(1024*1024)))+" Mo";
    } else if(taille >= 1024*1024*1024 && taille < (double)1024*(double)1024*(double)1024*(double)1024){
        return QString::number(taille/((double)1024*(double)1024*(double)1024),'g', 3)+" Go";
    }
}

void WindowsS::getActiveSearchPropertyWhenSearch()
{
    QString searchKey;
    foreach (searchKey, parametresDeRecherche.keys()) {
        if(parametresDeRecherche[searchKey] == true){
            activeSearchProperty.insert(searchKey,getSettingsType(searchKey));
        }
    }
}

QString WindowsS::getSettingsType(QString searchKey)
{
    return searchKey.split("_").at(0);
}

void WindowsS::initialisationSettings()
{
    parametresDeRecherche["bool_hideFile"] = false;
    parametresDeRecherche["bool_systemFile"] = false;
    parametresDeRecherche["bool_ftp"] = false;

    parametresDeRecherche["contains_basename"] = false;
    parametresDeRecherche["interval_modifyAt"] = false;
    parametresDeRecherche["interval_readAt"] = false;
    parametresDeRecherche["interval_createAt"] = false;
    parametresDeRecherche["interval_sizeFile"] = false;

    valeurDeFiltres[""] = ui->nameField->text();

    //creations d'une liste de groupe d'extensions pour le premier lancement
    QMap<QString, QVariant> extensionsFirstUse;
    extensionsFirstUse["videos"] = "mp4,avi,mpeg";
    extensionsFirstUse["audios"] = "mp3";
    extensionsFirstUse["images"] = "jpg,png,gif,svg";

    extensions = settings->value("extensions", extensionsFirstUse).toMap();
    parametresDeRecherche = settings->value("parametresDeFiltre").toMap();

    systemFilterSetting->setChecked(parametresDeRecherche["bool_systemFile"].toBool());
    hideFilterSetting->setChecked(parametresDeRecherche["bool_hideFile"].toBool());
    ftpModeSetting->setChecked(parametresDeRecherche["bool_ftp"].toBool());
    nameSetting->setChecked(parametresDeRecherche["contains_basename"].toBool());
    sizeSetting->setChecked(parametresDeRecherche["interval_sizeFile"].toBool());
    createdDateSetting->setChecked(parametresDeRecherche["interval_createAt"].toBool());
    ModifiedDateSetting->setChecked(parametresDeRecherche["interval_modifyAt"].toBool());
    LastAccessSetting->setChecked(parametresDeRecherche["interval_readAt"].toBool());

    ui->actionRenommer->setEnabled(false);
    ui->actionLire->setEnabled(false);
    ui->actionCopier->setEnabled(false);
    ui->actionD_placer->setEnabled(false);
    ui->actionSupprimer->setEnabled(false);
    ui->actionAjouter_repertoire_de_recherche->setEnabled(!ftpModeSetting->isChecked());
    ui->actionSupprimer_repertoire->setEnabled(!ftpModeSetting->isChecked());
    ui->actionOuvrir_l_emplacement->setEnabled(false);
    ui->actionActiver_mode_FTP->setEnabled(!ftpModeSetting->isChecked());
    ui->actionD_sactiver_mode_FTP->setEnabled(ftpModeSetting->isChecked());
    ui->actionRechercher->setEnabled(false);
    ui->actionAnnuler_recherche->setEnabled(false);
    (ftpModeSetting->isChecked()) ? ui->rootFolder->setText(QDir::separator()) : ui->rootFolder->setText("");

    enableCheckedFieldForSearch();
}

void WindowsS::setApplicationStyle()
{
    QApplication::setStyle("plastique");
    QFontDatabase database;
    database.addApplicationFont(":/fonts/Roboto-Medium.ttf");
    database.addApplicationFont(":/fonts/Roboto-Condensed.ttf");
    QFont font("Roboto Lt",10);
    QApplication::setFont(font);
}

void WindowsS::enableOkBtn_pressed()
{
    if(extensions.keys().contains(lineEdit->text()))
    {
        if(lineEditNewExtensionGroup->text() != extensions[lineEdit->text()])
        {
            msgBox->setText(tr("Ce groupe d'extensions existe déja."));
            msgBox->setInformativeText(tr("Voulez vous le modifier?"));
            msgBox->setStandardButtons(QMessageBox::Ok | QMessageBox::Discard | QMessageBox::Cancel);
            msgBox->setDefaultButton(QMessageBox::Ok);
            int ret = msgBox->exec();

            if(ret == QMessageBox::Ok)
            {
                createNewExtensionGroup(lineEdit->text(),lineEditNewExtensionGroup->text());
                msgBox->setText(tr("Groupe d'extensions modifié avec succès."));
                msgBox->setStandardButtons(QMessageBox::Ok);
                msgBox->setInformativeText("");
                msgBox->exec();
            }
        }
        dialog->close();
    }
    else
    {
        createNewExtensionGroup(lineEdit->text(),lineEditNewExtensionGroup->text());
        msgBox->setText(tr("Groupe d'extensions enrégistré avec succès."));
        msgBox->exec();
        dialog->close();
        populateComboListExtensions();
    }
}

void WindowsS::closeEvent(QCloseEvent *event)
{
    settings->setValue("extensions", extensions);
    settings->setValue("parametresDeFiltre", parametresDeRecherche);
}


void WindowsS::on_filterLineEdit_textChanged(QString text)
{

    for (int var = 0; var < nombreResultats; ++var)
    {
        tableResult->setRowHidden(tableResult->item(var,5)->row(),(tableResult->item(var,5) != 0 && !tableResult->item(var,5)->text().contains(text, Qt::CaseInsensitive)));
    }
}

void WindowsS::on_rootFolder_textChanged(QString text)
{
    if(!ftpModeSetting->isChecked())
    {
        ui->actionRechercher->setEnabled(!text.isEmpty());
    }
}

void WindowsS::ftpConnect()
{
    ftpHost = ui->hostLineEdit->text();
    ftpPort = ui->portEdit->value();
    ftpUsername = ui->username->text();
    ftpPassword = ui->password->text();
    ftp->connectToHost(ftpHost, ftpPort);
    ftp->login(ftpUsername, ftpPassword);
}

void WindowsS::downloadFile(QString url, QString receptionDir, QString filename)
{
    currentReceptionDir = receptionDir;
    localFile.setFileName(receptionDir+QDir::separator()+filename.split(QDir::separator()).last());
    reply = networkManager->get(QNetworkRequest(QUrl(url)));
    connect(reply, SIGNAL(readyRead()), this, SLOT(writeAFile()));
    connect(reply, SIGNAL(finished()), this, SLOT(writeAndCloseFile()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(downloadError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(updateDownloadProgressBar(qint64,qint64)));
}

void WindowsS::writeAFile()
{
    if(!localFile.isOpen()){
        if (!localFile.open(QIODevice::WriteOnly)){
            return;
        }
    }
    localFile.write(reply->readAll());
}

void WindowsS::writeAndCloseFile()
{
    writeAFile();
    localFile.close();
    if(readAfterDownload){
        QDesktopServices::openUrl(QUrl::fromLocalFile(tempDir.path()+QDir::separator()+pile.first().split(QDir::separator()).last()));
    }
    pile.removeFirst();
    if(!pile.isEmpty()){
        downloadCurrent++;
        downloadFile(makeUrl(pile.first()),currentReceptionDir,pile.first());
    } else if(pile.isEmpty()&&readAfterDownload){
        readAfterDownload=false;
    }
}

void WindowsS::writeAndCloseFileForMove()
{
    writeAFile();
    localFile.close();
    removeFtpFile(pile.first().section(QDir::separator(),3,pile.first().split(QDir::separator()).size()-1));
    pile.removeFirst();
    if(!pile.isEmpty()){
        moveFtpFile(pile.first(),currentReceptionDir);
    }
}

void WindowsS::authentification(QNetworkReply *, QAuthenticator *)
{
    qDebug() << "authentification";
}

QString WindowsS::makeUrl(QString filename)
{
    return "ftp:"+QString(QDir::separator())+QString(QDir::separator())+ftpUsername+":"+ftpPassword+"@"+ftpHost+":"+ftpPort+QDir::separator()+filename.section(QDir::separator(),3,filename.split(QDir::separator()).size()-1);
}

void WindowsS::downloadError(QNetworkReply::NetworkError code)
{
    qDebug() << "error while dowload";
}

void WindowsS::updateDownloadProgressBar(qint64 bytesReceived, qint64 bytesTotal)
{
    //pour le downloadAverage, faire le calcul lorsqu'on met les élements dans la pile et remettre de downloadCurrent à 1 à chaque fois
    qDebug() <<  (downloadAverage*((bytesReceived/(float)bytesTotal)*100))/100+(downloadCurrent*downloadAverage);
}

void WindowsS::handleCommandFinished(int id, bool error)
{
    if(ftp->currentCommand() == QFtp::Remove) {
        currentCommand = "Remove";
    } else if(ftp->currentCommand() == QFtp::List){
        qDebug() << "c'est un list";
        qDebug() << ftpFolders.size();
        if(!ftpFolders.isEmpty()){
            currentFtpFolder = ftpFolders.dequeue();
            qDebug() << "retour en arrierer";
            qDebug() << QDir::separator();
            ftp->cd(QDir::separator());
            qDebug() << "devant";
            qDebug() << currentFtpFolder;
            ftp->cd(currentFtpFolder);
            ftp->list();
        }
        currentCommand = "List";
    } else if(ftp->currentCommand() == QFtp::ConnectToHost)
    {
        currentCommand = "ConnectToHost";
        qDebug() << "all iterate with success ";
    }else if(ftp->currentCommand() == QFtp::Login)
    {
        currentCommand = "Login";
    }else if(ftp->currentCommand() == QFtp::Cd)
    {
        currentCommand = "Cd";
    }else if(ftp->currentCommand() == QFtp::Rename)
    {
        currentCommand = "Rename";
    }

   if((currentCommand == "ConnectToHost" || currentCommand == "Login" || currentCommand == "List") && error)
    {
        removeGifOnTable();
        reverseSearchAndStopSearchEnable();
    }
}

void WindowsS::removeFtpFile(QString filePath)
{
    ftp->remove(filePath);
}

void WindowsS::moveFtpFile(QString filePath, QString receptionDir)
{
    currentReceptionDir = receptionDir;
    localFile.setFileName(receptionDir+QDir::separator()+filePath.split(QDir::separator()).last());
    reply = networkManager->get(QNetworkRequest(QUrl(makeUrl(filePath))));
    connect(reply, SIGNAL(readyRead()), this, SLOT(writeAFile()));
    connect(reply, SIGNAL(finished()), this, SLOT(writeAndCloseFileForMove()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(downloadError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(updateDownloadProgressBar(qint64,qint64)));
}

void WindowsS::readFtpFile(QString filePath)
{
    readAfterDownload = true;
    downloadFile(makeUrl(filePath),tempDir.path(),filePath);
}

void WindowsS::getFilesFromDir(QUrlInfo info)
{
    if(info.isFile()){
        ftpFilesList[currentFtpFolder+QDir::separator()+info.name()] = info;
    } else if(info.isDir()){
        qDebug() << "anvant dy ajouter quoi que se soit";
        qDebug() << currentFtpFolder;
        ftpFolders.enqueue(currentFtpFolder+QDir::separator()+info.name());
    }
}

void WindowsS::getAllFilesFromFtpDir(QString dir)
{
    reverseSearchAndStopSearchEnable();
    ftpFolders.clear();
    ftpFilesList.clear();
    currentFtpFolder = "";
    ftp->cd(dir);
    ftp->list();
}

void WindowsS::handleDone(bool error)
{
    if(error)
    {
        qDebug() << "erreur lors des signaux "+QString::number(ftpFilesList.count());
        if(currentCommand == "Rename")
        {
            QMessageBox::critical(this,"Advanced File Search","Erreur lors de la modification",QMessageBox::Ok);
        }
        else if(currentCommand == "List")
        {
            QMessageBox::critical(this,"Advanced File Search","Erreur lors de la recherche",QMessageBox::Ok);
            removeGifOnTable();
            reverseSearchAndStopSearchEnable();
        }
        else if(currentCommand == "ConnectToHost")
        {
            QMessageBox::critical(this,"Advanced File Search","Erreur lors de la connexion au serveur FTP \n Vérifiez l'adresse du serveur.",QMessageBox::Ok);
        }
        else if(currentCommand == "Login")
        {
            QMessageBox::critical(this,"Advanced File Search","Erreur lors de la connexion <b>Identifiant ou Mot de passe incorrect</b>",QMessageBox::Ok);
        }
        else
        {
            qDebug() << currentCommand;
        }
    }
    else
    {
        qDebug() << "all iterate with success "+QString::number(ftpFilesList.count());
        if(currentCommand == "List")
        {
            qDebug() << "après avoir recupere tous avec les commandes list, nous avons "<<nombreResultats;
            QVector<QString> vec;
            resultVec.clear();
            QStringList list = extensions[ui->comboListType->currentText()].toString().split(",");

            //remplir le vecteur d'extensions
            for (int var = 0; var < list.size(); ++var) {
                vec.push_back(list.value(var));
            }
            QMapIterator<QString, QUrlInfo> i(ftpFilesList);

            while (i.hasNext()) {
                i.next();
                if(matchExtensions(i.value().name(), vec) && matchParameters(i.value(),activeSearchProperty)){
                  resultVec[i.key()] = i.value();
              }
            }
            removeGifOnTable();
            qDebug() << ui->actionRechercher->isEnabled();
            reverseSearchAndStopSearchEnable();
            qDebug() << "bon la je ne compren plus rien";
            qDebug() << ui->actionRechercher->isEnabled();
            ui->actionRenommer->setEnabled(false);
            ui->actionRenommer->setEnabled(false);
            ui->actionLire->setEnabled(false);
            ui->actionCopier->setEnabled(false);
            ui->actionD_placer->setEnabled(false);
            ui->actionSupprimer->setEnabled(false);
            ui->actionOuvrir_l_emplacement->setEnabled(false);
            nombreResultats = resultVec.size();
            affichageResultatsFtp(resultVec);
        } else if(currentCommand == "Rename")
        {
            QMessageBox::information(this,"Advanced File Search","Modification éffectuée",QMessageBox::Ok);
            tableResult->setItem(currentFileRenameIndex,1,new QTableWidgetItem(newFilename.split(QDir::separator()).last()));
            qDebug() << newFilename;
            tableResult->setItem(currentFileRenameIndex,5,new QTableWidgetItem("ftp:"+QString(QDir::separator())+newFilename));
        }
    }
}

void WindowsS::affichageResultatsFtp(QMap<QString,QUrlInfo> &list)
{
    tableResult->setRowCount(0);
    tableResult->setRowCount(nombreResultats);
    QMapIterator<QString, QUrlInfo> i(list);
    int var = 0;
    while (i.hasNext()) {
        i.next();
        tableResult->setItem(var,1,new QTableWidgetItem(i.value().name()));
        tableResult->setItem(var,2,new QTableWidgetItem(i.value().lastModified().toString()));
        tableResult->setItem(var,3,new QTableWidgetItem(convertirTaille(i.value().size())));
        tableResult->setItem(var,4,new QTableWidgetItem(i.value().lastRead().toString()));//date de creation
        tableResult->setItem(var,5,new QTableWidgetItem("ftp:"+QString(QDir::separator())+QString(QDir::separator())+ftpHost+i.key()));//chemin dacces
        var++;
    }
    headerV->setSectionResizeMode(QHeaderView::Interactive);
    QMessageBox::information(this,"Advanced File Search",QString::number(list.size())+" éléments(s) trouvé(s).");
    ui->filterLayout->setVisible(list.size()!=0);
}

bool WindowsS::matchExtensions(QString filename, QVector<QString> ext)
{
    return ext.contains(filename.split(".").last().toLower());
}

bool WindowsS::matchParameters(QUrlInfo info,QMap<QString, QString> &activeSearchProperty)
{
    QString value;
    foreach (value, activeSearchProperty.keys())
    {
        if((activeSearchProperty[value]=="contains" || activeSearchProperty[value]=="interval") &&
                activeSearchProperty.keys().contains(value))
        {
            if(value.contains("Name", Qt::CaseInsensitive))
            {
                if(!info.name().contains(valeurDeFiltres["nameValue"].toString(), Qt::CaseInsensitive))
                {
                    return false;
                }
            }
            else if(value.contains("sizeFile", Qt::CaseInsensitive))
            {
                if(!isBetween(valeurDeFiltres["minimumSize"].toInt(),valeurDeFiltres["maximumSize"].toInt(),info.size()))
                {
                    return false;
                }
            }
            else if(value.contains("interval_modifyAt", Qt::CaseInsensitive))
            {
                if(!isDateBetween(valeurDeFiltres["modifiedDateMinimum"].toDate(),valeurDeFiltres["modifiedDateMaximum"].toDate(),info.lastModified().date()))
                {
                    return false;
                }
            }
            else if(value.contains("interval_readAt", Qt::CaseInsensitive))
            {
                if(!isDateBetween(valeurDeFiltres["lastAccessDateMinimum"].toDate(),valeurDeFiltres["lastAccessDateMaximum"].toDate(),info.lastRead().date()))
                {
                    return false;
                }
            }
        }
    }
    return true;
}

bool WindowsS::isBetween(unsigned int minimumFileZize, unsigned int maximumFileSize, unsigned int value)
{
    return (value>=convertirTailleEnOctects(minimumFileZize) && value<=convertirTailleEnOctects(maximumFileSize));
}

bool WindowsS::isDateBetween(const QDate &minimumDate, const QDate &maximumDate, const QDate &value)
{
    return (value>=minimumDate && value<=maximumDate);
}

unsigned int WindowsS::convertirTailleEnOctects(unsigned int tailleEnMo)
{
    return tailleEnMo*1024*1024;
}

void WindowsS::getFtpFileForReading(QStack<QString> data)
{
    pile = data;
    readFtpFile(pile.first());
}
void WindowsS::getFtpFileForCopy(QStack<QString> data, QString receptionDir)
{
    pile = data;
    downloadFile(makeUrl(pile.first()),receptionDir,pile.first());
}

void WindowsS::getFtpFileForCut(QStack<QString> data, QString receptionDir)
{
    pile = data;
    moveFtpFile(pile.first(),receptionDir);
}

void WindowsS::getFtpFileForRemove(QStack<QString> data)
{
    pile = data;
    while(!pile.isEmpty())
    {
        removeFtpFile(pile.first().section(QDir::separator(),3,pile.first().split(QDir::separator()).size()-1));
        pile.removeFirst();
    }
}

void WindowsS::getFtpFileForRename(int index)
{
    QString data = tableResult->item(index,5)->text();
    QStringList pathTemp = data.split(QDir::separator());
    bool ok;
    QStringList filenameTemp = pathTemp.last().split(".");
    filenameTemp.removeLast();
    QString trueFileName = filenameTemp.join(".");
    QString text = QInputDialog::getText(this, tr("Advanced File Search : Renommer"),
                                               tr("Nouveau nom"), QLineEdit::Normal,
                                               trueFileName, &ok);
    if(ok && !text.isEmpty() && text!=trueFileName)
    {
        newFilename = data.section(QDir::separator(),3,data.split(QDir::separator()).size()-2)+QDir::separator()+text+"."+data.split(".").last();
        currentFileRenameIndex = index;
        renameFtpFile(data.section(QDir::separator(),3,data.split(QDir::separator()).size()-1), newFilename);
    }
}

void WindowsS::renameFtpFile(QString oldName, QString newName)
{
    ftp->rename(oldName, newName);
}

void WindowsS::setFtpActionsEnable(bool trigger)
{
    ftpModeSetting->setChecked(!ftpModeSetting->isChecked());
    if(ftpModeSetting->isChecked())
    {
        ui->actionActiver_mode_FTP->setEnabled(false);
        ui->actionD_sactiver_mode_FTP->setEnabled(true);
    }
    else
    {
        ui->actionActiver_mode_FTP->setEnabled(true);
        ui->actionD_sactiver_mode_FTP->setEnabled(false);
    }
    ui->actionRechercher->setEnabled(false);
    ui->actionAnnuler_recherche->setEnabled(false);
    enableFtpFieldForSearch();
    settingsAccept();

}
void WindowsS::reverseSearchAndStopSearchEnable()
{
    ui->actionRechercher->setEnabled(!ui->actionRechercher->isEnabled());
    ui->actionAnnuler_recherche->setEnabled(!ui->actionAnnuler_recherche->isEnabled());
}

void WindowsS::stopSearch()
{
    searchThread->terminate();
}

void WindowsS::enableSearchAction()
{
    ui->actionRechercher->setEnabled((!ui->hostLineEdit->text().isEmpty() && !ui->username->text().isEmpty() && !ui->password->text().isEmpty() && !ui->portEdit->text().isEmpty()));
}

void WindowsS::on_actionQuitter_triggered()
{
    this->close();
}
