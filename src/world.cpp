#include "world.hpp"

float ktp::generateRand(float min, float max) {
  const auto random{static_cast<float>(rand()) / static_cast<float>(RAND_MAX)};
	return min + (random * (max - min));
}

ktp::World::~World() {
  delete[] world_pixels_;
  delete[] world_grains_;
}

void ktp::World::addGrain(const SDL_Point& where) {
  const Grain gr {{where.x, where.y}, kSandColor_};
  const auto here {index(gr.pos_)};
  grains_.push_back(gr);
  world_grains_[here] = &grains_.back();
  world_pixels_[here] = grains_.back().color_;

  if (here < first_grain_) {
    first_grain_ = here;
  } else if (here > last_grain_) {
    last_grain_ = here;
  }
}

void ktp::World::draw() {
  world_texture_.render();
}

void ktp::World::init(const SDL2_Renderer& ren, int rows, int cols) {

  world_texture_.setRenderer(ren);
  world_texture_.create(SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, {cols, rows});
  world_pitch_ = cols * sizeof(Uint32);
  rows_ = rows;
  cols_ = cols;
  first_grain_ = rows * cols;

  world_pixels_ = new Uint32[rows * cols];
  world_grains_ = new Grain*[rows * cols];

  for (auto i = 0; i < rows; ++i) {
    for (auto j = 0; j < cols; ++j) {
      world_pixels_[index(i, j)] = kDefaultColor_;
      world_grains_[index(i, j)] = nullptr;
    }
  }

  grains_.reserve(rows * cols);

  sand_time_ = SDL2_Timer::getSDL2Ticks();
}

void ktp::World::update() {
  if (SDL2_Timer::getSDL2Ticks() - sand_time_ > 10) {
    addGrain({static_cast<int>(cols_ * generateRand(0, 1)), 0});
    //addGrain({0, 0});
    //addGrain({cols_ / 2, 0});
    //addGrain({cols_ - 1, 0});
    sand_time_ = SDL2_Timer::getSDL2Ticks();
  }

  for (auto i = rows_ - 2; i >= 0; --i) {
    for (auto j = cols_ - 1; j >= 0; --j) {
      if (world_grains_[index(i, j)] != nullptr) {
        // look down
        if (world_grains_[index(i + 1, j)] == nullptr) {
          movePixel(index(i, j), 0, 1);
        } else if (direction_) {
          // look down right
          if (j != cols_ -1 && world_grains_[index(i + 1, j + 1)] == nullptr) {
            movePixel(index(i, j), 1, 1);
          // look down left
          } else if (j != 0 && world_grains_[index(i + 1, j - 1)] == nullptr) {
            movePixel(index(i, j), -1, 1);
          }
          direction_ = !direction_;
        } else {
          // look down left
          if (j != 0 && world_grains_[index(i + 1, j - 1)] == nullptr) {
            movePixel(index(i, j), -1, 1);
          // look down right
          } else if (j != cols_ -1 && world_grains_[index(i + 1, j + 1)] == nullptr) {
            movePixel(index(i, j), 1, 1);
          }
          direction_ = !direction_;
        }
      }
    }
  }
  world_texture_.update(world_pixels_, world_pitch_);
}

void ktp::World::movePixel(Uint32 origin, int x_offset, int y_offset) {
  world_grains_[origin]->pos_.x += x_offset;
  world_grains_[origin]->pos_.y += y_offset;
  
  const auto new_pos {index(world_grains_[origin]->pos_)};
  world_grains_[new_pos] = world_grains_[origin];
  
  world_pixels_[origin] = kDefaultColor_;
  world_pixels_[new_pos] = world_grains_[origin]->color_;

  world_grains_[origin] = nullptr;
}