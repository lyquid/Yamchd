#ifndef YAMCHD_SRC_WORLD_HPP_
#define YAMCHD_SRC_WORLD_HPP_

#include "../sdl2_wrappers/sdl2_wrappers.hpp"
#include "palette.hpp"
#include <vector>

namespace ktp {

inline float generateRand(float min, float max);

enum class GrainTypes {
  Acid, Blood, Earth, Fire, Fume, Rock, Sand, Snow, Steel, Void, Water,
  count
};

struct Grain {
  GrainTypes type_ {GrainTypes::Void};
  Uint32 color_ {ColorsARGB8::black};
  bool direction_ {};
  bool ignore_ {};
  int life_ {};
};

class World {
 public:

  ~World() { delete[] world_grains_; }

  void draw();
  void drawRectangle(const SDL_Rect& rect, GrainTypes type);
  void init(const SDL2_Renderer& ren, int rows, int cols);
  void rain(GrainTypes type);
  void update();
  void updateOnlyTheRectangle();

 private:

  void addGrain(GrainTypes type, Uint32 where);
  inline void addGrain(GrainTypes type, const SDL_Point& where) { addGrain(type, getIndex(where)); }
  void checkAutomatons(Uint32 index, int i, int j);
  void checkMinimunRectangle();
  void generateWorld();
  template<typename T>
  inline Uint32 getIndex(T i, T j) const { return i * cols_ + j; }
  inline Uint32 getIndex(const SDL_Point& pos) const { return pos.y * cols_ + pos.x; }
  inline Uint32 getIndex(const SDL_FPoint& pos) const { return static_cast<Uint32>(pos.y * cols_ + pos.x); }
  inline SDL_Point getPosition(Uint32 index) { return SDL_Point{static_cast<int>(index % cols_), static_cast<int>(index / cols_)}; }
  inline void handleAcid(Uint32 index, int i, int j, Grain& aux_gr);
  inline void handleFume(Uint32 index, int i, int j, Grain& aux_gr);
  inline void handleSand(Uint32 index, int i, int j, Grain& aux_gr);
  inline void handleWater(Uint32 index, int i, int j, Grain& aux_gr);
  void swapPixels(Uint32 origin, Uint32 destination, Grain& aux);
  inline void toTheAbyss(Uint32 index);

  // used to draw the rectangles for the updated sections
  const SDL2_Renderer* renderer_;
  // final texture to be updated
  SDL2_Texture world_texture_ {};
  // pointer to the array of pixels returned by texture.lock()
  Uint32* world_pixels_ {nullptr};
  // an array of the actual Grain objects
  Grain* world_grains_ {nullptr};
  // It's the number of bytes in a row returned by texture.lock()
  int texture_pitch_ {};
  unsigned int rows_ {}, cols_ {};
  // timer used for rain
  Uint32 rain_time_ {};
  // which direction is checked first
  bool direction_flag_ {};
  // where to start updating
  bool from_left_ {};
  // stuff for the "precision" update
  std::vector<SDL_Point> coords_ {};
  SDL_Rect bounding_box_ {};
};

} // end namespace ktp

#endif // YAMCHD_SRC_WORLD_HPP_