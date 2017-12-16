cmake_minimum_required(VERSION 3.8)
project(yamlDB)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES
        sources/main.c
        sources/database/database.c
        sources/database/database.h
        sources/utils/xmalloc.c
        sources/utils/xmalloc.h
        sources/table/table.c
        sources/table/table.h
        sources/field/field.c
        sources/field/field.h
        sources/table/table.h
        sources/interface/interface.c
        sources/interface/interface.h
        sources/interface/interface.c
        sources/interface/interface.h
        sources/gtk/gtk.c
        sources/gtk/gtk.h)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER})
add_executable(yamlDB ${SOURCE_FILES})

target_link_libraries(yamlDB ${GTK3_LIBRARIES})