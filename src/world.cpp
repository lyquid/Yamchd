#include "world.hpp"

float ktp::generateRand(float min, float max) {
  const auto random{static_cast<float>(rand()) / static_cast<float>(RAND_MAX)};
	return min + (random * (max - min));
}

ktp::World::~World() {
  delete[] world_grains_;
}

void ktp::World::addGrain(GrainTypes type, const SDL_Point& where) {
  const auto here {index(where)};

  world_grains_[here].type_ = type;

  switch (type) {
    case GrainTypes::Acid:
      world_grains_[here].color_ = ColorsARGB8::green;
      world_pixels_[here] = ColorsARGB8::green;
      break;
    case GrainTypes::Blood:
      world_grains_[here].color_ = ColorsARGB8::red;
      world_pixels_[here] = ColorsARGB8::red;
      break;
    case GrainTypes::Earth:
      world_grains_[here].color_ = ColorsARGB8::dark_grey;
      world_pixels_[here] = ColorsARGB8::dark_grey;
      break;
    case GrainTypes::Fire:
      world_grains_[here].color_ = ColorsARGB8::orange;
      world_pixels_[here] = ColorsARGB8::orange;
      break;
    case GrainTypes::Sand:
      world_grains_[here].color_ = ColorsARGB8::dark_yellow;
      world_pixels_[here] = ColorsARGB8::dark_yellow;
      break;
    case GrainTypes::Void:
      [[fallthrough]];
    default:
      world_grains_[here].color_ = ColorsARGB8::black;
      world_pixels_[here] = ColorsARGB8::black;
      break;
  }
}

void ktp::World::draw() {
  world_texture_.render();
}

void ktp::World::init(const SDL2_Renderer& ren, int rows, int cols) {
  world_texture_.setRenderer(ren);
  world_texture_.create(SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, {cols, rows});
  rows_ = rows;
  cols_ = cols;

  world_grains_ = new Grain[rows * cols];

  world_texture_.lock((void**)&world_pixels_, &texture_pitch_);
  for (auto i = 0; i < rows; ++i) {
    for (auto j = 0; j < cols; ++j) {
      world_grains_[index(i, j)] = {GrainTypes::Void, ColorsARGB8::black};
      world_pixels_[index(i, j)] = ColorsARGB8::black;
    }
  }  
  world_texture_.unlock();

  sand_time_ = SDL2_Timer::getSDL2Ticks();
}

void ktp::World::update() {
  Grain aux_gr {};
  Uint32 current_index {};

  world_texture_.lock((void**)&world_pixels_, &texture_pitch_);

  for (auto i = rows_ - 1; i >= 0; --i) {
    for (auto j = cols_ - 1; j >= 0; --j) {
      current_index = index(i, j);
      switch (world_grains_[current_index].type_) {
        case GrainTypes::Acid:
          break;
        case GrainTypes::Blood:
          break;
        case GrainTypes::Earth:
          break;
        case GrainTypes::Fire:
          break;
        case GrainTypes::Sand:
          // if the grain is just above the abysm it just goes away
          if (i == rows_ - 1) {
            toTheAbyss(current_index);
            break;
          }
          // look down
          if (world_grains_[index(i + 1, j)].type_ == GrainTypes::Void) {
            const auto just_down {index(i + 1, j)};
            aux_gr = world_grains_[current_index];

            world_grains_[current_index] = world_grains_[just_down];
            world_pixels_[current_index] = world_grains_[current_index].color_;

            world_grains_[just_down] = aux_gr;
            world_pixels_[just_down] = world_grains_[just_down].color_;
          }
          break;
        case GrainTypes::Void:
          [[fallthrough]];
        default:
          world_pixels_[current_index] = ColorsARGB8::black;
          break;
      }
    }
  }

  if (SDL2_Timer::getSDL2Ticks() - sand_time_ > 10) {
    addGrain(GrainTypes::Sand, {static_cast<int>(cols_ * generateRand(0, 1)), 0});
    //addGrain({0, 0});
    //addGrain({cols_ / 2, 0});
    //addGrain({cols_ - 1, 0});
    sand_time_ = SDL2_Timer::getSDL2Ticks();
  }
  
  world_texture_.unlock();
}

void ktp::World::movePixel(Uint32 origin, int x_offset, int y_offset) {
  /* world_grains_[origin]->pos_.x += x_offset;
  world_grains_[origin]->pos_.y += y_offset;
  
  const auto new_pos {index(world_grains_[origin]->pos_)};
  world_grains_[new_pos] = world_grains_[origin];
  
  world_pixels_[origin] = kDefaultColor_;
  world_pixels_[new_pos] = world_grains_[origin]->color_;

  world_grains_[origin] = nullptr; */
}

void ktp::World::toTheAbyss(Uint32 index) {
  world_grains_[index] = {GrainTypes::Void, ColorsARGB8::black};
  world_pixels_[index] = ColorsARGB8::black;
}