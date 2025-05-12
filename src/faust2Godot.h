#ifndef FAUST2GODOT_HPP
#define FAUST2GODOT_HPP

#include <string>
#include <vector>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/audio_stream_generator_playback.hpp>
#include <godot_cpp/classes/audio_stream_generator.hpp>
#include <godot_cpp/classes/audio_server.hpp>
#include <godot_cpp/classes/audio_effect_capture.hpp>
#include <godot_cpp/classes/audio_effect_instance.hpp>
#include <godot_cpp/classes/audio_stream_player.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/classes/audio_frame.hpp>
#include <godot_cpp/classes/audio_effect.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/string.hpp>
#include <faust/dsp/dsp.h>
#include <faust/gui/MapUI.h>


namespace godot {

class Faust2GodotEffectInstance : public godot::AudioEffectInstance {
    GDCLASS(Faust2GodotEffectInstance, godot::AudioEffectInstance);
    
    private:
        int sampleRate;
        float **input = NULL;
        float **output = NULL;
        
        void *handle;
        dsp *dsp_instance;
        MapUI *map_ui;
        godot::String dspPath = "/home/kad/grame_internship/faustGDExtension/demo/dsp/libfoo.so"; // path to DSP .so file

    public:
        Faust2GodotEffectInstance();
        ~Faust2GodotEffectInstance();
        void _process(const void *srcptr, AudioFrame *dst, int frame_count); // function called at every cycle
        
        static void _bind_methods();

        void init(godot::String path, int sample_rate);
    
    
        void set_param(godot::String path, float value);
    
    
        float get_param(godot::String path);
    
    
        void compute(const AudioFrame *src, AudioFrame *dst, int frames);

        godot::Array get_all_params();
};

class Faust2Godot : public godot::AudioEffect {
    GDCLASS(Faust2Godot, godot::AudioEffect);

    public:
        Ref<AudioEffectInstance> _instantiate() override;
        static void _bind_methods();
        Faust2Godot();
        ~Faust2Godot();
        
        Ref<Faust2GodotEffectInstance> instance = nullptr;


        void init(godot::String path, int sample_rate) {
            instance.ptr()->init(path, sample_rate);
        };


        void set_param(godot::String path, float value) {
            instance.ptr()->set_param(path, value);
        };


        float get_param(godot::String path) {
            return instance.ptr()->get_param(path);
        };


        godot::Array get_all_params() {
            return instance.ptr()->get_all_params();
        };
};

}


#endif