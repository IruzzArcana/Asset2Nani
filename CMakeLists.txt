cmake_minimum_required(VERSION 3.12)

project(asset2nani)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/Nani.cpp
    src/NaniCmd.cpp
    src/JSONValidator.cpp
    src/NaniCmdDeserializer.cpp
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

target_include_directories(${PROJECT_NAME} PRIVATE
    includes
    external/json/single_include/nlohmann
)