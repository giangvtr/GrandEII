cmake_minimum_required(VERSION 3.10)  # Or a more recent version

project(GRANDEII_PC C)

# Set the C standard
set(CMAKE_C_STANDARD 99) # or 11
set (CMAKE_CXX_FLAGS "-rdynamic")

#Set this option to ON to generate coverage data
option(COVER "Generate coverage data" "ON")

# Find GTK+ 3
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS})

#Set ui and css path
file(REAL_PATH "./view/interface.ui" INTERFACE_UI)
add_compile_definitions(UI_PATH="${INTERFACE_UI}")

file(REAL_PATH "./view/style.css" CSS_STYLE)
add_compile_definitions(CSS_PATH="${CSS_STYLE}")

file(REAL_PATH "../data" DATA)
add_compile_definitions(DATA_PATH="${DATA}")

# Source files
file(GLOB_RECURSE ARDUINO_SOURCES "arduino/*.c")
list(FILTER ARDUINO_SOURCES EXCLUDE REGEX ".*main\\.c$")

file(GLOB_RECURSE MODEL_SOURCES "model/*.c")
list(FILTER MODEL_SOURCES EXCLUDE REGEX ".*main\\.c$")

file(GLOB_RECURSE CONTROLLER_SOURCES "controller/*.c")
list(FILTER CONTROLLER_SOURCES EXCLUDE REGEX ".*main\\.c$")

file(GLOB_RECURSE VIEW_SOURCES "view/*.c")
list(FILTER VIEW_SOURCES EXCLUDE REGEX ".*main\\.c$")

file(GLOB_RECURSE UT_SOURCES "unit_test/*.c")
list(FILTER UT_SOURCES EXCLUDE REGEX ".*main\\.c$")

# Include directories (where your .h files are)
include_directories("src")      # Include src for internal headers
include_directories("src/model")
include_directories("src/arduino")
include_directories("src/controller")
include_directories("src/view")
include_directories("src/unit_test")

# Create executable
add_executable(GRANDEII_PC ${ARDUINO_SOURCES} ${MODEL_SOURCES} ${VIEW_SOURCES} ${CONTROLLER_SOURCES} main.c)
add_executable(ARDUINO ${ARDUINO_SOURCES} ./arduino/main.c)
add_executable(MODEL ${MODEL_SOURCES} ${ARDUINO_SOURCES} ./model/main.c)
add_executable(UNIT_TESTS ${UT_SOURCES} ${ARDUINO_SOURCES} ${MODEL_SOURCES} ${VIEW_SOURCES} ${CONTROLLER_SOURCES} unit_test/main.c)

# Linking libs
target_link_libraries(GRANDEII_PC ${GTK3_LIBRARIES} cmocka m)
target_link_libraries(ARDUINO ${GTK3_LIBRARIES} cmocka m)
target_link_libraries(MODEL ${GTK3_LIBRARIES} cmocka m)
target_link_libraries(UNIT_TESTS ${GTK3_LIBRARIES} cmocka m)

# Linking wraps for Unit testing
target_link_options(UNIT_TESTS PRIVATE -Wl,--wrap=rand)
#target_link_options(UNIT_TESTS PRIVATE -Wl,--wrap=my_malloc)

#Adding compile option for coverage
if(COVER STREQUAL "ON")
    target_compile_options(GRANDEII_PC PRIVATE -coverage -O0)
    target_link_options(GRANDEII_PC PRIVATE -coverage)
    target_compile_options(ARDUINO PRIVATE -coverage -O0)
    target_link_options(ARDUINO PRIVATE -coverage)
    target_compile_options(MODEL PRIVATE -coverage -O0)
    target_link_options(MODEL PRIVATE -coverage)
    target_compile_options(UNIT_TESTS PRIVATE -coverage -O0)
    target_link_options(UNIT_TESTS PRIVATE -coverage)
endif()

# Customs commands like clean, generate docs, ...

add_custom_target(clean_all
    COMMAND rm -f *.o *.d *.gcno *.gcda *.gcov *.info *.log *.exe
    COMMAND rm -rf out
    COMMAND rm -rf build
    COMMAND rm -rf cmake-build-debug
    COMMAND rm -f cmake_install.cmake
    COMMAND rm -f CMakeCache.txt
    COMMAND rm -rf CMakeFiles
    COMMAND rm -f Makefile
    COMMAND rm -f UNIT_TESTS
)
