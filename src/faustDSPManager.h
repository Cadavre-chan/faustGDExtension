#ifndef FAUST_DSP_MANAGER_H
#define FAUST_DSP_MANAGER_H

#include <map>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/ref.hpp>
#include "faust2Godot.h"
#include "faustControlPanel.h"

namespace godot {

class FaustDSPManager : public Node {
    GDCLASS(FaustDSPManager, Node)

    private:
        std::map<Ref<Faust2Godot>, FaustControlPanel*> instances;
    public:
        FaustDSPManager() {
            UtilityFunctions::print("Constructing manager class!");
        }

        static void _bind_methods();

        void loadDSP(const godot::String &path, bool showControlPanel, Ref<Faust2Godot> DSP);

        Ref<Faust2Godot> cloneDSP(const Ref<Faust2Godot> src);

        Ref<Faust2Godot> createNewDSPInstance();

        virtual void _enter_tree() override;
        
        virtual void _exit_tree() override;
};



}

#endif