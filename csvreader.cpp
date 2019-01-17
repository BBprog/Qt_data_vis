#include "csvreader.h"

#include <QFile>
#include <QDebug>

QList<QList<QString> > CSVReader::getData()
{
    QFile file(fileName);
    QList<QList<QString> > wordList;

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        return wordList;
    }

    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine();

        if (removeQuotes) line.remove("\"");

        QString regExp = "[" + delimeter + "]+";
        QStringList list = line.split(QRegExp(regExp), QString::KeepEmptyParts);

        wordList.append(list);
    }

    return wordList;
}
