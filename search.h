#ifndef SEARCH_H
#define SEARCH_H

#include <QThread>
#include <QDirIterator>
#include <QDebug>
#include <QDate>
#include <QDir>
#include <QFile>
#include <QMainWindow>
#include <QMetaType>
#include <QList>
#include <QtFtp/QUrlInfo>

Q_DECLARE_METATYPE(QVector<QString>)

class Search : public QThread
{
    Q_OBJECT

public:
    /*
        fonction principale de notre thread qui nous permet
        de lancer l'itération pour une recherche, ce qui a pour but d'éviter
        à l'application de se figer quand la recherche prend un peu trop de temps
     */
    void run();

    /*
        fonction qui renvoie un bouléen pour signifier que la taille d'une fichier
        est comprise entre les l'intervalle de taille fourni par l'utilisateur
    */
    bool isBetween(unsigned int minimumFileZize,unsigned int maximumFileSize, unsigned int value);

    /*

    */
    bool isDateBetween(QDate const& minimumDate,QDate const& maximumDate,QDate const& value);

    /*
        fonction qui renvoie la taille en octets d'un fichier
    */
    unsigned int convertirTailleEnOctects(unsigned int tailleEnMo);

    /*
        fonction qui se charge de vérifier si oui ou non, le type de fichier(son extension)
        correspond à ceux recherchés par l'utilisateur
    */
    bool matchExtensions(QString filename, QVector<QString> ext);

    /*
        fonction qui se charge de vérifier si oui ou non, les propriétés du fichier
        correspondent à ceux recherchés par l'utilisateur
    */
    bool matchParameters(QString filename, QFileInfo &info,QMap<QString, QString> &activeSearchProperty);

public slots:
    /*
        fonction chargé de recevoir toutes les données nécessaires pour le traitement d'une recherche
        au sein du thread
    */
    void recevoirDonnees(QString dir, QVector<QString>& ext, QMap<QString, QString> &activeSearchProperty,
                         QMap<QString, QVariant> values);
signals:
    /*
        signal chargé d'envoyer la liste du resultat pour l'affichage
    */
    void donneesResultatDeRecherche(QVector<QString>);

private:
    QVector<QDirIterator*> m_it;
    QVector<QString> m_ext;
    QMap<QString, QString> m_activeSearchProperty;
    QMap<QString, QVariant> valeurDeFiltres;
    QList<QUrlInfo> ftpFilesList;
    int done, all;
};

#endif // SEARCH_H
