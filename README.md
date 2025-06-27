# jukebox

a simple library for loading and playing music files

supported formats: WAV, MP3, FLAC, XM

# usage

Below is a simple example. A `juke::Jukebox` must be instantiated with a `capo::IEngine`, which is included as a dependency. Load files through a `std::filesystem::path`, and simply call `play()` to start infinite playback.

```cpp
#include <juke/juke.hpp>

int main() {

  std::unique_ptr<capo::IEngine> engine{capo::create_engine()};
  auto jukebox = juke::Jukebox{engine};
  if(!jukebox.load_media(std::filesystem::path{"honkytonk.xm"})) { std::println("Failed to load media\n"); }
  jukebox.play();

  return 0;
};
```

Additional functions:

	bool load_media(std::filesystem::path const& path)
Loads audio file.
Returns true if file loaded successfully, false otherwise.

	void play(bool looping = true)
Starts audio playback. Loops indefinitely by default.

	void pause()
 Pauses audio. Audio stream for XM files cannot be restarted or scrubbed.

	void stop()
Stops audio and set cursor to beginning. This does not work for XM format.

Capo wrapper functions:

```
// setters
void set_gain(float gain)
void set_pitch(float pitch)
void set_pan(float pan)
void set_fade_in(std::chrono::duration<float> duration, float gain)
void set_fade_out(std::chrono::duration<float> duration)
void set_position(capo::Vec3f pos)
void set_spatialized(bool spatialized)

//getters
float get_gain()
float get_pitch()
float get_pan()
capo::Vec3f get_position()
bool is_spatialized()
std::string get_filename()

// state
bool has_file()
bool is_playing()
bool is_stopped()
```
