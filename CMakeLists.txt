cmake_minimum_required(VERSION 3.12)

project(asset2nani)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/Nani.cpp
    src/NaniCmd.cpp
    src/JSONValidator.cpp
    src/NaniCmdDeserializer.cpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE -DVERSION="v1.0.2")

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

target_include_directories(${PROJECT_NAME} PRIVATE
    includes
    external/json/single_include/nlohmann
    external/argparse/include/argparse
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/data/nanicmds.json"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/nanicmds.json"
    COMMENT "Copying nanicmds.json..."
)