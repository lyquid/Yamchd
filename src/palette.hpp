#ifndef YAMCHD_SRC_PALETTE_HPP_
#define YAMCHD_SRC_PALETTE_HPP_

#include <SDL.h>
#include <vector>

namespace ktp {

namespace Colors {

  /* https://lospec.com/palette-list/slso-clr17 */

  inline constexpr SDL_Color black        {0x2e, 0x2c, 0x3b, 0xFF};
  inline constexpr SDL_Color dark_blue    {0x3e, 0x41, 0x5f, 0xFF};
  inline constexpr SDL_Color dark_grey    {0x55, 0x60, 0x7d, 0xFF};
  inline constexpr SDL_Color grey         {0x74, 0x7d, 0x88, 0xFF};
  inline constexpr SDL_Color copper_green {0x41, 0xde, 0x95, 0xFF};
  inline constexpr SDL_Color turquoise    {0x2a, 0xa4, 0xaa, 0xFF};
  inline constexpr SDL_Color blue         {0x3b, 0x77, 0xa6, 0xFF};
  inline constexpr SDL_Color dark_green   {0x24, 0x93, 0x37, 0xFF};
  inline constexpr SDL_Color green        {0x56, 0xbe, 0x44, 0xFF};
  inline constexpr SDL_Color yellow       {0xc6, 0xde, 0x78, 0xFF};
  inline constexpr SDL_Color dark_yellow  {0xf3, 0xc2, 0x20, 0xFF};
  inline constexpr SDL_Color orange       {0xc4, 0x65, 0x1c, 0xFF};
  inline constexpr SDL_Color red          {0xb5, 0x41, 0x31, 0xFF};
  inline constexpr SDL_Color violet       {0x61, 0x40, 0x7a, 0xFF};
  inline constexpr SDL_Color purple       {0x8f, 0x3d, 0xa7, 0xFF};
  inline constexpr SDL_Color pink         {0xea, 0x61, 0x9d, 0xFF};
  inline constexpr SDL_Color white        {0xc1, 0xe5, 0xea, 0xFF};

  inline const std::vector<SDL_Color> palette {
    {0x2e, 0x2c, 0x3b, 0xFF},
    {0x3e, 0x41, 0x5f, 0xFF},
    {0x55, 0x60, 0x7d, 0xFF},
    {0x74, 0x7d, 0x88, 0xFF},
    {0x41, 0xde, 0x95, 0xFF},
    {0x2a, 0xa4, 0xaa, 0xFF},
    {0x3b, 0x77, 0xa6, 0xFF},
    {0x24, 0x93, 0x37, 0xFF},
    {0x56, 0xbe, 0x44, 0xFF},
    {0xc6, 0xde, 0x78, 0xFF},
    {0xf3, 0xc2, 0x20, 0xFF},
    {0xc4, 0x65, 0x1c, 0xFF},
    {0xb5, 0x41, 0x31, 0xFF},
    {0x61, 0x40, 0x7a, 0xFF},
    {0x8f, 0x3d, 0xa7, 0xFF},
    {0xea, 0x61, 0x9d, 0xFF},
    {0xc1, 0xe5, 0xea, 0xFF}
  };

  auto colorDistance (const SDL_Color& color1, const SDL_Color& color2);
  SDL_Color getNearestColor(const SDL_Color& color);

} // end namespace Colors

namespace ColorsARGB8 {

  inline constexpr Uint32 black        {0xFF2e2c3b};
  inline constexpr Uint32 dark_blue    {0xFF3e415f};
  inline constexpr Uint32 dark_grey    {0xFF55607d};
  inline constexpr Uint32 grey         {0xFF747d88};
  inline constexpr Uint32 copper_green {0xFF41de95};
  inline constexpr Uint32 turquoise    {0xFF2aa4aa};
  inline constexpr Uint32 blue         {0xFF3b77a6};
  inline constexpr Uint32 dark_green   {0xFF249337};
  inline constexpr Uint32 green        {0xFF56be44};
  inline constexpr Uint32 yellow       {0xFFc6de78};
  inline constexpr Uint32 dark_yellow  {0xFFf3c220};
  inline constexpr Uint32 orange       {0xFFc4651c};
  inline constexpr Uint32 red          {0xFFb54131};
  inline constexpr Uint32 violet       {0xFF61407a};
  inline constexpr Uint32 purple       {0xFF8f3da7};
  inline constexpr Uint32 pink         {0xFFea619d};
  inline constexpr Uint32 white        {0xFFc1e5ea};

} // end namespace ColorsARGB8

} // end namespace ktp

#endif // YAMCHD_SRC_PALETTE_HPP_