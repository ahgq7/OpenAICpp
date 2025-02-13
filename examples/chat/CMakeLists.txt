cmake_minimum_required(VERSION 3.16)
project(ChatExample)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ChatExample main.cpp)

include(FetchContent)
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(nlohmann_json)

target_link_libraries(ChatExample PRIVATE OpenAICpp ${CURL_LIBRARIES} nlohmann_json::nlohmann_json)
target_include_directories(ChatExample PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
