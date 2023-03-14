# Configure with [fxsdk build-fx] or [fxsdk build-cg], which provide the
# toolchain file and module path of the fxSDK

cmake_minimum_required(VERSION 3.15)
project(fxPI)

include(GenerateG1A)
include(GenerateG3A)
include(Fxconv)
find_package(Gint 2.9 REQUIRED)

set(SOURCES
  src/main.c
)

set(ASSETS_fx
  assets-fx/tiny_font.png
)

fxconv_declare_assets(${ASSETS_fx} WITH_METADATA)

add_executable(fxPI ${SOURCES} ${ASSETS_${FXSDK_PLATFORM}})
target_compile_options(fxPI PRIVATE -Wall -Wextra -Os)
target_link_libraries(fxPI Gint::Gint)

if("${FXSDK_PLATFORM_LONG}" STREQUAL fx9860G)
  generate_g1a(TARGET fxPI OUTPUT "fxPI.g1a"
    NAME "fxPI" ICON assets-fx/icon.png)
elseif("${FXSDK_PLATFORM_LONG}" STREQUAL fxCG50)
  generate_g3a(TARGET myaddin OUTPUT "MyAddin.g3a"
    NAME "MyAddin" ICONS assets-cg/icon-uns.png assets-cg/icon-sel.png)
endif()
