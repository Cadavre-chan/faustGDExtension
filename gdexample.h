#ifndef GDEXAMPLE_H
#define GDEXAMPLE_H

#include <string>
#include <memory>
#include <filesystem>
#include <functional>

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/audio_stream_generator_playback.hpp>
#include <godot_cpp/classes/audio_stream_generator.hpp>



namespace godot {

class GDExample : public RefCounted {
	GDCLASS(GDExample, RefCounted);

	private:
		Ref<AudioStreamGeneratorPlayback> playback;
		float phase;
		float sample_rate;
		float frequency;

		std::string library_path = "/home/kad/grame_internship/faust_ex/libfoo.so";
		void *_dsp = NULL;  // Smart pointer for shared library
		void *dspObject = NULL;
		// std::function<void(float)> init_dsp = nullptr;
		// std::function<void(int, float**, float**)> compute = nullptr;
		// std::function<int()> getNumInputs = nullptr;
		// std::function<int()> getNumOutputs = nullptr;
		// std::function<void*()> createDSP = nullptr;
		// std::function<void(void*)> destroyDSP = nullptr;

		void(*init_dsp)(void*, int) = NULL;
		void(*compute)(void*, int, float**, float**) = NULL;
		int(*getNumInputs)(void*) = NULL;
		int(*getNumOutputs)(void*) = NULL;
		void*(*createDSP)() = NULL;
		void(*destroyDSP)(void*)= NULL;
	
		bool compile_faust_code(const std::string& faust_code, const std::string& output_path);
		bool compile_shared_library(const std::string& cpp_path, const std::string& so_path);
		bool load_dsp_library();

		void load_faust_dll();

	public:
		GDExample();
		~GDExample();
	
		void init_ext(Ref<AudioStreamGeneratorPlayback> p_playback, float p_sample_rate, float p_frequency);

		void process();  // Generates audio samples
		
		static void _bind_methods();
};

// class GDExample : public Sprite2D {
// 	GDCLASS(GDExample, Sprite2D)

// private:
// 	double time_passed;
// 	double time_emit;
// 	double amplitude;
// 	double speed;

// protected:
// 	static void _bind_methods();

// public:
// 	GDExample();
// 	~GDExample();

// 	void _process(double delta) override;

// 	void set_amplitude(const double p_amplitude);
// 	double get_amplitude() const;
// 	void set_speed(const double p_speed);
// 	double get_speed() const;
// };

}

#endif
