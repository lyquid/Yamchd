cmake_minimum_required(VERSION 3.0.0)
project(Yamchd VERSION 0.1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(CTest)
enable_testing()

# Use our modified FindSDL2* modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${Yamchd_SOURCE_DIR}/cmake")
# Set an output directory for our binaries
set(BIN_DIR ${Yamchd_SOURCE_DIR}/bin)

# Bump up warning levels appropriately for clang, gcc & msvc
# Also set debug/optimization flags depending on the build type. IDE users choose this when
# selecting the build mode in their IDE
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Weffc++ -Wmain -Wfloat-equal -pedantic")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
	if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()
endif()

# Look up SDL2 and add the include directory to our include path
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_IMAGE_INCLUDE_DIR})
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_TTF_INCLUDE_DIR})
find_package(SDL2_mixer REQUIRED)
include_directories(${SDL2_MIXER_INCLUDE_DIR})

add_executable(Yamchd main.cpp
											sdl2_wrappers/sdl2_audio.cpp
											sdl2_wrappers/sdl2_font.cpp
											sdl2_wrappers/sdl2_image.cpp
											sdl2_wrappers/sdl2_music.cpp
											sdl2_wrappers/sdl2_renderer.cpp
											sdl2_wrappers/sdl2_sound.cpp
											sdl2_wrappers/sdl2_texture.cpp
											sdl2_wrappers/sdl2_timer.cpp
											sdl2_wrappers/sdl2_window.cpp
											kuge/event_bus.cpp
											kuge/event.cpp
											kuge/audio_system.cpp
											kuge/input_system.cpp
											kuge/output_system.cpp
											kuge/system.cpp
											src/game.cpp
											src/palette.cpp
											src/world.cpp )
													
target_link_libraries(Yamchd ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY} ${SDL2_MIXER_LIBRARY})
target_include_directories(Yamchd PUBLIC "${Yamchd_SOURCE_DIR}/include")
install(TARGETS Yamchd RUNTIME DESTINATION ${BIN_DIR})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
