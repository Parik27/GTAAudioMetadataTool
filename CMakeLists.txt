cmake_minimum_required(VERSION 3.1)
project(IVAM)

set (CMAKE_CXX_STANDARD 20)

file(GLOB SOURCES CONFIGURE_DEPENDS src/*.cc src/common/*.cc)
add_executable(ivam ${SOURCES})

include_directories(src)

if (MSVC_VERSION GREATER_EQUAL "1900")
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

  # Because CMAKE_MSVC_RUNTIME_LIBRARY doesn't work for whatever reason
  set(CompilerFlags
    CMAKE_CXX_FLAGS
    CMAKE_CXX_FLAGS_DEBUG
    CMAKE_CXX_FLAGS_RELEASE
    CMAKE_C_FLAGS
    CMAKE_C_FLAGS_DEBUG
    CMAKE_C_FLAGS_RELEASE
    )
  foreach(CompilerFlag ${CompilerFlags})
    string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
  endforeach()
  
endif()

# nlohman_json
include(FetchContent)

FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
  GIT_TAG v3.9.1)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  message("-- Populating nlohmann::json")
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

FetchContent_Declare(fifo_map
  GIT_REPOSITORY https://github.com/nlohmann/fifo_map
  GIT_SHALLOW)

FetchContent_GetProperties(fifo_map)
if(NOT fifo_map_POPULATED)
  message("-- Populating nlohmann::fifo_map")
  FetchContent_Populate(fifo_map)
  add_subdirectory(${fifo_map_SOURCE_DIR} ${fifo_map_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

include_directories(${fifo_map_SOURCE_DIR}/src/)

target_link_libraries(ivam PRIVATE nlohmann_json::nlohmann_json)

add_subdirectory(tests)
enable_testing()
