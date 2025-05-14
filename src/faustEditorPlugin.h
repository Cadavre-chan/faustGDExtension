#ifndef FAUST_EDITOR_PLUGIN_H
#define FAUST_EDITOR_PLUGIN_H

#include "faust2Godot.h"

#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/h_slider.hpp>
#include <godot_cpp/classes/control.hpp>
#include <map>


namespace godot {
    class FaustEditorPlugin : public EditorPlugin {
        GDCLASS(FaustEditorPlugin, EditorPlugin);
    
    private:
        VBoxContainer *faustPanel = nullptr;

        Ref<Faust2Godot> faustEffect;

        std::map<godot::String, HSlider*> sliderMap;
    
    public:
        FaustEditorPlugin();
        ~FaustEditorPlugin();

        virtual void _enter_tree() override;
    
        virtual void _exit_tree() override;

        void _on_slider_changed(double value, godot::String path);

        static void _bind_methods();
    };

}


#endif