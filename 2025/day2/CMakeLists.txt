cmake_minimum_required(VERSION 3.10)

project(AdventOfCode2025 C)

add_executable(part1 part1.c)
add_executable(part2 part2.c)

target_include_directories(part1 PRIVATE $ENV{HOME}/.local/include)
target_include_directories(part2 PRIVATE $ENV{HOME}/.local/include)

