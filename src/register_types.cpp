#include "register_types.h"

#include "faust2Godot.h"
#include "faustControlPanel.h"
#include "faustDSPManager.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/editor_interface.hpp>

using namespace godot;


void initialize_example_module(ModuleInitializationLevel p_level) {
    if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
		ClassDB::register_class<Faust2GodotEffectInstance>();
		ClassDB::register_class<Faust2Godot>();
        ClassDB::register_class<FaustControlPanel>();
		ClassDB::register_class<FaustDSPManager>();
    }
}

void uninitialize_example_module(ModuleInitializationLevel p_level) {

}

extern "C" {
GDExtensionBool GDE_EXPORT example_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
	init_obj.register_initializer(initialize_example_module);
	init_obj.register_terminator(uninitialize_example_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}