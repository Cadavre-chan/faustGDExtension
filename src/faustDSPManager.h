#ifndef FAUST_DSP_MANAGER_H
#define FAUST_DSP_MANAGER_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/ref.hpp>
#include "faust2Godot.h"
#include "faustControlPanel.h"

namespace godot {

class FaustDSPManager : public Node {
    GDCLASS(FaustDSPManager, Node)

    private:
        Ref<Faust2Godot> faustInstance;
        FaustControlPanel *editorPanel = nullptr;

    public:
        FaustDSPManager() {
            UtilityFunctions::print("Constructing manager class!");
            faustInstance.instantiate();
        }

        static void _bind_methods();

        void loadDSP(const godot::String &path);

        Ref<Faust2Godot> getDSPInstance() const;

        virtual void _enter_tree() override;
        
        virtual void _exit_tree() override;
};



}

#endif