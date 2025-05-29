cmake_minimum_required(VERSION 3.10)
project(TradeSimulator VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find required packages
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

# Add vcpkg for package management
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
endif()

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OPENSSL_INCLUDE_DIR}
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
)

# Add source files
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    Threads::Threads
    OpenSSL::SSL
    OpenSSL::Crypto
    Qt5::Core
    Qt5::Widgets
)

# Install dependencies
include(FetchContent)

# Fetch nlohmann_json
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
FetchContent_MakeAvailable(json)

# Fetch websocketpp
FetchContent_Declare(
    websocketpp
    GIT_REPOSITORY https://github.com/zaphoyd/websocketpp.git
    GIT_TAG 0.8.2
)
FetchContent_MakeAvailable(websocketpp)

# Add nlohmann_json to the project
target_link_libraries(${PROJECT_NAME} PRIVATE
    nlohmann_json::nlohmann_json
)

# Add websocketpp as a header-only library
target_include_directories(${PROJECT_NAME} PRIVATE
    ${websocketpp_SOURCE_DIR}
)

# Copy dependencies to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${websocketpp_SOURCE_DIR}/websocketpp
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/websocketpp
) 