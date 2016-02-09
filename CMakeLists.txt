cmake_minimum_required (VERSION 3.0.2)
include(ExternalProject)

SET(CMAKE_COLOR_MAKEFILE ON)

project (JustDontDie)
set (JustDontDie_MAJOR_VERSION 1)
set (JustDontDie_MINOR_VERSION 0)

ExternalProject_Add(flat
	PREFIX flat
	GIT_REPOSITORY https://github.com/LiquidityC/flat
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_BINARY_DIR}/flat
	TEST_AFTER_INSTALL
	TEST_COMMAND ${PROJECT_BINARY_DIR}/flat/bin/test_flat
	)

include_directories(${PROJECT_BINARY_DIR}/flat/include include)
link_directories(${PROJECT_BINARY_DIR}/flat/lib)

add_executable(justdontdie src/BloodParticle.cpp src/BoostParticle.cpp src/FireParticle.cpp
	src/FrameCounter.cpp src/GameStateController.cpp src/GhostOverlay.cpp src/GhostParticle.cpp
	src/LayerService.cpp src/MapParser.cpp src/MapTileObject.cpp src/Particle.cpp
	src/ParticleEmitter.cpp src/ParticleEngine.cpp src/Random.cpp src/ResourceContainer.cpp
	src/ResourceLoader.cpp src/Rocket.cpp src/Soldier.cpp src/SoldierMotionController.cpp
	src/SoldierPowerupContainer.cpp src/game.cpp)

target_link_libraries(justdontdie -lflat -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lsqlite3)
target_compile_options(justdontdie PRIVATE -std=c++11 -g -Wall)

add_executable(test_justdontdie  src/BloodParticle.cpp src/BoostParticle.cpp src/FireParticle.cpp
	src/FrameCounter.cpp src/GameStateController.cpp src/GhostOverlay.cpp src/GhostParticle.cpp
	src/LayerService.cpp src/MapParser.cpp src/MapTileObject.cpp src/Particle.cpp
	src/ParticleEmitter.cpp src/ParticleEngine.cpp src/Random.cpp src/ResourceContainer.cpp
	src/ResourceLoader.cpp src/Rocket.cpp src/Soldier.cpp src/SoldierMotionController.cpp
	src/SoldierPowerupContainer.cpp testsrc/DeadGaemTest.cpp testsrc/LayerServiceTest.cpp
	testsrc/TestSoldier.cpp)

target_link_libraries(test_justdontdie -lflat -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lsqlite3)
target_compile_options(test_justdontdie PRIVATE -std=c++11 -g -Wall)

add_custom_command(TARGET justdontdie
	POST_BUILD
	COMMENT "Copying resources"
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources ${PROJECT_BINARY_DIR}/resources
	)

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
