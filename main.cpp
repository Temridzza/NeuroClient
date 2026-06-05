#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "SingletonRegistration.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    registerNeuroClient(); // теперь "App" доступен в QML

    QQmlApplicationEngine engine;
    engine.load(QStringLiteral("qrc:/Main.qml")); // или build путь к Main.qml

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}