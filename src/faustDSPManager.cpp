#include "faustDSPManager.h"

#include <godot_cpp/classes/scene_tree.hpp>

using namespace godot;


void FaustDSPManager::_enter_tree() {
    UtilityFunctions::print("Enterting tree!");
}

void FaustDSPManager::_exit_tree() {
}

void FaustDSPManager::loadDSP(const godot::String &path) {
    UtilityFunctions::print("Loading DSP!");
    this->editorPanel = memnew(godot::FaustControlPanel);
    faustInstance->loadDSP(path, AudioServer::get_singleton()->get_mix_rate());
    this->editorPanel->setUpPanel(faustInstance, faustInstance->getMapUI());
    get_tree()->get_root()->call_deferred( "add_child", this->editorPanel);
    UtilityFunctions::print("The bug lies here, billions must die");
    this->editorPanel->call_deferred("popup_centered");
}

Ref<Faust2Godot> FaustDSPManager::getDSPInstance() const {
    return faustInstance;
}

void FaustDSPManager::_bind_methods() {
    ClassDB::bind_method(D_METHOD("loadDSP", "path"), &FaustDSPManager::loadDSP);
    ClassDB::bind_method(D_METHOD("getDSPInstance"), &FaustDSPManager::getDSPInstance);
}