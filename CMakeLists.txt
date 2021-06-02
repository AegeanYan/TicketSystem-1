cmake_minimum_required(VERSION 3.15)
project(code)

set(CMAKE_CXX_STANDARD 11)

add_executable(code  ticket.hpp time.hpp train.hpp user.hpp CommandHandler.hpp Database/B+Tree.hpp Database/data.hpp
        Bpt_and_database.cpp Bpt_and_database.h Bptest.cpp vector.h)