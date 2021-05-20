#include "world.hpp"
#include <vector>

float ktp::generateRand(float min, float max) {
  const auto random{static_cast<float>(rand()) / static_cast<float>(RAND_MAX)};
	return min + (random * (max - min));
}

ktp::World::~World() {
  delete[] world_grains_;
}

void ktp::World::addGrain(GrainTypes type, Uint32 where) {
  world_grains_[where].type_ = type;
  world_grains_[where].direction_ = direction_flag_;
  
  switch (type) {
    case GrainTypes::Acid:
      world_grains_[where].life_ = 1;
      world_grains_[where].color_ = ColorsARGB8::green;
      world_pixels_[where] = ColorsARGB8::green;
      break;
    case GrainTypes::Blood:
      world_grains_[where].life_ = 1;
      world_grains_[where].color_ = ColorsARGB8::red;
      world_pixels_[where] = ColorsARGB8::red;
      break;
    case GrainTypes::Earth:
      world_grains_[where].life_ = 1000;
      world_grains_[where].color_ = ColorsARGB8::orange;
      world_pixels_[where] = ColorsARGB8::orange;
      break;
    case GrainTypes::Fire:
      world_grains_[where].color_ = ColorsARGB8::red;
      world_pixels_[where] = ColorsARGB8::red;
      break;
    case GrainTypes::Fume: 
      world_grains_[where].life_ = 1;
      world_grains_[where].color_ = 0x0341de95;
      world_pixels_[where] = 0x0341de95;
      break;
    case GrainTypes::Rock:
      world_grains_[where].color_ = ColorsARGB8::dark_grey;
      world_pixels_[where] = ColorsARGB8::dark_grey;
      break;
    case GrainTypes::Sand:
      world_grains_[where].life_ = 100;
      world_grains_[where].color_ = ColorsARGB8::dark_yellow;
      world_pixels_[where] = ColorsARGB8::dark_yellow;
      break;
    case GrainTypes::Snow:
      world_grains_[where].color_ = ColorsARGB8::white;
      world_pixels_[where] = ColorsARGB8::white;
      break;
    case GrainTypes::Steel:
      world_grains_[where].life_ = 65000;
      world_grains_[where].color_ = ColorsARGB8::grey;
      world_pixels_[where] = ColorsARGB8::grey;
      break;
    case GrainTypes::Water:
      world_grains_[where].life_ = 1;
      world_grains_[where].color_ = ColorsARGB8::blue;
      world_pixels_[where] = ColorsARGB8::blue;
      break;
    case GrainTypes::Void:
      [[fallthrough]];
    default:
      world_grains_[where].color_ = ColorsARGB8::black;
      world_pixels_[where] = ColorsARGB8::black;
      break;
  } 
}

void ktp::World::checkAutomatons(Uint32 index, int i, int j) {
  Grain aux_gr {};
  switch (world_grains_[index].type_) {
    case GrainTypes::Acid:
      if (i == rows_ - 1) {
        toTheAbyss(index);
        break;
      } else if (!world_grains_[index].ignore_) {
        handleAcid(index, i, j, aux_gr);
        break;
      }
      break;
    case GrainTypes::Blood:
      if (i == rows_ - 1) {
        toTheAbyss(index);
        break;
      } else if (!world_grains_[index].ignore_) {
        handleWater(index, i, j, aux_gr);
        break;
      }
      break;
    case GrainTypes::Earth:
      world_pixels_[index] = world_grains_[index].color_;
      break;
    case GrainTypes::Fire:
      if (i == rows_ - 1) {
        toTheAbyss(index);
        break;
      }
      break;
    case GrainTypes::Fume:
      if (i == 0) {
        toTheAbyss(index);
        break;
      } else if (!world_grains_[index].ignore_) {
        handleFume(index, i, j, aux_gr);
        break;
      }
      break;
    case GrainTypes::Sand:
      if (i == rows_ - 1) {
        toTheAbyss(index);
        break;
      } else if (!world_grains_[index].ignore_) {
        handleSand(index, i, j, aux_gr);
        break;
      }
      break;
    case GrainTypes::Steel:
      world_pixels_[index] = world_grains_[index].color_;
      break;
    case GrainTypes::Water:
      if (i == rows_ - 1) {
        toTheAbyss(index);
        break;
      } else if (!world_grains_[index].ignore_) {
        handleWater(index, i, j, aux_gr);
        break;
      }
      break;
    case GrainTypes::Void:
      [[fallthrough]];
    default:
      world_pixels_[index] = ColorsARGB8::black;
      break;
  }
}

void ktp::World::draw() {
  world_texture_.render();
}

void ktp::World::drawRectangle(const SDL_Rect& rect, GrainTypes type) {
  for (auto i = rect.y; i < rect.y + rect.h && i < rows_; ++i) {
    for (auto j = rect.x; j < rect.x + rect.w && j < cols_; ++j) {
      addGrain(type, {j, i});
    }
  }
}

void ktp::World::generateWorld() {
  constexpr auto chunks {4000u};
  constexpr SDL_Point chunk_size {5, 5};
  constexpr auto high_threshold {0.66f};
  
  std::vector<unsigned int> positions {};
  positions.resize(chunks);

  for (auto& position: positions) {
    position = static_cast<unsigned int>(((rows_ * cols_) - 1) * generateRand(high_threshold, 1.f));
    const auto point {getPosition(position)};
    drawRectangle({point.x, point.y, chunk_size.x, chunk_size.y}, GrainTypes::Earth);
  }
  // drawRectangle({165, 121, 50, 2}, GrainTypes::Earth); // middle bar
  // drawRectangle({0, static_cast<int>(rows_) - 6, static_cast<int>(cols_), 1}, GrainTypes::Steel);
  drawRectangle({0, static_cast<int>(rows_) - 1, static_cast<int>(cols_), 1}, GrainTypes::Steel);
}

void ktp::World::handleAcid(Uint32 index, int i, int j, Grain& aux_gr) {
  auto acid_count {1};
  auto water_count {0};
  for (auto k = i - 1; k <= i + 1; ++k) {
    for (auto l = j - 1; l <= j + 1; ++l) {
      if (k >=0 && k < rows_ && l >= 0 && l < cols_ && (i != k || j != l)) {
        const auto neighbor {getIndex(k, l)};
        if (world_grains_[neighbor].type_ != GrainTypes::Void 
         && world_grains_[neighbor].type_ != GrainTypes::Acid
         && world_grains_[neighbor].type_ != GrainTypes::Fume
         && world_grains_[neighbor].type_ != GrainTypes::Water) {
          --world_grains_[neighbor].life_;
          // if (world_grains_[neighbor].life_ < 0) addGrain(GrainTypes::Fume, neighbor);
        }
        if (world_grains_[neighbor].type_ == GrainTypes::Acid) {
          ++acid_count;
        } else if (world_grains_[neighbor].type_ == GrainTypes::Water) {
          ++water_count;
        }
      }
    }
  }
  
  if (water_count > acid_count) {
    addGrain(GrainTypes::Water, index);
  } else {
    // look down for a hole
    if (world_grains_[getIndex(i + 1, j)].type_ == GrainTypes::Void) {
      world_grains_[index].direction_ = !world_grains_[index].direction_;
      swapPixels(index, getIndex(i + 1, j), aux_gr);
    // look down right
    } else if (j < cols_ - 1 && world_grains_[getIndex(i + 1, j + 1)].type_ == GrainTypes::Void) {
      swapPixels(index, getIndex(i + 1, j + 1), aux_gr);
    // look down left
    } else if (j != 0 && world_grains_[getIndex(i + 1, j - 1)].type_ == GrainTypes::Void) {
      swapPixels(index, getIndex(i + 1, j - 1), aux_gr);
    }

    if (world_grains_[index].direction_) {
      // look right
      if (j < cols_ - 1 && world_grains_[getIndex(i, j + 1)].type_ == GrainTypes::Void) {
        swapPixels(index, getIndex(i, j + 1), aux_gr);
      // look left
      } else if (j != 0 && world_grains_[getIndex(i, j - 1)].type_ == GrainTypes::Void) {
        world_grains_[index].direction_ = !world_grains_[index].direction_;
        swapPixels(index, getIndex(i, j - 1), aux_gr);
      }
    } else {
      // look left
      if (j != 0 && world_grains_[getIndex(i, j - 1)].type_ == GrainTypes::Void) {
        swapPixels(index, getIndex(i, j - 1), aux_gr);
      // look right
      } else if (j < cols_ - 1 && world_grains_[getIndex(i, j + 1)].type_ == GrainTypes::Void) {
        world_grains_[index].direction_ = !world_grains_[index].direction_;
        swapPixels(index, getIndex(i, j + 1), aux_gr);
      }
    }
  }
  direction_flag_ = !direction_flag_;
}

// WIP - NOT WORKING PROPERLY
void ktp::World::handleFume(Uint32 index, int i, int j, Grain& aux_gr) {
  // look up
  Uint32 aux {getIndex(i - 1, j)};
  if (world_grains_[aux].type_ == GrainTypes::Acid
   || world_grains_[aux].type_ == GrainTypes::Blood
   || world_grains_[aux].type_ == GrainTypes::Sand
   || world_grains_[aux].type_ == GrainTypes::Water
   || world_grains_[aux].type_ == GrainTypes::Void) {
    world_grains_[index].direction_ = !world_grains_[index].direction_;
    swapPixels(index, aux, aux_gr);
    //swapPixels(aux, index, aux_gr);
  // look up right
  aux = getIndex(i - 1, j + 1);
  } else if (j < cols_ - 1 
   && (world_grains_[aux].type_ == GrainTypes::Acid
   || world_grains_[aux].type_ == GrainTypes::Blood
   || world_grains_[aux].type_ == GrainTypes::Sand
   || world_grains_[aux].type_ == GrainTypes::Water
   || world_grains_[aux].type_ == GrainTypes::Void)) {
    swapPixels(index, aux, aux_gr);
    //swapPixels(aux, index, aux_gr);
  // look up left
  aux = getIndex(i - 1, j - 1);
  } else if (j != 0 
   && (world_grains_[aux].type_ == GrainTypes::Acid
   || world_grains_[aux].type_ == GrainTypes::Blood
   || world_grains_[aux].type_ == GrainTypes::Sand
   || world_grains_[aux].type_ == GrainTypes::Water
   || world_grains_[aux].type_ == GrainTypes::Void)) {
    swapPixels(index, aux, aux_gr);
    //swapPixels(aux, index, aux_gr);
  }

  /* if (world_grains_[index].direction_) {
    // look right
    aux = getIndex(i, j + 1);
    if (j < cols_ - 1 
     && (world_grains_[aux].type_ == GrainTypes::Acid
     || world_grains_[aux].type_ == GrainTypes::Blood
     || world_grains_[aux].type_ == GrainTypes::Sand
     || world_grains_[aux].type_ == GrainTypes::Water
     || world_grains_[aux].type_ == GrainTypes::Void)) {
      swapPixels(index, aux, aux_gr);
    // look left
    aux = getIndex(i, j - 1);
    } else if (j != 0 
     && (world_grains_[aux].type_ == GrainTypes::Acid
     || world_grains_[aux].type_ == GrainTypes::Blood
     || world_grains_[aux].type_ == GrainTypes::Sand
     || world_grains_[aux].type_ == GrainTypes::Water
     || world_grains_[aux].type_ == GrainTypes::Void)) {
      world_grains_[index].direction_ = !world_grains_[index].direction_;
      swapPixels(index, aux, aux_gr);
    }
  } else {
    // look left
    aux = getIndex(i, j - 1);
    if (j != 0 
     && (world_grains_[aux].type_ == GrainTypes::Acid
     || world_grains_[aux].type_ == GrainTypes::Blood
     || world_grains_[aux].type_ == GrainTypes::Sand
     || world_grains_[aux].type_ == GrainTypes::Water
     || world_grains_[aux].type_ == GrainTypes::Void)) {
      swapPixels(index, aux, aux_gr);
    // look right
    aux = getIndex(i, j + 1);
    } else if (j < cols_ - 1 
     && (world_grains_[aux].type_ == GrainTypes::Acid
     || world_grains_[aux].type_ == GrainTypes::Blood
     || world_grains_[aux].type_ == GrainTypes::Sand
     || world_grains_[aux].type_ == GrainTypes::Water
     || world_grains_[aux].type_ == GrainTypes::Void)) {
      world_grains_[index].direction_ = !world_grains_[index].direction_;
      swapPixels(index, aux, aux_gr);
    }
  }*/
}

void ktp::World::handleSand(Uint32 index, int i, int j, Grain& aux_gr) {
  Uint32 aux {getIndex(i + 1, j)};
  // look down
  if (world_grains_[aux].type_ == GrainTypes::Void 
   || world_grains_[aux].type_ == GrainTypes::Acid
   || world_grains_[aux].type_ == GrainTypes::Blood
   || world_grains_[aux].type_ == GrainTypes::Water) {
    swapPixels(index, aux, aux_gr);
  } else if (direction_flag_) {
    // look down right
    aux = getIndex(i + 1, j + 1);
    if (j < cols_ - 1
     && (world_grains_[aux].type_ == GrainTypes::Void 
     || world_grains_[aux].type_ == GrainTypes::Acid
     || world_grains_[aux].type_ == GrainTypes::Blood
     || world_grains_[aux].type_ == GrainTypes::Water)) {
      swapPixels(index, aux, aux_gr);
    // look down left
    aux = getIndex(i + 1, j - 1);
    } else if (j != 0 
     && (world_grains_[aux].type_ == GrainTypes::Void 
     || world_grains_[aux].type_ == GrainTypes::Acid
     || world_grains_[aux].type_ == GrainTypes::Blood
     || world_grains_[aux].type_ == GrainTypes::Water)) {
      swapPixels(index, aux, aux_gr);
    }
  } else {
    // look down left
    aux = getIndex(i + 1, j - 1);
    if (j != 0 
     && (world_grains_[aux].type_ == GrainTypes::Void 
     || world_grains_[aux].type_ == GrainTypes::Acid
     || world_grains_[aux].type_ == GrainTypes::Blood
     || world_grains_[aux].type_ == GrainTypes::Water)) {
      swapPixels(index, aux, aux_gr);
    // look down right
    aux = getIndex(i + 1, j + 1);
    } else if (j < cols_ - 1 
     && (world_grains_[aux].type_ == GrainTypes::Void 
     || world_grains_[aux].type_ == GrainTypes::Acid
     || world_grains_[aux].type_ == GrainTypes::Blood
     || world_grains_[aux].type_ == GrainTypes::Water)) {
      swapPixels(index, aux, aux_gr);
    }
  }
  direction_flag_ = !direction_flag_;
}

void ktp::World::handleWater(Uint32 index, int i, int j, Grain& aux_gr) {
  auto acid_count {0};
  auto water_count {1};
  for (auto k = i - 1; k <= i + 1; ++k) {
    for (auto l = j - 1; l <= j + 1; ++l) {
      if (k >=0 && k < rows_ && l >= 0 && l < cols_ && (i != k || j != l)) {
        const auto neighbor {getIndex(k, l)};
        if (world_grains_[neighbor].type_ == GrainTypes::Acid) {
          ++acid_count;
        } else if (world_grains_[neighbor].type_ == GrainTypes::Water) {
          ++water_count;
        }
      }
    }
  }
  if (acid_count > water_count) {
    addGrain(GrainTypes::Acid, index);
  } else {
    // look down
    if (world_grains_[getIndex(i + 1, j)].type_ == GrainTypes::Void) {
      world_grains_[index].direction_ = !world_grains_[index].direction_;
      swapPixels(index, getIndex(i + 1, j), aux_gr);
    // look down right
    } else if (j < cols_ - 1 && world_grains_[getIndex(i + 1, j + 1)].type_ == GrainTypes::Void) {
      swapPixels(index, getIndex(i + 1, j + 1), aux_gr);
    // look down left
    } else if (j != 0 && world_grains_[getIndex(i + 1, j - 1)].type_ == GrainTypes::Void) {
      swapPixels(index, getIndex(i + 1, j - 1), aux_gr);
    }

    if (world_grains_[index].direction_) {
      // look right
      if (j < cols_ - 1 && world_grains_[getIndex(i, j + 1)].type_ == GrainTypes::Void) {
        swapPixels(index, getIndex(i, j + 1), aux_gr);
      // look left
      } else if (j != 0 && world_grains_[getIndex(i, j - 1)].type_ == GrainTypes::Void) {
        world_grains_[index].direction_ = !world_grains_[index].direction_;
        swapPixels(index, getIndex(i, j - 1), aux_gr);
      }
    } else {
      // look left
      if (j != 0 && world_grains_[getIndex(i, j - 1)].type_ == GrainTypes::Void) {
        swapPixels(index, getIndex(i, j - 1), aux_gr);
      // look right
      } else if (j < cols_ - 1 && world_grains_[getIndex(i, j + 1)].type_ == GrainTypes::Void) {
        world_grains_[index].direction_ = !world_grains_[index].direction_;
        swapPixels(index, getIndex(i, j + 1), aux_gr);
      }
    }
  }
  direction_flag_ = !direction_flag_;
}

void ktp::World::init(const SDL2_Renderer& ren, int rows, int cols) {
  world_texture_.setRenderer(ren);
  world_texture_.create(SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, {(int)cols, (int)rows});
  rows_ = rows;
  cols_ = cols;

  world_grains_ = new Grain[rows * cols];

  world_texture_.lock((void**)&world_pixels_, &texture_pitch_);
  for (auto i = 0; i < rows; ++i) {
    for (auto j = 0; j < cols; ++j) {
      addGrain(GrainTypes::Void, {j, i});
    }
  }
  generateWorld();
  world_texture_.unlock();

  rain_time_ = SDL2_Timer::getSDL2Ticks();
}

void ktp::World::rain(GrainTypes type) {
  if (SDL2_Timer::getSDL2Ticks() - rain_time_ > 10) {
    addGrain(type, {static_cast<int>(cols_ * generateRand(0.f, 1.f)), 0});
    rain_time_ = SDL2_Timer::getSDL2Ticks();
  } 
}

void ktp::World::swapPixels(Uint32 origin, Uint32 destination, Grain& aux) {
  aux = world_grains_[origin];

  world_grains_[origin] = world_grains_[destination];
  world_pixels_[origin] = world_grains_[destination].color_;

  world_grains_[destination] = aux;
  world_grains_[destination].ignore_ = true;
  world_pixels_[destination] = aux.color_;
}

void ktp::World::toTheAbyss(Uint32 index) {
  world_grains_[index] = {GrainTypes::Void, ColorsARGB8::black};
  world_pixels_[index] = ColorsARGB8::black;
}

void ktp::World::update() {

  for (auto i = 0u; i < rows_ * cols_; ++i) world_grains_[i].ignore_ = false;

  world_texture_.lock((void**)&world_pixels_, &texture_pitch_);

  if (from_left_) {
    for (int i = rows_ - 1; i >= 0; --i) {
      for (int j = 0; j < cols_; ++j) {
        const auto current_index {getIndex(i, j)};
        if (world_grains_[current_index].life_ <= 0) {
          toTheAbyss(current_index);
        } else {
          checkAutomatons(current_index, i, j);
        }
      }
    }
  } else {
    for (int i = rows_ - 1; i >= 0; --i) {
      for (int j = cols_ - 1; j >= 0; --j) {
        const auto current_index {getIndex(i, j)};
        if (world_grains_[current_index].life_ <= 0) {
          toTheAbyss(current_index);
        } else {
          checkAutomatons(current_index, i, j);
        }
      }
    }         
  }
  from_left_ = !from_left_;

  world_texture_.unlock();
}