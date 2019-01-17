#ifndef CSVREADER_H
#define CSVREADER_H

#include <QString>

class CSVReader
{
    QString fileName;
    QString delimeter;
    bool removeQuotes;

public:
    CSVReader(QString filename, QString delm = ",", bool rm = true) :
            fileName(filename), delimeter(delm), removeQuotes(rm)
    { }

    // Function to fetch data from a CSV File
    QList<QList<QString> > getData();
};
#endif // CSVREADER_H
