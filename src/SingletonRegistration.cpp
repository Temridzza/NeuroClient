// src/SingletonRegistration.cpp
#include <QtQml>
#include "ApplicationOrchestrator.h"

// Этот файл будет скомпилирован вместе с плагином, но **не** будет
// объявлять собственный плагин. Он просто вызывается один раз
// при загрузке модуля (qt_add_qml_module делает это автоматически).
static QObject* appSingletonProvider(QQmlEngine*, QJSEngine*) {
    return new ApplicationOrchestrator(); // без родителя
}

void registerNeuroClient() {
    qmlRegisterSingletonType<ApplicationOrchestrator>("NeuroClient", 1, 0, "App", appSingletonProvider);
}

struct Init { Init() { registerNeuroClient(); } };
static Init _init;