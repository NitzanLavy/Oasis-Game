cmake_minimum_required(VERSION 3.10)
project(code)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror -pedantic-errors")

set(SOURCE_FILES
        Tree.h
        PlayerTree.h
        PlayerTree.cpp
        CoinTree.h
        CoinTree.cpp
        ClanTree.h
        ClanTree.cpp
        Oasis.h
        Oasis.cpp
        library1.h
        library1.cpp
        main1.cpp
        )

add_executable(Oasis ${SOURCE_FILES})
