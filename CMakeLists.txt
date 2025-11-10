cmake_minimum_required(VERSION 3.14)

set(PROJ_NAME starlet_image_sandbox)
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/inc")
set(ASSET_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")

project(${PROJ_NAME} LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_SUPPRESS_REGENERATION ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${SRC_DIR}/*.cpp)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS ${INC_DIR}/starlet-image-sandbox/*.hpp)
file(GLOB_RECURSE ASSETS  CONFIGURE_DEPENDS "${ASSET_DIR}/*")

add_executable(${PROJ_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJ_NAME} PUBLIC "${INC_DIR}")

file(TO_CMAKE_PATH "${ASSET_DIR}" ASSET_PATH)
target_compile_definitions(${PROJ_NAME} PRIVATE ASSET_DIR="${ASSET_PATH}")

source_group(TREE ${SRC_DIR} PREFIX "Source Files" FILES ${SOURCES})
source_group(TREE ${INC_DIR} PREFIX "Header Files" FILES ${HEADERS})

add_custom_target(Assets SOURCES ${ASSETS})
set_target_properties(Assets PROPERTIES FOLDER "Assets")
source_group("Assets" FILES ${ASSETS})

include(FetchContent)
FetchContent_Declare(starlet_serializer 
  GIT_REPOSITORY https://github.com/masonlet/starlet-serializer.git 
  GIT_TAG main
)
FetchContent_MakeAvailable(starlet_serializer)

target_link_libraries(${PROJ_NAME} PUBLIC starlet_serializer)

set_target_properties(${PROJ_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROJ_NAME}>"
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJ_NAME})




