#include <qqml.h>

#include "ModelSelectorViewModel.h"
#include "ChatViewModel.h"

void registerQmlTypes() {
    qmlRegisterType<ModelSelectorViewModel>(
        "NeuroClient",
        1,
        0,
        "ModelSelectorViewModel"
        );

    qmlRegisterType<ChatViewModel>(
        "NeuroClient",
        1,
        0,
        "ChatViewModel"
        );
}