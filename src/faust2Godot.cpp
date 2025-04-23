#include "faust2Godot.h"

#ifndef FAUST2GODOT_CPP
#define FAUST2GODOT_CPP

#include <godot_cpp/variant/utility_functions.hpp>

#include <dlfcn.h>

#include <godot_cpp/classes/time.hpp>

#endif

using namespace godot;

Faust2Godot::Faust2Godot() {
    UtilityFunctions::print("Constructing extension!");
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

void Faust2Godot::_bind_methods() {
    ClassDB::bind_method(D_METHOD("init_ext"), &Faust2Godot::initExtension);
    ClassDB::bind_method(D_METHOD("process"), &Faust2Godot::process);
}

void Faust2Godot::initExtension() {
    UtilityFunctions::print("Starting initialization!");

    // general setup
    input = new float*[2];
    input[0] = new float[20512];
    input[1] = new float[20512];

    output = new float*[2];
    output[0] = new float[512];
    output[1] = new float[512];

    currentTime = godot::Time::get_singleton();

    server = AudioServer::get_singleton();
    sampleRate = server->get_mix_rate();
    this->loadDSP();
    this->loadDSPLibrary();
    this->dspObject = this->createDSP();
    this->init_dsp(dspObject, sampleRate);

    // record bus setup

    recordBusIndex = server->get_bus_count();
    server->add_bus(recordBusIndex);
    server->set_bus_name(recordBusIndex, "recordBus");
    capture.instantiate();
    server->add_bus_effect(recordBusIndex, capture);
    server->set_bus_send(recordBusIndex, "Master");

    capturePlayer = memnew(AudioStreamPlayer);
    capturePlayer->set_name("recordBusPlayer");
    capturePlayer->set_bus("recordBus");
    server->set_bus_volume_db(recordBusIndex, 0);
    
    this->add_child(capturePlayer);
    
    captureStream = ResourceLoader::get_singleton()->load("res://assets/file_example_WAV_10MG.wav");
    if (!captureStream.is_valid()) {
        UtilityFunctions::print("Stream is invalid!");
    }
    capturePlayer->set_stream(captureStream);
    capturePlayer->play();

    // playback bus setup

    playbackBusIndex = server->get_bus_count();
    server->add_bus(playbackBusIndex);
    server->set_bus_name(playbackBusIndex, "playbackBus");
    server->set_bus_send(playbackBusIndex, "Master");

    playbackGenerator.instantiate();
    playbackGenerator->set_mix_rate(44100);
    playbackGenerator->set_buffer_length(0.2);

    playbackPlayer = memnew(AudioStreamPlayer);
    playbackPlayer->set_stream(playbackGenerator);
    playbackPlayer->set_bus("playbackBus");
    this->add_child(playbackPlayer);
    playbackPlayer->play();

    
    playbackStreamGenerator = playbackPlayer->get_stream_playback();

    startTime = currentTime->get_unix_time_from_system();
}

void Faust2Godot::process() {
    double engine_time = currentTime->get_unix_time_from_system();
    // UtilityFunctions::print("Time delta from last frame (s): ", (engine_time - startTime) - (lastFrameTime - startTime));
    lastFrameTime = engine_time;
    int maxBuffer = 1024;
    int available = capture->get_frames_available();
    PackedVector2Array buffer = capture->get_buffer(MIN<int, int>(maxBuffer, available));
    int bufferSize = buffer.size();
    UtilityFunctions::print("Buffer size: ", bufferSize);
    if (bufferSize == 0) {
        UtilityFunctions::print("Empty buffer, skipping process.");
        return;  
    }
    
    vector2ArrayToFloats(buffer, bufferSize, input);
    
    
    this->compute(this->dspObject, bufferSize, input, output);
    UtilityFunctions::print("Frames available: ", playbackStreamGenerator->get_frames_available());
    this->playbackStreamGenerator->push_buffer(buffer);

    // for (int i = 0; i < buffer.size(); i++) {
    //     UtilityFunctions::print("Amplitude left: ", Math::abs(output[0][i]), " Amplitude right: ", Math::abs(output[1][i]));
    // }

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

PackedVector2Array Faust2Godot::floatsToVector2Array(float **channels, int size) {
    PackedVector2Array array;

    for (int i = 0; i < size; i++) {
        array.push_back(Vector2(channels[0][i], channels[1][i]));
    }

    return array;
}

void Faust2Godot::vector2ArrayToFloats(PackedVector2Array array, int size, float **channels) {

    for (int i = 0; i < size; i++) {
        channels[0][i] = array[i].x;
        channels[1][i] = array[i].y;
    }
}