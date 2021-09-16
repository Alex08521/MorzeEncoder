#include "encoder.h"

Encoder::Encoder(QObject *parent) : QObject(parent)
{
    questionString = "Enter text or morze code, and you may load code from file";
    answerString = "You answer will be here";
    russian = false;

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./morze.sqlite");
    if(db.open()) qDebug() << "Data base is open";
    else qDebug() << "Cannot open database";

    qDebug() << "Construstor done";
}

void Encoder::saveToTXT(QString savepath)
{
    savepath.remove("file:///");
    QTextCodec::setCodecForLocale(QTextCodec::codecForName( "UTF-8" ));
    QFile toSave(savepath);
        if(toSave.open(QIODevice::WriteOnly))
        {
            QTextStream stream(&toSave);
            stream << answerString;
            toSave.close();
        }

    qDebug() << savepath;
}

void Encoder::loadFromTXT(QString loadpath)
{    
    loadpath.remove("file:///");
    QTextCodec::setCodecForLocale(QTextCodec::codecForName( "UTF-8" ));
    QFile onLoad(loadpath);
    if(onLoad.open(QIODevice::ReadOnly))
    {
        questionString = onLoad.readAll();
        qDebug() << onLoad.readAll();
        onLoad.close();
    }

    qDebug() << loadpath;
}

void Encoder::encode()
{
    QSqlQuery q(db);

    int g = 0;
    int stringSize = questionString.size();
    bool english = true;
    answerString = "";
    int symbolnumber = 0;
    QChar lastsymbol = '-';

    QString textval;
    QString codeval;
    qDebug() << stringSize;


    if((questionString[0] == '.' && (questionString[1] == "-" || questionString[1] == " " || questionString[1] == ".")) ||
           (questionString[0] == '-' && (questionString[1] == "." || questionString[1] == " " || questionString[1] == "-")) ||
            (stringSize == 1 && (questionString[0] == "." || questionString[0] == "-")))
    {
        for (int i = 0; i < stringSize; i++)
        {
            symbolnumber++;
            if (textval == '\n') answerString += '\n';
            else if(questionString[i] != " ")
            {
                do
                {
                    codeval += questionString[i];
                    i++;
                    if(i >= stringSize) break;
                }while(questionString[i] != " ");

                qDebug() << "Cycle DO comkplete";

                if(q.prepare("SELECT id FROM morzeEncode WHERE code =:codeval"))
                {
                    q.bindValue(":codeval", codeval);
                    if(!q.exec()) qDebug() << q.lastError();
                }
                else qDebug() << "Query failed";
                if(!q.first()) qDebug() << q.lastError();
                g = q.value(0).toInt();

                if(russian)
                {
                    if(symbolnumber < 2 || lastsymbol == '?' || lastsymbol == '!' || lastsymbol == '.' || lastsymbol == '\n')
                    {
                        if(q.prepare("SELECT kyrillic FROM morzeEncode WHERE id =:g"))
                        {
                            q.bindValue(":g", g);
                            if(!q.exec()) qDebug() << q.lastError();
                        }
                        else qDebug() << "Query failed";
                    }

                    else
                    {
                        if(q.prepare("SELECT kyrillicSmall FROM morzeEncode WHERE id =:g"))
                        {
                            q.bindValue(":g", g);
                            if(!q.exec()) qDebug() << q.lastError();
                        }
                        else qDebug() << "Query failed";
                    }
                }
                else
                {
                    if(symbolnumber < 2 || lastsymbol == '?' || lastsymbol == '!' || lastsymbol == '.' || lastsymbol == '\n')
                    {
                        if(q.prepare("SELECT latinic FROM morzeEncode WHERE id =:g"))
                        {
                            q.bindValue(":g", g);
                            if(!q.exec()) qDebug() << q.lastError();
                        }
                        else qDebug() << "Query failed";
                    }

                    else
                    {
                        if(q.prepare("SELECT latinicSmall FROM morzeEncode WHERE id =:g"))
                        {
                            q.bindValue(":g", g);
                            if(!q.exec()) qDebug() << q.lastError();
                        }
                        else qDebug() << "Query failed";
                    }
                }

                if(!q.first()) qDebug() << q.lastError();
                textval = q.value(0).toString();
                codeval = "";
                lastsymbol = textval[0];
                answerString += textval;
            }
            else answerString += " ";
        }
    }
    else
    {
        for (int i = 0; i < stringSize; i++)
        {
            textval = questionString[i];

            if(textval == "Ё") textval = "Е";
            else if(textval == "ё") textval = "е";
            else if((textval == 'c' && questionString[i+1] == 'h') ||
                    (textval == 'C' && questionString[i+1] == 'H') ||
                    (textval == 'C' && questionString[i+1] == 'h'))
            {
                if(q.prepare("SELECT id FROM morzeEncode WHERE latinic = 'CH' OR (latinic = 'Ch' OR latinicSmall ='ch')"))
                {
                    if(!q.exec()) qDebug() << q.lastError();
                }
                else qDebug() << "Query failed";
                if(!q.first()) qDebug() << q.lastError();
                g = q.value(0).toInt();

                if(q.prepare("SELECT code FROM morzeEncode WHERE id =:g"))
                {
                    q.bindValue(":g", g);
                    if(!q.exec()) qDebug() << q.lastError();
                }
                else qDebug() << "Query failed";
                if(!q.first()) qDebug() << q.lastError();
                textval = q.value(0).toString();
                i++;
                answerString += textval + ' ';
            }
            else if(textval == ' ')  answerString += " ";
            else if (textval == '\n') answerString += '\n';
            else if(textval == '.' || textval == ',' || textval == '!')
            {
                if(english)
                {
                    if(q.prepare("SELECT id FROM morzeEncode WHERE latinic =:textval OR latinicSmall =:textval"))
                    {
                        q.bindValue(":textval", textval);
                        if(!q.exec()) qDebug() << q.lastError() << "in enhlish true";
                    }
                    else qDebug() << "Query failed";
                    if(!q.first()) qDebug() << q.lastError() << "in enhlish true";
                    g = q.value(0).toInt();
                    qDebug() << g << "Last symbol is latinic";
                }
                else
                {
                    if(q.prepare("SELECT id FROM morzeEncode WHERE kyrillic =:textval OR kyrillicSmall =:textval"))
                    {
                        q.bindValue(":textval", textval);
                        if(!q.exec()) qDebug() << q.lastError() << "in enhlish false";
                    }
                    else qDebug() << "Query failed";
                    if(!q.first()) qDebug() << q.lastError() << "in enhlish false";
                    g = q.value(0).toInt();
                    qDebug() << g << "Last symbol is kyrillic";
                }

                if(q.prepare("SELECT code FROM morzeEncode WHERE id =:g"))
                {
                    q.bindValue(":g", g);
                    if(!q.exec()) qDebug() << q.lastError();
                }
                else qDebug() << "Query failed";
                if(!q.first()) qDebug() << q.lastError();
                textval = q.value(0).toString();

                answerString += textval + ' ';
            }
            else
            {
                if((textval > '@' && textval < '[') || (textval > '`' && textval < '{')) english = true;
                else if(textval >= "А" && textval <= "я") english = false;

                qDebug() << "English is " << english;

                if(q.prepare("SELECT id FROM morzeEncode WHERE (latinic =:textval OR latinicSmall =:textval) OR (kyrillic =:textval OR kyrillicSmall =:textval)"))
                {
                    q.bindValue(":textval", textval);
                    if(!q.exec()) qDebug() << q.lastError() << "in read symbol id";
                }
                else qDebug() << "Query failed";
                if(!q.first()) qDebug() << q.lastError() << "in read symbol id 2" << questionString[i];
                g = q.value(0).toInt();

                if(q.prepare("SELECT code FROM morzeEncode WHERE id =:g"))
                {
                    q.bindValue(":g", g);
                    if(!q.exec()) qDebug() << q.lastError() << "in read symbol from id";
                }
                else qDebug() << "Query failed";
                if(!q.first()) qDebug() << q.lastError() << "in read symbol from id 2" << questionString[i];
                textval = q.value(0).toString();

                answerString += textval + ' ';
            }
        }
    }
}

bool Encoder::langStatus()
{
    return russian;
}

QString Encoder::questionStringRead()
{
    return questionString;
}

QString Encoder::answerStringRead()
{
    return answerString;
}

void Encoder::changeLangStatus()
{
    russian = !russian;
    qDebug() << "Russian is" << russian;
}

void Encoder::changeQuestion(QString newStatus)
{
    questionString = newStatus;
    qDebug() << questionString;
}
