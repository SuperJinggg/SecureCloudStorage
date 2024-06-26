cmake_minimum_required(VERSION 3.20)
project(cloudStorage VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "E:/Qt/5.15.2/mingw81_64/lib/cmake")

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DQT_NO_DEBUG_OUTPUT)
endif ()

find_package(Qt5 COMPONENTS
             Core
             gui
             Widgets
             network
             test
             sql
             REQUIRED)

add_subdirectory(Login)
add_subdirectory(UserRegister)
add_subdirectory(UserClient)
add_subdirectory(SystemAdminClient)
add_subdirectory(SecurityAdminClient)
add_subdirectory(AuditAdminClient)

install(TARGETS ${PROJECT_NAME}
        BUNDLE  DESTINATION .
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()

    if (MINGW)
        
    endif ()
endif ()

install(FILES ${QT_INSTALL_PATH}/bin/Qt5Sql.dll
        DESTINATION ${CMAKE_INSTALL_PREFIX})

find_file(LIBEAY32 NAMES libeay32.dll PATHS ${CMAKE_SYSTEM_LIBRARY_PATH})
find_file(SSLEAY32 NAMES ssleay32.dll PATHS ${CMAKE_SYSTEM_LIBRARY_PATH})
install(FILES ${LIBEAY32} ${SSLEAY32}
        DESTINATION ${CMAKE_INSTALL_PREFIX})
