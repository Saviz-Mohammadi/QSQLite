cmake_minimum_required(VERSION 3.14)

project(QSQLite LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Sql)



set(HEADER_FILES

    "${CMAKE_CURRENT_SOURCE_DIR}/include/DatabaseSQLite.hpp")

set(SOURCE_FILES

    "${CMAKE_CURRENT_SOURCE_DIR}/src/DatabaseSQLite.cpp")



add_library(${PROJECT_NAME} STATIC)



target_sources(${PROJECT_NAME}

    PUBLIC
    ${HEADER_FILES}

    PRIVATE
    ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME}

    PUBLIC

    "${CMAKE_CURRENT_SOURCE_DIR}/include")


target_link_libraries(${PROJECT_NAME}

   Qt${QT_VERSION_MAJOR}::Core
   Qt${QT_VERSION_MAJOR}::Sql)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
