cmake_minimum_required(VERSION 3.5.0)

project(capstone_project VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE HEADER_FILES ${PROJECT_SOURCE_DIR}/include/*.h)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 REQUIRED COMPONENTS Widgets Qml Quick
    HINTS "/opt/homebrew/opt/qt@5")

#MAIN APPLICATION
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

#QML LINT FOR COMPILE TIME ERRORS
file(GLOB_RECURSE QML_FILES ${PROJECT_SOURCE_DIR}/resource/qml/*.qml)

# Create a custom target that always runs the linting commands
add_custom_target(qmllint ALL)

# Iterate over each QML file and create a custom command
foreach(QML_FILE ${QML_FILES})
    add_custom_command(
        TARGET qmllint
        PRE_BUILD  # Execute before building the `qmllint` target
        COMMAND ${CMAKE_COMMAND} -E echo "Linting ${QML_FILE}..."
        COMMAND ${CMAKE_COMMAND} -E env qmllint -I ${PROJECT_SOURCE_DIR}/resource/qml --resource resource/resources.qrc --unqualified info ${QML_FILE} || ${CMAKE_COMMAND} -E echo "Linting failed for ${QML_FILE}, but build will continue."
    )
endforeach()


add_executable(capstone
    ${SOURCES}
    resource/resources.qrc
    ${MOC_WRAPPED_FILES}
)

target_link_libraries(capstone
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Qml
    Qt${QT_VERSION_MAJOR}::Quick
)


#TESTS
add_subdirectory(third_party/googletest)
file(GLOB_RECURSE TEST_SOURCES "src/*.cpp")
foreach(source IN ITEMS ${TEST_SOURCES})
    if(source MATCHES "main\\.cpp$")
        list(REMOVE_ITEM TEST_SOURCES ${source})
    endif()
endforeach()

add_executable(test
    test/tests.cpp
    ${TEST_SOURCES}
)

target_compile_definitions(test
    PRIVATE G_TEST
)

target_link_libraries(test
    gtest_main
    ${CURSES_LIBRARIES}
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Qml
    Qt${QT_VERSION_MAJOR}::Quick
)