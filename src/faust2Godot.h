#ifndef FAUST2GODOT_HPP
#define FAUST2GODOT_HPP

#include <string>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/audio_stream_generator_playback.hpp>
#include <godot_cpp/classes/audio_stream_generator.hpp>
#include <godot_cpp/classes/audio_server.hpp>
#include <godot_cpp/classes/audio_effect_capture.hpp>
#include <godot_cpp/classes/audio_effect_instance.hpp>
#include <godot_cpp/classes/audio_stream_player.hpp>
#include <godot_cpp/variant/packed_vector2_array.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/classes/audio_frame.hpp>
#include <godot_cpp/classes/audio_effect.hpp>
#include <godot_cpp/classes/weak_ref.hpp>

namespace godot {
class Faust2GodotEffectInstance : public godot::AudioEffectInstance {
    GDCLASS(Faust2GodotEffectInstance, godot::AudioEffectInstance);

    private:
    
        float sampleRate; // AudioServer sample rate
        float *drive;
        float *offset;

        
        // DSP stuff
        
        std::string dspPath = "/home/kad/grame_internship/faust_ex/libfoo.so"; // path to DSP .so file
        
        void *_dsp = NULL; // pointer to DSP shared object
        
        void *dspObject = NULL; // pointer to the handle given by the DSP upon creation
        
        void loadDSP(); // function in charge of dynamically loading DSP
        
        void loadDSPLibrary(); // function in charge of attributing DSP function pointers
        
        void(*init_dsp)(void*, int) = NULL;
        void(*compute)(void*, int, float**, float**) = NULL;
        int(*getNumInputs)(void*) = NULL;
        int(*getNumOutputs)(void*) = NULL;
        void*(*createDSP)() = NULL;
        void(*destroyDSP)(void*)= NULL;
        
        float*(*getDrivePointer)(void*) = NULL;
        float*(*getOffsetPointer)(void*) = NULL;

        void(*setDriveValue)(void*, float) = NULL;
        void(*setOffsetValue)(void*, float) = NULL;

        const float(*getDriveValue)(void*) = NULL;
        const float(*getOffsetValue)(void*) = NULL;
        
        
        float **input = NULL;
        float **output = NULL;
        
        public:
        
        Faust2GodotEffectInstance();
        ~Faust2GodotEffectInstance();
        
        virtual void _process(const AudioFrame *src, AudioFrame *dst, int frame_count); // function called at every cycle
        
        static void _bind_methods();

        float getDrive();
        float getOffset();
        void setDrive(float value);
        void setOffset(float value);
};

class Faust2Godot : public godot::AudioEffect {
    GDCLASS(Faust2Godot, godot::AudioEffect);

    public:
    Ref<AudioEffectInstance> _instantiate() override;
    static void _bind_methods();
    Faust2Godot();
    ~Faust2Godot();
    
    Ref<Faust2GodotEffectInstance> instance = nullptr;
    float getDrive();
    float getOffset();
    void setDrive(float value);
    void setOffset(float value);
};

}

#endif