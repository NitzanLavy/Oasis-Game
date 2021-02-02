cmake_minimum_required(VERSION 3.10)
project(Wet2)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror -pedantic-errors")

set(SOURCE_FILES
        Tree.h
        library2.h library2.cpp
        main.cpp
        List.h HashTable.h
        DynamicArray.h
        Heap.h
        Clan.cpp Clan.h
        Player.cpp Player.h
        Oasis2.cpp Oasis2.h
        Pair.h Pair.cpp)
add_executable(Wet2 ${SOURCE_FILES})