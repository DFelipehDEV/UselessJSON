include(FetchContent)
FetchContent_Declare(
    rapidjson
    URL https://github.com/Tencent/rapidjson/archive/refs/heads/master.zip
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)
FetchContent_GetProperties(rapidjson)
if(NOT rapidjson_POPULATED)
    FetchContent_Populate(rapidjson)
endif()

add_library(UselessJSON SHARED ../useless.hpp api.cpp) 
target_include_directories(UselessJSON PRIVATE ${rapidjson_SOURCE_DIR}/include)

add_custom_command(TARGET UselessJSON POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "$<TARGET_FILE:UselessJSON>"
        "${CMAKE_CURRENT_BINARY_DIR}/UselessJSON.dll"
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/UselessJSON.gej"
        "${CMAKE_CURRENT_BINARY_DIR}/UselessJSON.gej"
    COMMAND python "${CMAKE_SOURCE_DIR}/gm82gex.py" "UselessJSON.gej"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    COMMENT "Created UselessJSON.gex at ${CMAKE_CURRENT_BINARY_DIR}"
)