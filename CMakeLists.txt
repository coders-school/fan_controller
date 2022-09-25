cmake_minimum_required(VERSION 3.11.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        main # release-1.10.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

enable_testing()

project(FanController)

add_compile_options(-Wall -Wextra -Wpedantic -g)
include_directories(.)
add_library(SlowThermometer-lib SlowThermometer/SlowThermometer.cpp)

set(SRC_LIST
    Controller.hpp
    Controller.cpp
    Fan.hpp
    Fan.cpp
    LcdDisplay.hpp
    LcdDisplay.cpp
    ProductionThermometer.cpp
)

add_library(${PROJECT_NAME}-lib ${SRC_LIST})
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-lib SlowThermometer-lib pthread)

set(TEST_SRC_LIST
    tests/FanTests.cpp
    tests/ControllerTests.cpp
)

# unit test with gtest
add_executable(Controller-ut tests/ControllerTests.cpp)
# # //TODO: VERIFY
# target_include_directories(${PROJECT_NAME}-ut
#                            PRIVATE
#                            ${CMAKE_SOURCE_DIR})
target_link_libraries(Controller-ut
                      ${PROJECT_NAME}-lib
                      SlowThermometer-lib # //TODO: VERIFY
                      gtest_main
                      gmock_main
                      pthread)

#  unit-tests with catch
add_executable(Fan-ut tests/FanTests.cpp)
# //TODO: VERIFY
# target_include_directories(${PROJECT_NAME}-ut
#                            PRIVATE
#                            ${CMAKE_SOURCE_DIR})
target_link_libraries(Fan-ut
                      ${PROJECT_NAME}-lib
                      SlowThermometer-lib # //TODO: VERIFY
                      pthread)
# // TODO: REMOVE
# include(GoogleTest)
gtest_discover_tests(Controller-ut)
