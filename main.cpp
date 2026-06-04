#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QLoggingCategory>
#include <QDebug>

int main(int argc, char *argv[])
{
    // --- базовые логи Qt/QML ---
    QLoggingCategory::setFilterRules(QStringLiteral(
        "*.debug=true\n"
        "qt.qml.debug=true\n"
        "qt.qml.imports=true\n"
        "qt.qml.loading=true\n"
        "qt.qml.errors=true\n"
        "qt.quick.*=true\n"
        ));

    // qputenv("QML_IMPORT_TRACE", "1".toLatin1());
    // qputenv("QML_DISABLE_DISK_CACHE", "1".toLatin1());

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // --- твои import paths ---
    engine.addImportPath("/home/temridzza/Documents/NeuroClient/qml");
    engine.addImportPath("/etc/nixos/Ambxst/modules");

    // --- лог import paths ---
    qDebug() << "QML Import paths:";
    for (const auto &p : engine.importPathList())
        qDebug() << " -" << p;

    // --- обработка ошибок QML ---
    QObject::connect(&engine, &QQmlApplicationEngine::warnings,
                     [](const QList<QQmlError> &warnings) {
                         qWarning() << "QML WARNINGS:";
                         for (const auto &w : warnings)
                             qWarning().noquote() << w.toString();
                     });

    // --- root load ---
    const QUrl url(QStringLiteral("file:/home/temridzza/Documents/NeuroClient/shell.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                         if (!obj && url == objUrl) {
                             qCritical() << "FAILED TO LOAD ROOT QML";
                             QCoreApplication::exit(-1);
                         }

                         if (obj)
                             qDebug() << "Root QML loaded OK";
                     });

    engine.load(url);

    return app.exec();
}