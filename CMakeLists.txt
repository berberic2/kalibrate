cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0011 OLD)

project(kalibrate)
set(VERSION 0.1)

find_package(KDE4 REQUIRED)
include (KDE4Defaults)

add_definitions(-Wall -g)

# config.h
configure_file(config.h.in config.h)

subdirs(kalibrate) #  po doc

