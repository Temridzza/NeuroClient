#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>
#include <QLoggingCategory>

int main(int argc, char *argv[])
{
    // Включаем все возможные логи Qt/QML
    qSetMessagePattern(
        "[%{time hh:mm:ss.zzz}] "
        "%{if-debug}DEBUG%{endif}"
        "%{if-info}INFO%{endif}"
        "%{if-warning}WARNING%{endif}"
        "%{if-critical}CRITICAL%{endif}"
        "%{if-fatal}FATAL%{endif}"
        " %{category}: %{message}"
        );

    QLoggingCategory::setFilterRules(QStringLiteral(
        "*.debug=true\n"
        "qt.qml.debug=true\n"
        "qt.qml.binding.removal.info=true\n"
        "qt.qml.connections=true\n"
        "qt.qml.gc=true\n"
        "qt.qml.imports=true\n"
        "qt.qml.loading=true\n"
        "qt.qml.memory=true\n"
        "qt.qml.objectcreation=true\n"
        "qt.qml.translation=true\n"
        "qt.quick.*=true\n"
        "qt.scenegraph.*=true\n"
        "qt.qpa.*=true\n"
        ));

    qputenv("QML_IMPORT_TRACE", "1");
    qputenv("QML_DISABLE_DISK_CACHE", "1");

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qDebug() << "Import paths BEFORE:";
    for (const auto &path : engine.importPathList())
        qDebug() << " " << path;

    engine.addImportPath("/etc/nixos/Ambxst/modules");
    engine.addImportPath("/home/temridzza/Documents/NeuroClient/qml");

    qDebug() << "Import paths AFTER:";
    for (const auto &path : engine.importPathList())
        qDebug() << " " << path;

    const QUrl url(QStringLiteral(
        "file:///home/temridzza/Documents/NeuroClient/shell.qml"));

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::warnings,
        [](const QList<QQmlError> &warnings) {
            qCritical() << "========== QML WARNINGS ==========";
            for (const auto &warning : warnings)
                qCritical().noquote() << warning.toString();
            qCritical() << "==================================";
        });

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            qDebug() << "objectCreated:" << objUrl;

            if (!obj && objUrl == url) {
                qCritical() << "Failed to load root QML object!";
                QCoreApplication::exit(-1);
            }

            if (obj)
                qDebug() << "Root object created:" << obj;
        },
        Qt::QueuedConnection);

    qDebug() << "Loading:" << url;

    engine.load(url);

    qDebug() << "Root objects count:"
             << engine.rootObjects().size();

    for (QObject *obj : engine.rootObjects())
        qDebug() << "Root object:" << obj;

    return app.exec();
}