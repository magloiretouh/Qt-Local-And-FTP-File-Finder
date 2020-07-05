#include "search.h"

void Search::run()
{
    done = 0;
    all= 1;
    qRegisterMetaType< QVector<QString> >();
    QString next;
    QFileInfo infos;
    QVector<QString> resultVec;
    for(QDirIterator* di: m_it)
    {
        while (di->hasNext()) {
            next = di->next();
            if(matchExtensions(next, m_ext) && matchParameters(next,infos,m_activeSearchProperty)){
                resultVec.push_back(next);
            }
         }
    }
    emit donneesResultatDeRecherche(resultVec);
}

void Search::recevoirDonnees(QString dir, QVector<QString> &ext, QMap<QString, QString>& activeSearchProperty,
                             QMap<QString, QVariant> values)
{
    m_ext = ext;
    m_activeSearchProperty = activeSearchProperty;
    valeurDeFiltres = values;
    m_it.clear();

    for(QString s: dir.split("; "))
    {
        if(valeurDeFiltres["hideFile"].toBool())
        {
            m_it.push_back(new QDirIterator(s,QDir::Files | QDir::Hidden , QDirIterator::Subdirectories));
        } else if(!valeurDeFiltres["hideFile"].toBool())
        {
            m_it.push_back(new QDirIterator(s,QDir::Files , QDirIterator::Subdirectories));
        }
    }
}

bool Search::matchExtensions(QString filename, QVector<QString> ext)
{
    return ext.contains(filename.split(".").last().toLower());
}

bool Search::matchParameters(QString filename, QFileInfo &info,QMap<QString, QString> &activeSearchProperty)
{
    info.setFile(filename);
    QString value;
    foreach (value, activeSearchProperty.keys())
    {
        if((activeSearchProperty[value]=="contains" || activeSearchProperty[value]=="interval") &&
                activeSearchProperty.keys().contains(value))
        {
            if(value.contains("Name", Qt::CaseInsensitive))
            {
                if(!info.fileName().contains(valeurDeFiltres["nameValue"].toString(), Qt::CaseInsensitive))
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
            else if(value.contains("interval_createAt", Qt::CaseInsensitive))
            {
                if(!isDateBetween(valeurDeFiltres["createdDateMinimum"].toDate(),valeurDeFiltres["createdDateMaximum"].toDate(),info.created().date()))
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

bool Search::isBetween(unsigned int minimumFileZize, unsigned int maximumFileSize, unsigned int value)
{
    return (value>=convertirTailleEnOctects(minimumFileZize) && value<=convertirTailleEnOctects(maximumFileSize));
}

bool Search::isDateBetween(const QDate &minimumDate, const QDate &maximumDate, const QDate &value)
{
    return (value>=minimumDate && value<=maximumDate);
}

unsigned int Search::convertirTailleEnOctects(unsigned int tailleEnMo)
{
    return tailleEnMo*1024*1024;
}
