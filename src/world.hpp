#ifndef YAMCHD_SRC_WORLD_HPP_
#define YAMCHD_SRC_WORLD_HPP_

#include "../sdl2_wrappers/sdl2_wrappers.hpp"
#include "palette.hpp"
#include <vector>

namespace ktp {

inline float generateRand(float min, float max);

struct Grain {
  SDL_Point pos_ {};
  Uint32 color_ {ColorsARGB8::dark_blue};
};

class World {
 public:

  ~World();

  void addGrain(const SDL_Point& where);
  void draw();
  void init(const SDL2_Renderer& ren, int rows, int cols);
  void update();
  
 private:

  template<typename T>
  inline Uint32 index(T i, T j) const { return i * cols_ + j; }
  inline Uint32 index(const SDL_Point& pos) const { return pos.y * cols_ + pos.x; }
  inline Uint32 index(const SDL_FPoint& pos) const { return pos.y * cols_ + pos.x; }
  void movePixel(Uint32 origin, int x_offset, int y_offset);

  static constexpr Uint32 kDefaultColor_ {ColorsARGB8::dark_blue};
  static constexpr Uint32 kSandColor_ {ColorsARGB8::dark_yellow};

  // final texture to be updated
  SDL2_Texture world_texture_ {};
  // an array of pixels in ARGB8 format to be updated to the wolrd_texture_
  Uint32* world_pixels_ {nullptr};
  // an array of addresses to the actual Grain objects
  Grain** world_grains_ {nullptr};
  // this is used to update the world_texture_. It's the number of bytes in a row
  unsigned int world_pitch_ {};
  int rows_ {}, cols_ {};
  // the actual grain information is stored here
  std::vector<Grain> grains_ {};
  // those will be used for optimization
  unsigned int first_grain_ {}, last_grain_ {};
  // timer used for sand generation
  Uint32 sand_time_ {};
  // which direction is checked first
  bool direction_ {};
};

} // end namespace ktp

#endif // YAMCHD_SRC_WORLD_HPP_