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

    sampleRate = AudioServer::get_singleton()->get_mix_rate();
    dspWrapper.init(dspPath, sampleRate);
}

Faust2GodotEffectInstance::~Faust2GodotEffectInstance() {
}

void Faust2GodotEffectInstance::_bind_methods() {
}

void Faust2GodotEffectInstance::_process(const void *srcptr, AudioFrame *dst, int frame_count) {

    AudioFrame *src = (AudioFrame*)srcptr;

    dspWrapper.compute(src, dst, frame_count);
}


Ref<AudioEffectInstance> Faust2Godot::_instantiate() {
    Ref<Faust2GodotEffectInstance> effectInstance = memnew(Faust2GodotEffectInstance);

    instance = effectInstance;

    return effectInstance;
}

Faust2Godot::Faust2Godot() {

}
Faust2Godot::~Faust2Godot() {

}

void Faust2Godot::_bind_methods() {
    ClassDB::bind_method(D_METHOD("instantiate"), &Faust2Godot::_instantiate);
}


FaustDSPWrapper::FaustDSPWrapper() {
    input = new float*[2];
    input[0] = new float[1024];
    input[1] = new float[1024];

    output = new float*[2];
    output[0] = new float[1024];
    output[1] = new float[1024];
}

FaustDSPWrapper::~FaustDSPWrapper() {
    if (handle) {
        dlclose(handle);
        delete dsp_instance;
    }
    delete[] input[0];
    delete[] input[1];
    delete[] input;

    delete[] output[0];
    delete[] output[1];
    delete[] output;

}

void FaustDSPWrapper::init(godot::String path, int sample_rate) {
    handle = dlopen(path.utf8().get_data(), RTLD_NOW);

    if (!handle) {
        ERR_FAIL_MSG("Failed to open DSP");
    }

    dsp *(*dspConstructor)() = (dsp*(*)())dlsym(handle, "newmydsp");

    dsp_instance = dspConstructor();

    dsp_instance->init(sample_rate);
    dsp_instance->buildUserInterface(&map_ui);
}

void FaustDSPWrapper::set_param(godot::String path, float value) {
    map_ui.setParamValue(path.utf8().get_data(), value);
}

float FaustDSPWrapper::get_param(godot::String path) {
    return map_ui.getParamValue(path.utf8().get_data());
}

godot::Array FaustDSPWrapper::get_all_params() {
    godot::Array paths;

    int paramsCount = map_ui.getParamsCount();

    for (int i = 0; i < paramsCount; i++) {
        paths.append(map_ui.getParamAddress(i).c_str());
    }

    return paths;
}

void FaustDSPWrapper::compute(const AudioFrame *src, AudioFrame *dst, int frames)
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

void FaustDSPWrapper::_bind_methods() {
    ClassDB::bind_method(D_METHOD("init", "path", "sampleRate"), &FaustDSPWrapper::init);
    ClassDB::bind_method(D_METHOD("setParam", "path", "value"), &FaustDSPWrapper::set_param);
    ClassDB::bind_method(D_METHOD("getParam", "path"), &FaustDSPWrapper::get_param);
    ClassDB::bind_method(D_METHOD("getAllParams"), &FaustDSPWrapper::get_all_params);
}