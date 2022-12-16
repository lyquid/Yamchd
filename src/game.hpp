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
  void update(double delta_time);

 private:
 
  void checkKeyStates(double delta_time);
  void clean();
  bool initSDL2();
  bool loadResources();
  inline int scalateX(int n) const { return (n * render_size_.x) / window_size_.x; }
  inline int scalateY(int n) const { return (n * render_size_.y) / window_size_.y; }

  inline static const std::string kGameTitle_{"Yamch'd"};
  inline static constexpr SDL_Point render_size_{426, 240};
  
  SDL_Point window_size_{1920, 1080};
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
  GrainTypes selected_grain_ {GrainTypes::Earth};
  bool draw_grain_ {};
  bool rain_ {};
};

} // end namespace ktp

#endif // YAMCHD_SRC_GAME_HPP_