cmake_minimum_required(VERSION 3.26)

project(libcapture)

set(
  SRC
  ./src/input/mouse.c
  ./src/input/keyboard.c
  ./src/input/listener.c
  ./src/clipboard/clipboard.c
  ./src/utils.c
)

# target_link_libraries($(PROJECT_NAME))

# add_executable(${PROJECT_NAME} ${SRC})

add_library(${PROJECT_NAME} SHARED ${SRC})