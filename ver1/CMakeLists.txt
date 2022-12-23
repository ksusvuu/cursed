# set the minimum version of cmake required
cmake_minimum_required(VERSION 3.0)

# set the project name
project(graph_colorer_old)

# add the executable
add_executable(graph_colorer_old main.cpp graph.cpp graph.h)