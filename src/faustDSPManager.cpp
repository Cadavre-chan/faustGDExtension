#include "faustDSPManager.h"

using namespace godot;

static FaustDSPManager *singleton = nullptr;

FaustDSPManager *FaustDSPManager::getSingleton() {
    return singleton;
}

void FaustDSPManager::_enter_tree() {
    singleton = this;

    editorPlugin = memnew(FaustEditorPlugin);
    editorPlugin->setDSPInstance(faustInstance);
    add_child(editorPlugin);
}

void FaustDSPManager::_exit_tree() {
    memdelete(editorPlugin);
    editorPlugin = nullptr;

    singleton = nullptr;
}

void FaustDSPManager::loadDSP(const godot::String &path) {
    faustInstance.instantiate();
    faustInstance->init(path, AudioServer::get_singleton()->get_mix_rate());

    editorPlugin->setDSPInstance(faustInstance);
}

Ref<Faust2Godot> FaustDSPManager::getDSPInstance() const {
    return faustInstance;
}
