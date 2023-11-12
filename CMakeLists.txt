cmake_minimum_required(VERSION 3.5)

project(desktopd
    VERSION 0.1.0
    LANGUAGES C CXX
)

add_executable(desktopd
    src/desktopd.c
    src/interfaces.c
    src/interfaces.h
    src/desktopd-server.h
    src/desktopd-server.c
)

# Link libdbus-1.
find_package(PkgConfig REQUIRED)
pkg_check_modules(DBUS REQUIRED dbus-1)

target_include_directories(desktopd
    PRIVATE ${DBUS_INCLUDE_DIRS})

target_link_options(desktopd
    PRIVATE -ldbus-1)

add_custom_target(introspections
    SOURCES
    introspections/SERVER_INTROSPECTION.xml
    introspections/IO_INTROSPECTION.xml
    introspections/ORBITRC_INTROSPECTION.xml
    introspections/DESKTOP_INTROSPECTION.xml
    introspections/DESKTOP_DESKTOP_INTROSPECTION.xml
)
