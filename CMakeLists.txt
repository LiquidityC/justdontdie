cmake_minimum_required (VERSION 3.0.2)
include(ExternalProject)

SET(CMAKE_COLOR_MAKEFILE ON)

project (JustDontDie CXX)
set (JustDontDie_MAJOR_VERSION 1)
set (JustDontDie_MINOR_VERSION 0)

if(CMAKE_TOOLCHAIN_FILE)
	set(EXTERNAL_TOOLCHAIN "-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}")
else(CMAKE_TOOLCHAIN_FILE)
	set(EXTERNAL_TOOLCHAIN "")
endif(CMAKE_TOOLCHAIN_FILE)

if(MINGW)
	set(DLLS bin/win/SDL2.dll bin/win/SDL2_image.dll bin/win/SDL2_mixer.dll bin/win/SDL2_ttf.dll
		bin/win/libbz2-1.dll bin/win/libfreetype-6.dll bin/win/libgcc_s_seh-1.dll
		bin/win/libpng16-16.dll bin/win/libsqlite3-0.dll bin/win/libstdc++-6.dll bin/win/libwinpthread-1.dll
		bin/win/zlib1.dll)
endif(MINGW)

ExternalProject_Add(flat
	PREFIX flat
	GIT_REPOSITORY https://github.com/LiquidityC/flat
	CMAKE_ARGS ${EXTERNAL_TOOLCHAIN} -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_BINARY_DIR}/flat
	)

include_directories(${PROJECT_BINARY_DIR}/flat/include include)
link_directories(${PROJECT_BINARY_DIR}/flat/lib)

add_executable(justdontdie src/BloodParticle.cpp src/BoostParticle.cpp src/FireParticle.cpp
	src/FrameCounter.cpp src/GameStateController.cpp src/GhostOverlay.cpp src/GhostParticle.cpp
	src/LayerService.cpp src/MapParser.cpp src/MapTileObject.cpp src/Particle.cpp
	src/ParticleEmitter.cpp src/ParticleEngine.cpp src/Random.cpp src/ResourceContainer.cpp
	src/ResourceLoader.cpp src/Rocket.cpp src/Soldier.cpp src/SoldierMotionController.cpp
	src/SoldierPowerupContainer.cpp src/game.cpp)

if(MINGW)
	target_link_libraries(justdontdie -lflat -lmingw32 -lSDL2main -lSDL2 -mwindows -lSDL2_image -lSDL2_mixer -lSDL2_ttf)
	target_compile_options(justdontdie PRIVATE -std=c++11 -g -Wall)
else(MINGW)
	target_compile_options(justdontdie PRIVATE -std=c++11 -g -Wall)
	target_link_libraries(justdontdie -lflat -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf)
endif(MINGW)

add_executable(test_justdontdie  src/BloodParticle.cpp src/BoostParticle.cpp src/FireParticle.cpp
	src/FrameCounter.cpp src/GameStateController.cpp src/GhostOverlay.cpp src/GhostParticle.cpp
	src/LayerService.cpp src/MapParser.cpp src/MapTileObject.cpp src/Particle.cpp
	src/ParticleEmitter.cpp src/ParticleEngine.cpp src/Random.cpp src/ResourceContainer.cpp
	src/ResourceLoader.cpp src/Rocket.cpp src/Soldier.cpp src/SoldierMotionController.cpp
	src/SoldierPowerupContainer.cpp testsrc/DeadGaemTest.cpp testsrc/LayerServiceTest.cpp
	testsrc/TestSoldier.cpp)

if(MINGW)
	target_link_libraries(test_justdontdie -lflat -lmingw32 -lSDL2main -lSDL2 -mwindows -lSDL2_image -lSDL2_mixer -lSDL2_ttf)
	target_compile_options(test_justdontdie PRIVATE -std=c++11 -g -Wall)
else(MINGW)
	target_link_libraries(test_justdontdie -lflat -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf)
	target_compile_options(test_justdontdie PRIVATE -std=c++11 -g -Wall)
endif(MINGW)

add_custom_command(TARGET justdontdie
	POST_BUILD
	COMMENT "Copying resources"
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources ${PROJECT_BINARY_DIR}/resources
	)

if(MINGW)
	add_custom_command(TARGET justdontdie
		POST_BUILD
		COMMENT "Copying .dll files"
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/bin/win ${PROJECT_BINARY_DIR}
		COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/flat/bin/libflat.dll ${PROJECT_BINARY_DIR}
		)
endif(MINGW)

add_custom_target(lint
	COMMENT "Running cpp-lint"
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMAND python2.7 lint/cpplint.py --linelength=120 --root=src --extensions=cpp,h --filter=-whitespace/tab,-whitespace/parens,-whitespace/braces,-whitespace/comments,-legal src/*
	)

add_custom_target(check
	COMMENT "Running tests"
	DEPENDS test_justdontdie
	COMMAND ./test_justdontdie
	)

install(TARGETS justdontdie
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib)
install(DIRECTORY resources
	DESTINATION bin/resources)
if(MINGW)
	install(FILES ${DLLS} DESTINATION bin)
endif(MINGW)
