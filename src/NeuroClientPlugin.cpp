// #include <QQmlExtensionPlugin>
// #include <qqml.h>

// #include "ApplicationOrchestrator.h"

// class NeuroClientPlugin : public QQmlExtensionPlugin
// {
//     Q_OBJECT
//     Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

// public:
//     void registerTypes(const char *uri) override
//     {
//         Q_ASSERT(uri == QLatin1String("NeuroClient"));

//         qmlRegisterSingletonType<ApplicationOrchestrator>(
//             uri, 1, 0, "App",
//             [](QQmlEngine*, QJSEngine*) -> QObject*
//             {
//                 auto *app = new ApplicationOrchestrator();

//                 app->init();

//                 return app;
//             }
//             );
//     }
// };

// #include "NeuroClientPlugin.moc"