#include <qqml.h>

#include "QmlModule.h"
#include "ModelSelector.h"
#include "ProviderModel.h"
#include "ChatViewModel.h"

void registerQmlTypes(ApplicationOrchestrator* app) {
    qmlRegisterSingletonInstance(
        "NeuroClient",
        1,
        0,
        "App",
        app
        );
}