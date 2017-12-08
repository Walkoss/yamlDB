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
        sources/data/data.c
        sources/data/data.h
        sources/select/select.c
        sources/utils/strsplit.c
        sources/utils/strsplit.h)

add_executable(yamlDB ${SOURCE_FILES})