#include "faustDSPManager.h"

#include <godot_cpp/classes/scene_tree.hpp>

using namespace godot;


void FaustDSPManager::_enter_tree() {
}

void FaustDSPManager::_exit_tree() {
}

void FaustDSPManager::loadDSP(const godot::String &path, bool showControlPanel, Ref<Faust2Godot> DSP) {
    UtilityFunctions::print("Loading DSP!");
    DSP->loadDSP(path, AudioServer::get_singleton()->get_mix_rate());

    if (showControlPanel) {
        this->instances.at(DSP) = memnew(godot::FaustControlPanel);
        this->instances.at(DSP)->setUpPanel(DSP, DSP->getMapUI());
        get_tree()->get_root()->call_deferred( "add_child", this->instances.at(DSP));
        this->instances.at(DSP)->call_deferred("popup_centered");
    }
}

Ref<Faust2Godot> FaustDSPManager::cloneDSP(const Ref<Faust2Godot> src) {
    Ref<Faust2Godot> newDSP;
    newDSP.instantiate();
    newDSP->setDSP(src->cloneDSP());
    instances.insert(std::pair<Ref<Faust2Godot>, FaustControlPanel*>(newDSP, nullptr));
    return newDSP;
}

Ref<Faust2Godot> FaustDSPManager::createNewDSPInstance() {
    Ref<Faust2Godot> newDSP;
    newDSP.instantiate();
    this->instances.insert(std::pair<Ref<Faust2Godot>, FaustControlPanel*>(newDSP, nullptr));
    return newDSP;
}

void FaustDSPManager::_bind_methods() {
    ClassDB::bind_method(D_METHOD("loadDSP", "path", "showControlPanel", "DSP"), &FaustDSPManager::loadDSP);
    ClassDB::bind_method(D_METHOD("cloneDSP", "src"), &FaustDSPManager::cloneDSP);
    ClassDB::bind_method(D_METHOD("createNewDSPInstance"), &FaustDSPManager::createNewDSPInstance);
}