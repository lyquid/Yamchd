#ifndef YAMCHD_SRC_WORLD_HPP_
#define YAMCHD_SRC_WORLD_HPP_

#include "../sdl2_wrappers/sdl2_wrappers.hpp"
#include "palette.hpp"
#include <vector>

namespace ktp {

inline float generateRand(float min, float max);

enum class GrainTypes {
  Acid, Blood, Earth, Fire, Sand, Void,
  count
};

struct Grain {
  GrainTypes type_ {GrainTypes::Void};
  Uint32 color_ {ColorsARGB8::black};
  // SDL_Point pos_ {};
};

class World {
 public:

  ~World();

  void addGrain(GrainTypes type, const SDL_Point& where);
  void draw();
  void init(const SDL2_Renderer& ren, int rows, int cols);
  void update();
  
 private:

  template<typename T>
  inline Uint32 index(T i, T j) const { return i * cols_ + j; }
  inline Uint32 index(const SDL_Point& pos) const { return pos.y * cols_ + pos.x; }
  inline Uint32 index(const SDL_FPoint& pos) const { return static_cast<Uint32>(pos.y * cols_ + pos.x); }
  void movePixel(Uint32 origin, int x_offset, int y_offset);
  inline void toTheAbyss(Uint32 index);

  //static constexpr Uint32 kDefaultColor_ {ColorsARGB8::black};
  //static constexpr Uint32 kSandColor_ {ColorsARGB8::dark_yellow};

  // final texture to be updated
  SDL2_Texture world_texture_ {};
  // pointer to the array of pixels returned by texture.lock()
  Uint32* world_pixels_ {nullptr};
  // an array of the actual Grain objects
  Grain* world_grains_ {nullptr};
  // It's the number of bytes in a row returned by texture.lock()
  int texture_pitch_ {};
  int rows_ {}, cols_ {};
  // timer used for sand generation
  Uint32 sand_time_ {};
  // which direction is checked first
  bool direction_ {};
};

} // end namespace ktp

#endif // YAMCHD_SRC_WORLD_HPP_