#ifndef FAUST2GODOT_HPP
#define FAUST2GODOT_HPP

#include <string>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/audio_stream_generator_playback.hpp>
#include <godot_cpp/classes/audio_stream_generator.hpp>
#include <godot_cpp/classes/audio_server.hpp>
#include <godot_cpp/classes/audio_effect_capture.hpp>
#include <godot_cpp/classes/audio_stream_player.hpp>
#include <godot_cpp/variant/packed_vector2_array.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/time.hpp>


namespace godot {

class Faust2Godot : public godot::Node {
    GDCLASS(Faust2Godot, godot::Node);

    private:
        godot::Time *currentTime;

        double startTime = 0;
        double lastFrameTime = 0;

        AudioServer * server = NULL; // pointer to Godot AudioServer singleton

        float sampleRate; // AudioServer sample rate

        // record bus

        int recordBusIndex; // index of the bus using the capture effect

        Ref<AudioEffectCapture> capture; // current instance of the capture effect

        Ref<AudioStream> captureStream; // audio source for the capture bus

        AudioStreamPlayer *capturePlayer = NULL; // pointer to the object playing the audio

        // playback bus

        int playbackBusIndex;

        Ref<AudioStreamGeneratorPlayback> playbackStreamGenerator;

        Ref<AudioStreamGenerator> playbackGenerator; // apparently this is needed for the variable above

        AudioStreamPlayer *playbackPlayer = NULL;

        // DSP stuff

        std::string dspPath = "/home/kad/grame_internship/faust_ex/libbar.so"; // path to DSP .so file

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

        float **input = NULL;
        float **output = NULL;

        // Utility

		PackedVector2Array floatsToVector2Array(float **list, int size);

		void vector2ArrayToFloats(PackedVector2Array array, int size, float **channels);

    public:
        Faust2Godot();
        ~Faust2Godot();

        void initExtension();

        void process(); // function called at every cycle

        static void _bind_methods();
};

}

#endif