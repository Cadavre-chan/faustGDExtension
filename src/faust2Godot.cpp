#include "faust2Godot.h"

#ifndef FAUST2GODOT_CPP
#define FAUST2GODOT_CPP

#include <godot_cpp/variant/utility_functions.hpp>

#include <dlfcn.h>

#include <godot_cpp/classes/time.hpp>

#endif

using namespace godot;

Faust2GodotEffectInstance::Faust2GodotEffectInstance() {
    UtilityFunctions::print("Starting initialization!");
    input = new float*[2];
    input[0] = new float[1024];
    input[1] = new float[1024];

    output = new float*[2];
    output[0] = new float[1024];
    output[1] = new float[1024];    
    sampleRate = AudioServer::get_singleton()->get_mix_rate();
    map_ui = new MapUI();
    this->init(dspPath, sampleRate);
}

Faust2GodotEffectInstance::~Faust2GodotEffectInstance() {
    delete dsp_instance;
    // dlclose(handle);
    delete[] input[0];
    delete[] input[1];
    delete[] input;

    delete[] output[0];
    delete[] output[1];
    delete[] output;

    delete map_ui;
}

void Faust2GodotEffectInstance::_bind_methods() {}

void Faust2GodotEffectInstance::_process(const void *srcptr, AudioFrame *dst, int frame_count) {
    AudioFrame *src = (AudioFrame*)srcptr;
    
    this->compute(src, dst, frame_count);
}

void Faust2GodotEffectInstance::init(godot::String path, int sample_rate) {
    UtilityFunctions::print("Initializing Effect Instance");

    sampleRate = sample_rate;
    handle = dlopen(path.utf8().get_data(), RTLD_NOW);
    
    if (!handle) {
        UtilityFunctions::print("Failed to open DSP");
    }
    
    void *(*dspConstructor)() = (void*(*)())dlsym(handle, "newmydsp");
    
    if (!dspConstructor) {
        UtilityFunctions::print("Failed to create DSP constructor");
    }
    
    dsp_instance = (dsp*) dspConstructor();
    if (!dsp_instance) {
        UtilityFunctions::print("Failed to use constructor");
    }
    
    dsp_instance->buildUserInterface(map_ui);
    dsp_instance->init(sample_rate);
    godot::Array names = this->get_all_params();

    for (int i = 0; i < names.size(); i++) {
        UtilityFunctions::print(names[i]);
    }
}

void Faust2GodotEffectInstance::set_param(godot::String path, float value) {
    map_ui->setParamValue(path.utf8().get_data(), value);
}

float Faust2GodotEffectInstance::get_param(godot::String path) {
    return map_ui->getParamValue(path.utf8().get_data());
}

godot::Array Faust2GodotEffectInstance::get_all_params() {
    godot::Array paths;

    int paramsCount = map_ui->getParamsCount();

    for (int i = 0; i < paramsCount; i++) {
        paths.append(map_ui->getParamAddress(i).c_str());
    }

    return paths;
}

void Faust2GodotEffectInstance::compute(const AudioFrame *src, AudioFrame *dst, int frames)
{

    for (int i = 0; i < frames; i++) {
        input[0][i] = src[i].left;
        input[1][i] = src[i].right;
    }

    dsp_instance->compute(frames, input, output);

    for (int i = 0; i < frames; i++) {
        dst[i].left = output[0][i];
        dst[i].right = output[1][i];
    }
}
Faust2Godot::Faust2Godot() {}
Faust2Godot::~Faust2Godot() {}

Ref<AudioEffectInstance> Faust2Godot::_instantiate() {
    Ref<Faust2GodotEffectInstance> effectInstance = memnew(Faust2GodotEffectInstance);
    
    instance = effectInstance;
    
    return effectInstance;
}

void Faust2Godot::_bind_methods() {
    ClassDB::bind_method(D_METHOD("setParam", "path", "value"), &Faust2Godot::set_param);
    ClassDB::bind_method(D_METHOD("getParam", "path"), &Faust2Godot::get_param);
    ClassDB::bind_method(D_METHOD("getAllParams"), &Faust2Godot::get_all_params);
}

