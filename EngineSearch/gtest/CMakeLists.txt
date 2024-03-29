set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
	googletest
	 URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

include_directories(
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/nlohmann_json/include)


add_executable(
	EngineSearchTest
	${CMAKE_SOURCE_DIR}/EngineSearch/src/ConverterJSON.cpp
	${CMAKE_SOURCE_DIR}/EngineSearch/src/Entry.cpp
	${CMAKE_SOURCE_DIR}/EngineSearch/src/InvertedIndex.cpp
	${CMAKE_SOURCE_DIR}/EngineSearch/src/SearchServer.cpp
	main_test.cpp)

target_link_libraries(
	EngineSearchTest
	GTest::gtest_main
	nlohmann_json::nlohmann_json
)
include(GoogleTest)
gtest_discover_tests(EngineSearchTest)