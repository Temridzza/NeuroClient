#include <qqml.h>

#include "ModelSelectorViewModel.h"

void registerQmlTypes() {
    qmlRegisterType<ModelSelectorViewModel>(
        "NeuroClient",
        1,
        0,
        "ModelSelectorViewModel"
        );
}