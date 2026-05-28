#include <qqml.h>

#include "QmlModule.h"
#include "ModelSelector.h"
#include "ProviderModel.h"

void registerQmlTypes(ChatListModel* chatModel,
                      ProviderModel* providerVM,
                      ModelsModel* modelsVM
                      ) {
    qmlRegisterType<ModelSelector>(
        "NeuroClient",
        1,
        0,
        "ModelSelector"
        );

    qmlRegisterType<ChatViewModel>(
        "NeuroClient",
        1,
        0,
        "ChatViewModel"
        );
    qmlRegisterSingletonInstance(
        "NeuroClient",
        1,
        0,
        "ChatModel",
        chatModel
        );
    qmlRegisterSingletonInstance(
        "NeuroClient",
        1,
        0,
        "ProviderVM",
        providerVM
        );
    qmlRegisterSingletonInstance(
        "NeuroClient",
        1,
        0,
        "ModelsVM",
        modelsVM
        );
}