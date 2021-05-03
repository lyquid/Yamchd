#ifndef YAMCHD_SRC_GAME_HPP_
#define YAMCHD_SRC_GAME_HPP_

#include "../kuge/kuge.hpp"
#include "../sdl2_wrappers/sdl2_wrappers.hpp"
#include "world.hpp"
#include <string>

namespace ktp {

void loadConfigFiles();

class Game {
 public:

  Game();
  ~Game() { clean(); }
  void draw();
  void handleSDL2Events();
  void handleSDL2KeyEvents(const SDL_Keycode& key);
  bool init();
  inline bool quit() const { return quit_; }
  void update(float delta_time);

 private:
 
  void checkKeyStates(float delta_time);
  void clean();
  bool initSDL2();
  bool loadResources();

  inline static const std::string kGameTitle_{"Yamch'd"};
  inline static constexpr SDL_Point render_size_{384, 288};
  SDL_Point window_size_{1024, 768};
  bool quit_{false};

  ktp::SDL2_Window main_window_{};
  ktp::SDL2_Renderer renderer_{};
  SDL_Event sdl_event_{};
  ktp::SDL2_Font font_{};
  /* FPS */
  ktp::SDL2_FPS fps_{};
  /* KUGE engine */
  kuge::EventBus event_bus_{};
  kuge::AudioSystem audio_sys_{event_bus_};
  kuge::InputSystem input_sys_{event_bus_};
  kuge::OutputSystem output_sys_{event_bus_};
  /* World */
  World world_ {};
};

} // end namespace ktp

#endif // YAMCHD_SRC_GAME_HPP_