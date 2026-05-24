#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "QmlModule.h"

#include <QDir>
#include <QFile>
#include <QFileSystemWatcher>
#include <QLockFile>

#include "DatabaseManager.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    QString lockFilePath =
        QDir::temp().absoluteFilePath("myapp.lock");

    QLockFile lockFile(lockFilePath);

    if (!lockFile.tryLock(100)) {
        QFile msgFile(
            QDir::temp().absoluteFilePath("myapp.msg")
            );

        msgFile.open(QIODevice::WriteOnly);
        msgFile.write("raise");
        msgFile.close();

        return 0;
    }

    DatabaseManager::instance().init("chat_database.db");

    // 👇 РЕГИСТРАЦИЯ QML ТИПОВ
    registerQmlTypes();

    QQmlApplicationEngine engine;

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() {
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection
        );

    engine.loadFromModule("ChatGPT", "Main");

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}