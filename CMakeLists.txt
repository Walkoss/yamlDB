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
        sources/token/token.c
        sources/token/token.h
        sources/lexer/lexer.c
        sources/lexer/lexer.h
        sources/hash_map/hash_map.c
        sources/hash_map/hash_map.h
        sources/parser/parser.c
        sources/parser/parser.h
        sources/utils/to_lower.c
        sources/utils/to_lower.h sources/statement/statement.c sources/statement/statement.h)

add_executable(yamlDB ${SOURCE_FILES})