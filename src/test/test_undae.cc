#include <memory>

#include <SDL2/SDL.h>

#include "tests.hh"
#include "meadow/undae.hh"

using namespace meadow;

struct UndaeTester {
	UndaeTester() {
		
	}
private:
	
};

void test_undae() {
	
	std::unique_ptr<undae::Generator> root_gen;
	
	auto sine = std::make_unique<undae::osc::Sine>(80.0 / 44100);
	root_gen = std::make_unique<undae::Volume>(0.25, std::move(sine));
	
	SDL_AudioSpec spec_want {}, spec {};
	spec_want.channels = 2;
	spec_want.format = AUDIO_F32;
	spec_want.freq = 44100;
	spec_want.samples = 8192;
	spec_want.userdata = root_gen.get();
	spec_want.callback = *[](void * udat, Uint8 * data, int size){
		size_t len = size / sizeof(float);
		undae::Buffer buf(len);
		buf.resize(len);
		reinterpret_cast<undae::Generator *>(udat)->generate(buf);
		for (size_t i = 0; i < len; i++)
			reinterpret_cast<float *>(data)[i] = buf[i];
	};
	
	SDL_Init(SDL_INIT_AUDIO);
	int adev = SDL_OpenAudioDevice(nullptr, 0, &spec_want, &spec, 0);
	if (!adev) {
		tlog << "Could not open audio device, cannot test!";
		exit(1);
	}
	
	SDL_PauseAudioDevice(adev, 0);
	SDL_Delay(-1);
	SDL_PauseAudioDevice(adev, 1);
	SDL_Quit();
}
