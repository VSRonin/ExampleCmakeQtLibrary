cmake_minimum_required(VERSION 3.12)
project(MyLib LANGUAGES CXX)
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)
set(VERSION_SHORT "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
option(BUILD_SHARED_LIBS "Build the library as a dynamically linked library" ON)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets Gui REQUIRED)

include(GenerateExportHeader)
include(GNUInstallDirs)
add_library(MyLib myclass.h myclass.cpp)
add_library(MyLib::MyLib ALIAS MyLib)
generate_export_header(MyLib
    EXPORT_FILE_NAME globaldefs/mylib_global.h
)
target_include_directories(MyLib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/globaldefs>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/includes>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(MyLib PUBLIC MYLIB_STATIC_DEFINE)
endif()
target_link_libraries(MyLib PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(MyLib PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN TRUE
    SOVERSION ${VERSION_MAJOR}
    VERSION ${VERSION_SHORT}
    EXPORT_NAME "MyLib"
)
include(CMakePackageConfigHelpers)
write_basic_package_version_file("MyLibConfigVersion.cmake"
    VERSION ${VERSION_SHORT}
    COMPATIBILITY SameMajorVersion
)
install(FILES myclass.h includes/MyClass "${CMAKE_CURRENT_BINARY_DIR}/globaldefs/mylib_global.h"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    COMPONENT MyLib_Development
)
install(TARGETS MyLib
    EXPORT MyLibTargets
    RUNTIME
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT MyLib_Runtime
    LIBRARY
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT MyLib_Runtime
        NAMELINK_COMPONENT MyLib_Development
    INCLUDES
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        COMPONENT MyLib_Development
    ARCHIVE
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT MyLib_Development
)

install(EXPORT MyLibTargets
        FILE MyLibTargets.cmake
        NAMESPACE MyLib::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MyLib
        COMPONENT MyLib_Development
)
install(FILES "MyLibConfig.cmake" "${CMAKE_CURRENT_BINARY_DIR}/MyLibConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MyLib
    COMPONENT MyLib_Development
)
