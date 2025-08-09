cmake_minimum_required(VERSION 3.16)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(kPlugin_Installer)

include("$ENV{VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME "Kernel Plugin Installer")
set(VITA_TITLEID "KPLUG0001")
SET(VITA_VERSION "01.00")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1")

include_directories(
    include
    3rdparty
    /usr/local/vitasdk/arm-vita-eabi/include
)

link_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    $ENV{VITASDK}/arm-vita-eabi/lib
)

add_executable(${PROJECT_NAME}
    src/main.c
    src/font.c
    src/graphics.c
)

target_link_libraries(${PROJECT_NAME}
  SceLibKernel_stub
  SceDisplay_stub
  SceCtrl_stub
  ScePower_stub
  SceVshBridge_stub
)

vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME})

vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} ${PROJECT_NAME}.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
  #FILE data/sce_sys/icon0.png sce_sys/icon0.png
  #FILE data/sce_sys/livearea/contents/bg.png sce_sys/livearea/contents/bg.png
  #FILE data/sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
  #FILE data/sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
)
