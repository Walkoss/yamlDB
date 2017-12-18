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
        sources/data/data.c
        sources/data/data.h
        sources/token/token.c
        sources/token/token.h
        sources/lexer/lexer.c
        sources/lexer/lexer.h
        sources/hash_map/hash_map.c
        sources/hash_map/hash_map.h
        sources/parser/parser.c
        sources/parser/parser.h
        sources/utils/to_lower.c
        sources/utils/to_lower.h
        sources/statement/statement.c
        sources/statement/statement.h
        sources/data/data.h
        sources/utils/strsplit.c
        sources/utils/strsplit.h
        sources/select/select.c
        sources/select/union.c
        sources/interface/interface.c
        sources/interface/interface.h
        sources/gtk/gtk.c
        sources/gtk/gtk.h
        sources/print_color/print_color.c
        sources/print_color/print_color.h)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER})
add_executable(yamlDB ${SOURCE_FILES})

target_link_libraries(yamlDB ${GTK3_LIBRARIES})