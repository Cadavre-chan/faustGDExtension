#include "gdexample.h"
#include <cstddef>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/audio_server.hpp>
#include <cmath>
#include <iterator>
#include <memory>
#include <stdexcept>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif
#include <fstream>
#include <iostream>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <exception>


using namespace godot;

void GDExample::_bind_methods() {
    ClassDB::bind_method(D_METHOD("init_ext", "playback", "sample_rate", "frequency"), &GDExample::init_ext);
    ClassDB::bind_method(D_METHOD("process", "input_sample"), &GDExample::process);
}

GDExample::GDExample() {
    phase = 0.0f;
    sample_rate = AudioServer::get_singleton()->get_mix_rate();  // Default sample rate
    frequency = 440.0f;  // Default frequency (A4)
}

GDExample::~GDExample() {
    if (_dsp) {
        if (dspObject) {
            destroyDSP(dspObject);
        }
        dlclose(_dsp);
    }
}

void GDExample::init_ext(Ref<AudioStreamGeneratorPlayback> p_playback, float p_sample_rate, float p_frequency) {
    playback = p_playback;
    sample_rate = p_sample_rate;
    frequency = p_frequency;
    load_faust_dll();
    load_dsp_library();
}

void GDExample::process() {
    if (playback.is_null()) return;

    dspObject = createDSP();
    if (dspObject == NULL) {
        UtilityFunctions::print("failed to allocate DSP\n");
    }

    if (dspObject == NULL) {
        std::cerr << "failed to allocate DSP" << std::endl;
    }

    init_dsp(dspObject, sample_rate);

    int buffer_size;
    while (true) {
        buffer_size = playback->get_frames_available();
        float output[buffer_size];
        compute(dspObject, buffer_size, NULL, &output);
        playback->push_frame(Vector2(output, output));  // Stereo output (L, R)
    }
    // return dsp_process ? dsp_process(input_sample) : 0.0f;
}

bool GDExample::compile_faust_code(const std::string& faust_code, const std::string& output_path) {
    std::ofstream faust_file(output_path);
    if (!faust_file) {
        UtilityFunctions::print("Failed to write Faust file.\n");
        return false;
    }
    faust_file << faust_code;
    faust_file.close();

    std::string cpp_output = output_path + ".cpp";
    std::string command = "faust -lang cpp -cn FaustDSP -o " + cpp_output + " " + output_path;
    
    if (std::system(command.c_str()) != 0) {
        UtilityFunctions::print("Faust compilation failed.\n");
        return false;
    }

    return compile_shared_library(cpp_output, output_path + ".so");
}

bool GDExample::compile_shared_library(const std::string& cpp_path, const std::string& so_path) {
    std::string command = "g++ -fPIC -shared -o " + so_path + " " + cpp_path;
    return std::system(command.c_str()) == 0;
}

bool GDExample::load_dsp_library() {

    if (_dsp == nullptr) {
        UtilityFunctions::print("Failed to load DSP library.\n");
        return false;
    }

    init_dsp = (void(*)(void*, int))dlsym(_dsp, "initmydsp");
    compute = (void(*)(void*, int, float**, float**))dlsym(_dsp, "computemydsp");
    getNumInputs = (int(*)(void*))dlsym(_dsp, "getNumInputsmydsp");
    getNumOutputs = (int(*)(void*))dlsym(_dsp, "getNumOutputsmydsp");
    createDSP = (void*(*)())dlsym(_dsp, "newmydsp");
    destroyDSP = (void(*)(void*))dlsym(_dsp, "deletemydsp");

    return true;
}

void GDExample::load_faust_dll() {
    #ifdef _WIN32
    _dsp = LoadLibrary(library_path.c_str());
    if (_dsp == nullptr) {
        std::cerr << "Failed to load DSP: " << path << "\n";
    }
    #else

    _dsp = dlopen(library_path.c_str(), RTLD_LAZY), dlclose;
    if (!_dsp) {
        exit(84);
    }
    #endif
}


// void GDExample::_bind_methods() {
// 	ClassDB::bind_method(D_METHOD("get_amplitude"), &GDExample::get_amplitude);
// 	ClassDB::bind_method(D_METHOD("set_amplitude", "p_amplitude"), &GDExample::set_amplitude);

// 	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "amplitude"), "set_amplitude", "get_amplitude");

// 	ClassDB::bind_method(D_METHOD("get_speed"), &GDExample::get_speed);
// 	ClassDB::bind_method(D_METHOD("set_speed", "p_speed"), &GDExample::set_speed);

// 	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed", PROPERTY_HINT_RANGE, "0,20,0.01"), "set_speed", "get_speed");
// 	ADD_SIGNAL(MethodInfo("position_changed", PropertyInfo(Variant::OBJECT, "node"), PropertyInfo(Variant::VECTOR2, "new_pos")));
// }

// GDExample::GDExample() {
// 	// Initialize any variables here.
// 	time_passed = 0.0;
// 	amplitude = 10.0;
// 	speed = 1.0;
// }

// GDExample::~GDExample() {
// 	// Add your cleanup here.
// }

// void GDExample::_process(double delta) {
// 	time_passed += speed * delta;

// 	Vector2 new_position = Vector2(
// 		amplitude + (amplitude * sin(time_passed * 2.0)),
// 		amplitude + (amplitude * cos(time_passed * 1.5))
// 	);

// 	set_position(new_position);

// 	time_emit += delta;
// 	if (time_emit > 1.0) {
// 		emit_signal("position_changed", this, new_position);

// 		time_emit = 0.0;
// 	}
// }

// void GDExample::set_amplitude(const double p_amplitude) {
// 	amplitude = p_amplitude;
// }

// double GDExample::get_amplitude() const {
// 	return amplitude;
// }

// void GDExample::set_speed(const double p_speed) {
// 	speed = p_speed;
// }

// double GDExample::get_speed() const {
// 	return speed;
// }
