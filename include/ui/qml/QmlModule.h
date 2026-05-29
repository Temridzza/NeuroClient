#ifndef QMLMODULE_H
#define QMLMODULE_H

#include "ChatViewModel.h"
#include "ChatListViewModel.h"
#include "ProviderModel.h"
#include "ModelsModel.h"
#include "ChatOrchestrator.h"

class ChatListViewModel;
class ProviderModel;
class ModelsModel;
class ChatOrchestrator;

void registerQmlTypes(
    ChatListViewModel* chatModel,
    ProviderModel* providerVM,
    ModelsModel* modelsVM,
    ChatViewModel* chatVM
    );

#endif