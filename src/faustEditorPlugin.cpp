#include "./faustEditorPlugin.h"

using namespace godot;

FaustEditorPlugin::FaustEditorPlugin() {}

FaustEditorPlugin::~FaustEditorPlugin() {}

void FaustEditorPlugin::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_on_slider_changed", "value", "path"), &FaustEditorPlugin::_on_slider_changed);
}

void FaustEditorPlugin::_enter_tree() {
    faustPanel = memnew(VBoxContainer);
    faustPanel->set_name("Faust DSP");

    Label *title = memnew(Label);
    title->set_text("Faust DSP Control Panel");
    faustPanel->add_child(title);

    faustEffect->_instantiate();
    faustEffect->init("/home/kad/grame_internship/faustGDExtension/demo/dsp/libfoo.so", 44100);

    Array paramPaths = faustEffect->get_all_params();

    for (int i = 0; i < paramPaths.size(); i++) {
        godot::String path = paramPaths[i];

        float min = 0.0;
        float max = 1.0;
        float init = faustEffect->get_param(path);

        Label *label = memnew(Label);
        label->set_text(path);
        faustPanel->add_child(label);

        HSlider *slider = memnew(HSlider);
        slider->set_min(min);
        slider->set_max(max);
        slider->set_step(0.01);
        slider->set_value(init);
        sliderMap[path] = slider;

        slider->connect("value_changed", Callable(this, "_on_slider_changed").bind(path));
        faustPanel->add_child(slider);
    }

    this->add_control_to_bottom_panel(faustPanel, "Faust DSP"); 
}

void FaustEditorPlugin::_exit_tree() {
    this->remove_control_from_bottom_panel(faustPanel);
    memdelete(faustPanel);
    faustPanel = nullptr;
    sliderMap.clear();
}
