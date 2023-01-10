cmake_minimum_required(VERSION 3.5)

project(QtPS VERSION 0.1 LANGUAGES CXX)


set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS include) # 设置 AUTO UIC 的搜索文件夹

set(OpenCV_DIR "D:/Program Files/OpenCV_4.7.0/opencv/my_build/install/x64/mingw/lib/") # OpenCVConfig.cmake 所在路径

#添加头文件
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/include/)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Charts)
find_package(OpenCV REQUIRED)

file(GLOB_RECURSE CPP_SRC_FILES src/*.cpp)
file(GLOB_RECURSE INCLUDE_SRC_FILES include/*.h include/*.hpp)
file(GLOB_RECURSE UI_FILES include/*.ui)

set(PROJECT_SOURCES
        ${UI_FILES}
        ${CPP_SRC_FILES}
        ${INCLUDE_SRC_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    find_package(Qt6 COMPONENTS Core5Compat REQUIRED)
    set(Qt6_COMPAT Qt6::Core5Compat)
    qt_add_executable(QtPS
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

# Define target properties for Android with Qt 6 as:
#    set_property(TARGET QtPS APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(QtPS SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(QtPS
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(QtPS PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Charts ${OpenCV_LIBS} ${Qt6_COMPAT})

set_target_properties(QtPS PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS QtPS
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QtPS)
endif()
