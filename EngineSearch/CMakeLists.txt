# CMakeList.txt: проект CMake для EngineSearch; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#

# Добавьте источник в исполняемый файл этого проекта.
add_executable (
	EngineSearch 
	"EngineSearch.cpp" "EngineSearch.h"
	"src/InvertedIndex.h" "include/InvertedIndex.cpp"
	"src/Entry.h" "include/Entry.cpp"
	"src/ConverterJSON.h" "include/ConverterJSON.cpp"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET EngineSearch PROPERTY CXX_STANDARD 20)
endif()

add_subdirectory(nlohmann_json)
target_link_libraries(EngineSearch PRIVATE nlohmann_json::nlohmann_json)
# TODO: Добавьте тесты и целевые объекты, если это необходимо.

add_subdirectory(./gtest)