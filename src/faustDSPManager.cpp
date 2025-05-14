#include "faustDSPManager.h"

using namespace godot;

static FaustDSPManager *singleton = nullptr;

FaustDSPManager *FaustDSPManager::getSingleton() {
    return singleton;
}