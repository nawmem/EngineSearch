cmake_minimum_required (VERSION 3.25)
project ("EngineSearch")

add_executable (
        EngineSearch
        src/ConverterJSON.cpp
        src/Entry.cpp
        src/InvertedIndex.cpp
        src/SearchServer.cpp
        EngineSearch.cpp
        include/RelativeIndex.h
        include/ExeptNotFoundFile.h
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET EngineSearch PROPERTY CXX_STANDARD 20)
endif()

include_directories(
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/nlohmann_json)


add_subdirectory(nlohmann_json)
target_link_libraries(EngineSearch PRIVATE nlohmann_json::nlohmann_json)

add_subdirectory(./gtest)


