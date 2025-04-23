#include "faust2Godot.h"

#ifndef FAUST2GODOT_CPP
#define FAUST2GODOT_CPP

#include <godot_cpp/variant/utility_functions.hpp>

#include <dlfcn.h>

#include <godot_cpp/classes/time.hpp>

#endif

using namespace godot;

Faust2Godot::Faust2Godot() {
    UtilityFunctions::print("Starting initialization!");

    // general setup
    input = new float*[2];
    input[0] = new float[1024];
    input[1] = new float[1024];

    output = new float*[2];
    output[0] = new float[1024];
    output[1] = new float[1024];

    sampleRate = AudioServer::get_singleton()->get_mix_rate();
    this->loadDSP();
    this->loadDSPLibrary();
    this->dspObject = this->createDSP();
    this->init_dsp(dspObject, sampleRate);
}

Faust2Godot::~Faust2Godot() {
    if (_dsp) {
        if (dspObject) {
            destroyDSP(dspObject);
        }
        dlclose(_dsp);
    }
    if (input != NULL && output != NULL) {
        delete[] input[0];
        delete[] input[1];
        delete[] input;
    
        delete[] output[0];
        delete[] output[1];
        delete[] output;
    }
}

// void Faust2Godot::_bind_methods() {
//     ClassDB::bind_method(D_METHOD("process"), &Faust2Godot::process);
// }

void Faust2Godot::process(const AudioFrame *src, AudioFrame *dst, int frame_count) {
    for (int i = 0; i < frame_count; i++) {
        input[0][i] = src[i].left;
        input[1][i] = src[i].right;
    }

    compute(dspObject, frame_count, input, output);

    for (int i = 0; i < frame_count; i++) {
        dst[i].left = output[0][i];
        dst[i].right = output[1][i];
    }
}

void Faust2Godot::loadDSPLibrary() {
    if (_dsp == nullptr) {
        UtilityFunctions::print("Failed to load DSP library.\n");
        return;
    }
    
    init_dsp = (void(*)(void*, int))dlsym(_dsp, "initmydsp");
    compute = (void(*)(void*, int, float**, float**))dlsym(_dsp, "computemydsp");
    getNumInputs = (int(*)(void*))dlsym(_dsp, "getNumInputsmydsp");
    getNumOutputs = (int(*)(void*))dlsym(_dsp, "getNumOutputsmydsp");
    createDSP = (void*(*)())dlsym(_dsp, "newmydsp");
    destroyDSP = (void(*)(void*))dlsym(_dsp, "deletemydsp");
}

void Faust2Godot::loadDSP() {
    this->_dsp = dlopen(dspPath.c_str(), RTLD_LAZY);
    if (!this->_dsp) {
        UtilityFunctions::print("Failed to load DSP.\n");
    }
}