cmake_minimum_required(VERSION 3.16)
project(OpenAICpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL REQUIRED)
if(CURL_FOUND)
  include_directories(${CURL_INCLUDE_DIRS})
  message(STATUS "libcurl found")
else()
  message(FATAL_ERROR "libcurl not found")
endif()

include(FetchContent)
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(nlohmann_json)

add_library(OpenAICpp SHARED
    src/openai.cpp
    src/chatcompletion.cpp
    src/embedding.cpp
    src/imagegeneration.cpp
    src/networkrequest.cpp

    include/openai/openai.h
    include/openai/chatcompletion.h
    include/openai/embedding.h
    include/openai/imagegeneration.h
    include/openai/networkrequest.h
    include/openai/openai_export.h
    include/openai/error.h
)

target_compile_definitions(OpenAICpp PRIVATE OPENAI_LIBRARY)

target_include_directories(OpenAICpp PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(OpenAICpp PRIVATE CURL::libcurl)

# Çıktı dizinlerini zorla ayarla
set_target_properties(OpenAICpp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

add_subdirectory(examples/chat)
