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
    mapUI = new ExtendedMapUI();
}

Faust2GodotEffectInstance::~Faust2GodotEffectInstance() {
    delete dsp_instance;
    delete[] input[0];
    delete[] input[1];
    delete[] input;

    delete[] output[0];
    delete[] output[1];
    delete[] output;

    delete mapUI;

    if (this->handle != NULL) {
        dlclose(handle);
    }
    if (this->dsp_instance != NULL) {
        this->dsp_instance->~dsp();
    }
}

void Faust2GodotEffectInstance::_bind_methods() {}

void Faust2GodotEffectInstance::_process(const void *srcptr, AudioFrame *dst, int frame_count) {
    AudioFrame *src = (AudioFrame*)srcptr;
    
    this->compute(src, dst, frame_count);
}

void Faust2GodotEffectInstance::init(godot::String path, int sample_rate) {
    
    CharString cpath = path.utf8();
    handle = dlopen(cpath.get_data(), RTLD_NOW);
    UtilityFunctions::print(path, " ", sample_rate);

    sampleRate = sample_rate;

    void *(*dspConstructor)() = (void*(*)())dlsym(handle, "newmydsp");
    dsp_instance = (dsp*) dspConstructor();
    
    dsp_instance->buildUserInterface(mapUI);
    dsp_instance->init(sample_rate);
    godot::Array names = this->get_all_params();

    for (int i = 0; i < names.size(); i++) {
        UtilityFunctions::print(names[i]);
    }
}

void Faust2GodotEffectInstance::set_param(godot::String path, float value) {
    mapUI->setParamValue(path.utf8().get_data(), value);
}

float Faust2GodotEffectInstance::get_param(godot::String path) {
    return mapUI->getParamValue(path.utf8().get_data());
}

godot::Array Faust2GodotEffectInstance::get_all_params() {
    godot::Array paths;

    int paramsCount = mapUI->getParamsCount();

    for (int i = 0; i < paramsCount; i++) {
        paths.append(mapUI->getParamAddress(i).c_str());
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

dsp *Faust2GodotEffectInstance::cloneDSP() {
    return this->dsp_instance->clone();
}

void Faust2GodotEffectInstance::setDSP(dsp *DSP) {
    this->dsp_instance = DSP;
}

Faust2Godot::Faust2Godot() {}
Faust2Godot::~Faust2Godot() {}

Ref<AudioEffectInstance> Faust2Godot::_instantiate() {
    Ref<Faust2GodotEffectInstance> effectInstance = memnew(Faust2GodotEffectInstance);
    
    instance = effectInstance;
    
    return effectInstance;
}

void Faust2Godot::loadDSP(godot::String path, int pSampleRate) {
    if (instance.is_null()) {
        UtilityFunctions::printerr("Instance not initialized. Please add Faust2Godot to your audio bus before calling this function.");
        return;
    }
    instance.ptr()->init(path, pSampleRate);
}

void Faust2Godot::_bind_methods() {
    ClassDB::bind_method(D_METHOD("setParam", "path", "value"), &Faust2Godot::set_param);
    ClassDB::bind_method(D_METHOD("getParam", "path"), &Faust2Godot::get_param);
    ClassDB::bind_method(D_METHOD("getAllParams"), &Faust2Godot::get_all_params);
    ClassDB::bind_method(D_METHOD("loadDSP", "path", "sampleRate"), &Faust2Godot::loadDSP);
}
