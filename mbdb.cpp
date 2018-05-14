#include "mbdb.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

MbDb::MbDb(QString &db) :
    dbName(db)
{}

bool MbDb::dbConnect()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setDatabaseName(dbName);
    db.setHostName("localhost");
    db.setUserName("vpd");
    db.setPassword("xesefassixesfles432");

    if (!db.open())
    {
        qDebug("Could not open database. Error: %s", qPrintable(db.lastError().text()));
        return false;
    }

    return true;
}

void MbDb::dbDisconnect()
{
    QSqlDatabase::database().close();
}
