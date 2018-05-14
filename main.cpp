#include <QGuiApplication>
#include <QQmlApplicationEngine>
//#include <QQmlContext>
#include <QDebug>

#include "mbdb.h"
#include "rbacusermanager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QString dbName("mbdb");

    MbDb db(dbName);

    if (!db.dbConnect())
    {
        qDebug() << "Could not connect to database!";
    }
    else
    {
     qDebug() << "Database connection successful!";
    }

    QQmlApplicationEngine engine;

    // Register types here
    qmlRegisterType<RBACUserManager>("ModioBurn.Tools", 1, 0, "UserManager");

    // Add context properties here
//    engine.rootContext()->setContextProperty("MoviesModel", initMoviesModel());

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
