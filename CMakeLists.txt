###############################################################################
# Main CMake file for the Chase project
###############################################################################
# @author Michele Lora
#
    
cmake_minimum_required (VERSION 2.4)
project (Chase)

add_subdirectory(src)

set_property(TARGET chase PROPERTY CXX_STANDARD 11)

