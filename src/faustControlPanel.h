#ifndef FAUST_CONTROL_PANEL_H
#define FAUST_CONTROL_PANEL_H

#include "faust2Godot.h"

#include <godot_cpp/classes/popup_panel.hpp>
#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/h_slider.hpp>
#include <godot_cpp/classes/control.hpp>
#include <map>


namespace godot {
    class FaustControlPanel : public Window {
        GDCLASS(FaustControlPanel, Window);
    
    private:
        VBoxContainer *faustPanel = nullptr;
        Label *title = nullptr;

        Ref<Faust2Godot> faustEffect = nullptr;

        std::map<godot::String, HSlider*> sliderMap;

        ExtendedMapUI *mapUI = nullptr;
    
    public:
        FaustControlPanel();
        ~FaustControlPanel();

        void _on_slider_changed(double value, godot::String path);

        void setUpPanel(Ref<Faust2Godot> DSPInstance, ExtendedMapUI *mapUIPtr);


        static void _bind_methods();
    };

}


#endif