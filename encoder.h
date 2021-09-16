#ifndef ENCODER_H
#define ENCODER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QFile>
#include <QTextCodec>
#include <QTextStream>
#include <QDebug>

class Encoder : public QObject
{
    Q_OBJECT
private:
    bool russian;
    QString questionString;
    QString answerString;
    QSqlDatabase db;

public:
    explicit Encoder(QObject *parent = nullptr);

    Q_INVOKABLE bool langStatus();

    Q_INVOKABLE QString questionStringRead();
    Q_INVOKABLE QString answerStringRead();

    Q_INVOKABLE void loadFromTXT(QString loadpath);
    Q_INVOKABLE void saveToTXT(QString savepath);
    Q_INVOKABLE void encode();
    Q_INVOKABLE void changeLangStatus();
    Q_INVOKABLE void changeQuestion(QString newStatus);
};

#endif // ENCODER_H
