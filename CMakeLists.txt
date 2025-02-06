# Configure with [fxsdk build-fx] or [fxsdk build-cg], which provide the
# toolchain file and module path of the fxSDK

cmake_minimum_required(VERSION 3.15)
project(fxPI)

if (DEFINED FXSDK_PLATFORM)
  include(GenerateG1A)
  include(GenerateG3A)
  include(Fxconv)
  find_package(Gint 2.9 REQUIRED)
  add_compile_definitions(__CASIO__)
endif()

file(GLOB SOURCES src/*.c  src/*.h)

set(ASSETS_fx
  assets-fx/tiny_font.png
)

set(ASSETS_cg
  assets-cg/tiny_font.png
)
if (DEFINED FXSDK_PLATFORM)
  fxconv_declare_assets(${ASSETS_fx} ${ASSETS_cg} WITH_METADATA)
endif()

add_executable(fxPI ${SOURCES} ${ASSETS_${FXSDK_PLATFORM}})
target_compile_options(fxPI PRIVATE -Wall -Wextra -Os)
if (DEFINED FXSDK_PLATFORM)
  target_link_libraries(fxPI Gint::Gint)
endif()
message(STATUS ${FXSDK_PLATFORM})
if(FXSDK_PLATFORM STREQUAL "fx")
  generate_g1a(TARGET fxPI OUTPUT "fxPI.g1a" NAME "fxPI" ICON assets-fx/icon.png)
elseif(FXSDK_PLATFORM STREQUAL "cg")
  generate_g3a(TARGET fxPI OUTPUT "fxPI.g3a" NAME "fxPI" ICONS assets-cg/icon-uns.png assets-cg/icon-sel.png)
endif()
