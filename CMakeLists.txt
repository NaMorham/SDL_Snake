cmake_minimum_required(VERSION 3.16...4.4)

project(SDL_Snake VERSION 1.0.0.0)

configure_file(SDL_SnakeVer.h.in SDL_SnakeVer.h)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

message("PROJECT_NAME: ${PROJECT_NAME}")
message("SDL base dir: ${SDL2_DIR}")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set (SRCS
    Vec2D.inl
    RigidBody.inl
    Game.cpp
    main.cpp
)

set (HDRS
    ${PROJECT_BINARY_DIR}/SDL_SnakeVer.h
    Vec2D.h
    RigidBody.h
    Game.h
)

set (FONTS
    fonts/cour.ttf
    fonts/courbd.ttf
    fonts/courbi.ttf
    fonts/couri.ttf
)

set (EXTRAS
    SDL_SnakeVer.h.in
)

source_group("Headers" FILES ${HDRS})
source_group("Resources/fonts" FILES ${FONTS})
source_group("Sources" FILES ${SRCS})
source_group("Extras" FILES ${EXTRAS})

add_executable(${PROJECT_NAME} ${SRCS} ${HDRS} ${FONTS} ${EXTRAS})

set_target_properties(${PROJECT_NAME} PROPERTIES
   VS_STARTUP_PROJECT ${PROJECT_NAME}
   CXX_STANDARD 20
   C_STANDARD 17
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_BINARY_DIR}
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} SDL2_ttf::SDL2_ttf)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_SOURCE_DIR}/out/NaM/bin)
#install(TARGET_RUNTIME_DLLS ${PROJECT_NAME} DESTINATION ${CMAKE_SOURCE_DIR}/out/NaM/bin)
