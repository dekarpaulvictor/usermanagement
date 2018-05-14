#ifndef MBDB_H
#define MBDB_H

#include <QString>

class MbDb
{
public:
    explicit MbDb(QString &db);
    bool dbConnect();
    void dbDisconnect();

private:
    QString dbName;
};

#endif // MBDB_H
