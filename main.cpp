#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "model/Connection_Client.h"
#include "client.h"


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    Client client;
//    qmlRegisterType<Client>("com.import", 1, 0, "Client");

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("client", &client);
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();

}
