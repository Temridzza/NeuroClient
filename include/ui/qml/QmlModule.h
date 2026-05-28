#ifndef QMLMODULE_H
#define QMLMODULE_H

#include "ChatViewModel.h"
#include "ChatListModel.h"
#include "ProviderModel.h"
#include "ModelsModel.h"

class ChatListModel;
class ProviderModel;
class ModelsModel;

void registerQmlTypes(
    ChatListModel* chatModel,
    ProviderModel* providerVM,
    ModelsModel* modelsVM
    );

#endif