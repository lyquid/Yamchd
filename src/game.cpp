#include "game.hpp"
#include "palette.hpp"
#include "../include/resources_path.hpp"

void ktp::loadConfigFiles() {}

/* GAME */

ktp::Game::Game() {
  event_bus_.setSystems(audio_sys_, input_sys_, output_sys_);
}

void ktp::Game::checkKeyStates(float delta_time) {
  const Uint8* state = SDL_GetKeyboardState(nullptr);
  /* if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP]) {
    player_.thrust(delta_time);
  } else {
    player_.stopThrusting();
  }
  if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT]) {
    player_.steerLeft(delta_time);
  }
  if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT]) {
    player_.steerRight(delta_time);
  }
  if (state[SDL_SCANCODE_SPACE]) {
    player_.shoot();
  } */
}

void ktp::Game::clean() {
  ktp::SDL2_Audio::closeMixer();
  ktp::SDL2_Font::closeTTF();
  ktp::SDL2_Image::closeImage();
	SDL_Quit();
}

void ktp::Game::draw() {
  renderer_.clear();
  
  world_.draw();

  renderer_.present();
  ++fps_;
}

void ktp::Game::handleSDL2Events() {
  while (SDL_PollEvent(&sdl_event_)) {
    switch (sdl_event_.type) {
      case SDL_QUIT:
        input_sys_.postEvent(kuge::EventTypes::ExitGame);
        quit_ = true;
        break;
      case SDL_KEYDOWN:
        input_sys_.postEvent(kuge::EventTypes::KeyPressed, SDL_GetKeyName(sdl_event_.key.keysym.sym));
        handleSDL2KeyEvents(sdl_event_.key.keysym.sym);
        break;
      case SDL_MOUSEBUTTONDOWN: {
        int x{0}, y{0};
        if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT)) {

        }
        break;
      }
      default:
        break;
    }
  }
}

void ktp::Game::handleSDL2KeyEvents(const SDL_Keycode& key) {
  switch (key) {
    case SDLK_ESCAPE:
      input_sys_.postEvent(kuge::EventTypes::ExitGame);
      quit_ = true;
      break;
    default:
      break;
  }
}

bool ktp::Game::init() {
  if (!initSDL2()) return false;
  if (!main_window_.create(kGameTitle_, window_size_)) return false;
  if (!renderer_.create(main_window_, render_size_, SDL_RENDERER_ACCELERATED)) return false;
  if (!loadResources()) return false;

  world_.init(renderer_, render_size_.y, render_size_.x);

  fps_.start();

  return true;
}

bool ktp::Game::initSDL2() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    ktp::logSDL2Error("SDL_Init");
    return false;
  }
  if (!ktp::SDL2_Image::initImage()) return false;
  if (!ktp::SDL2_Font::initTTF()) return false;
  if (!ktp::SDL2_Audio::initMixer()) return false;

  return true;
}

bool ktp::Game::loadResources() {
  if (!font_.loadFont(getResourcesPath("fonts") + "Future n0t Found.ttf", 18)) {
    return false;
  }
  /* if (!audio_sys_.loadResources()) {
    return false;
  } */
  return true;
}

void ktp::Game::update(float delta_time) {
  /* FPS */
  main_window_.setTitle(kGameTitle_ + " - FPS: " + std::to_string(fps_.average()));
  /* Event bus */
  event_bus_.processEvents();
  /* World */
  world_.update();
}