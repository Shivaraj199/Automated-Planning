cmake_minimum_required(VERSION 3.0)

project(data_structures)

set(
	HEADERS
	kdt.hpp
	tuple.hpp
)

add_library(apla ${HEADERS})
