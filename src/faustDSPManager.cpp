#include "faustDSPManager.h"

using namespace godot;


void FaustDSPManager::_enter_tree() {

    editorPlugin = memnew(FaustEditorPlugin);
    editorPlugin->setDSPInstance(faustInstance);
    add_child(editorPlugin);
}

void FaustDSPManager::_exit_tree() {
    memdelete(editorPlugin);
    editorPlugin = nullptr;
}

void FaustDSPManager::loadDSP(const godot::String &path) {
    faustInstance->loadDSP(path, AudioServer::get_singleton()->get_mix_rate());
}

Ref<Faust2Godot> FaustDSPManager::getDSPInstance() const {
    return faustInstance;
}

void FaustDSPManager::_bind_methods() {
    ClassDB::bind_method(D_METHOD("loadDSP", "path"), &FaustDSPManager::loadDSP);
    ClassDB::bind_method(D_METHOD("getDSPInstance"), &FaustDSPManager::getDSPInstance);
}