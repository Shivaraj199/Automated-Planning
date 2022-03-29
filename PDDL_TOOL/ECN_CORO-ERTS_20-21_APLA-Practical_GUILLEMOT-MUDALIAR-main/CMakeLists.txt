cmake_minimum_required(VERSION 3.0)

project(apla)

add_subdirectory(data_structures)

set(
	SRCS
	main.cpp
	solver.cpp
	planning_problem/action.cpp
	planning_problem/domain.cpp
	planning_problem/problem.cpp
	planning_problem/state.cpp
	parser.cpp
)


set(
	HEADERS
	solver.hpp
	planning_problem/action.hpp
	planning_problem/domain.hpp
	planning_problem/problem.hpp
	planning_problem/state.hpp
	parser.hpp

)

add_executable(main ${SRCS})
set_target_properties(apla PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(main apla)
