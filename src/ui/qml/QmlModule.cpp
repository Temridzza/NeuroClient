#include <qqml.h>

#include "QmlModule.h"
#include "ModelSelector.h"
#include "ProviderModel.h"
#include "ChatViewModel.h"

void registerQmlTypes(ChatListViewModel* chatModel,
                      ProviderModel* providerVM,
                      ModelsModel* modelsVM,
                      ChatViewModel* chatVM
                      ) {
    qmlRegisterType<ModelSelector>(
        "NeuroClient",
        1,
        0,
        "ModelSelector"
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

    qmlRegisterSingletonInstance(
        "NeuroClient",
        1,
        0,
        "ChatVM",
        chatVM
        );
}