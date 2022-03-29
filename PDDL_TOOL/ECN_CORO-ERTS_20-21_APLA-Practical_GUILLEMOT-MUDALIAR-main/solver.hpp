#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "planning_problem/problem.hpp"
#include "planning_problem/state.hpp"

#include <algorithm>
#include <cassert>
#include <climits>
#include <stack>
#include <tuple>
#include <vector>

// FOR TEST PURPOSES ONLY
#include <iostream>
// END OF TEST

typedef std::tuple<std::vector<state>, std::vector<std::vector<symbol>>, unsigned int> path;

/**
 * @arg prob The problem to solve
 * @arg init The initial state whom estimated cost we want to compute
 * @arg power The power of the heuristic in its family (used only with critical path heuristic
 *	      to choose between h^{1}, h^{2}, etc.)
 * @return The estimated cost of solving the problem from state init.
*/
typedef unsigned int (*heuristic)(const problem &prob, const state &init, unsigned int power);

struct
{
	bool operator()(std::pair<state, unsigned int> p1, std::pair<state, unsigned int> p2) const
	{
		return p1.second > p2.second;
	}
} greater;

bool max_count(unsigned int nb_params, unsigned int nb_objects, unsigned int *counter);

void increment_count(unsigned int nb_params, unsigned int nb_objects, unsigned int *counter,
		     unsigned int index = 0);

/**
 * @arg prob The problem to solve
 * @arg h The heuristics used to estimate the cost of a state
 * @arg power The power of the heuristic in its family (used only with critical path heuristic
 * 	      to choose between h^{1}, h^{2}, etc.)
*/
path astar(const problem &prob, heuristic h, unsigned int power = 1);

unsigned int zero_heuristic(const problem &prob, const state &init, unsigned int power);

unsigned int delete_relaxation(const problem &prob, const state &init, unsigned int power);

unsigned int critical_path(const problem &prob, const state &init, unsigned int power);

#endif // SOLVER_HPP
