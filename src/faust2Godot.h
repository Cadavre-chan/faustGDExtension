#ifndef FAUST2GODOT_HPP
#define FAUST2GODOT_HPP

#include "extendedMapUI.h"

#include <string>
#include <vector>
#include <memory>
#include <dlfcn.h>

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
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {

struct DLCloser {
    void operator()(void *handle) const {
        if (handle) dlclose(handle);
    }
};
struct DSPCloser {
    void operator()(dsp *dsp_instance) const {
        if (dsp_instance) dsp_instance->~dsp();
    }
};

class Faust2GodotEffectInstance : public godot::AudioEffectInstance {
    GDCLASS(Faust2GodotEffectInstance, godot::AudioEffectInstance);
    
    private:
        int sampleRate;

        /*
        Input and output buffers are kept as raw pointers because it integrates better
        with C++ Faust DSPs
        */
        float **input = nullptr;
        float **output = nullptr;
        
        std::unique_ptr<void, DLCloser> handle;
        std::unique_ptr<dsp, DSPCloser> dsp_instance;
        std::shared_ptr<ExtendedMapUI> mapUI;

    public:
        Faust2GodotEffectInstance();
        ~Faust2GodotEffectInstance();
        void _process(const void *srcptr, AudioFrame *dst, int frame_count); // function called at every cycle
        
        static void _bind_methods();

        void init(godot::String path, int sample_rate);
    
    
        void set_param(godot::String path, float value);
    
    
        float get_param(godot::String path);
    
    
        void compute(const AudioFrame *src, AudioFrame *dst, int frames);

        std::unique_ptr<dsp, DSPCloser> cloneDSP();

        void setDSP(std::unique_ptr<dsp, DSPCloser> DSP);

        std::shared_ptr<ExtendedMapUI> getMapUI() const {return mapUI;}
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


        void loadDSP(godot::String path, int pSampleRate);

        void set_param(godot::String path, float value) {
            instance.ptr()->set_param(path, value);
        };


        float get_param(godot::String path) {
            return instance.ptr()->get_param(path);
        };


        godot::Array get_all_params() {
            return instance.ptr()->get_all_params();
        };

        std::unique_ptr<dsp, DSPCloser> cloneDSP() {
            return this->instance->cloneDSP();
        }

        void setDSP(std::unique_ptr<dsp, DSPCloser> DSP) {
            this->instance->setDSP(std::move(DSP));
        }

        std::shared_ptr<ExtendedMapUI> getMapUI() const {return instance->getMapUI();}
};

};


#endif