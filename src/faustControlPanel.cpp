#include "./faustControlPanel.h"

using namespace godot;

FaustControlPanel::FaustControlPanel() {
    
}

FaustControlPanel::~FaustControlPanel() {}

void FaustControlPanel::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_on_slider_changed", "value", "path"), &FaustControlPanel::_on_slider_changed);
}

void FaustControlPanel::setUpPanel(Ref<Faust2Godot> DSPInstance, ExtendedMapUI *mapUIPtr) {
    faustPanel = memnew(VBoxContainer);
    title = memnew(Label);
    
    faustPanel->add_child(title);
    this->add_child(faustPanel);
    this->set_size( godot::Vector2i( 400, 300));
    
    mapUI = mapUIPtr;
    faustEffect = DSPInstance;

    Array paramPaths = faustEffect->get_all_params();
    
    for (int i = 0; i < paramPaths.size(); i++) {
        godot::String path = paramPaths[i];
    
        float min = mapUI->getParamMin(path.utf8().get_data());
        float max = mapUI->getParamMax(path.utf8().get_data());
        float init = faustEffect->get_param(path);
    
        title = memnew(Label);
        title->set_text(path);
        faustPanel->add_child(title);
    
        HSlider *slider = memnew(HSlider);
        slider->set_min(min);
        slider->set_max(max);
        slider->set_step(0.01);
        slider->set_value(init);
        faustPanel->add_child(slider);
        
        slider->connect("value_changed", Callable(this, "_on_slider_changed").bind(path));
        sliderMap[path] = slider;
    }
}

void FaustControlPanel::_on_slider_changed(double value, godot::String path) {
    faustEffect->set_param(path, value);
    UtilityFunctions::print("Updated param: ", path, " to ", value);
}

