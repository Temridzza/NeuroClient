#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "QmlModule.h"

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileSystemWatcher>
#include <QLockFile>

#include "DatabaseManager.h"

void messageHandler(
    QtMsgType type,
    const QMessageLogContext &context,
    const QString &msg
    ) {
    QString level;

    switch (type) {
    case QtDebugMsg:
        level = "DEBUG";
        break;

    case QtInfoMsg:
        level = "INFO";
        break;

    case QtWarningMsg:
        level = "WARNING";
        break;

    case QtCriticalMsg:
        level = "CRITICAL";
        break;

    case QtFatalMsg:
        level = "FATAL";
        break;
    }

    QString output = QString("[%1] [%2] %3")
                         .arg(
                             QDateTime::currentDateTime()
                                 .toString("hh:mm:ss")
                             )
                         .arg(level)
                         .arg(msg);

    if (context.file) {
        output += QString("\n -> %1:%2")
        .arg(context.file)
            .arg(context.line);
    }

    fprintf(stderr, "%s\n",
            output.toLocal8Bit().constData());

    fflush(stderr);

    if (type == QtFatalMsg) {
        abort();
    }
}

int main(int argc, char *argv[]) {

    // 👇 УСТАНОВКА ОБРАБОТЧИКА ЛОГОВ
    qInstallMessageHandler(messageHandler);

    QGuiApplication app(argc, argv);

    if (!DatabaseManager::instance().init("chat.db")) {
        qFatal("Database initialization failed");
        return -1;
    }
    auto& db = DatabaseManager::instance().db();

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

    DatabaseManager::instance().init(
        "chat_database.db"
        );

    registerQmlTypes();

    QQmlApplicationEngine engine;

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() {
            qCritical()
            << "QML object creation failed";

            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection
        );

    engine.addImportPath("qrc:/");
    engine.loadFromModule(
        "ChatGPT",
        "Main"
        );

    if (engine.rootObjects().isEmpty()) {
        qCritical()
        << "Root objects are empty";

        return -1;
    }

    return app.exec();
}