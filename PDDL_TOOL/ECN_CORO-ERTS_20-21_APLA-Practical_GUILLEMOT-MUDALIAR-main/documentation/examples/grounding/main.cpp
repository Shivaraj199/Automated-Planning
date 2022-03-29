/**
 * The aim of this problem is to move a robot from a room to another.
 * Two rooms must be connected so that the robot can move from one to another. The rooms are
 * connected following this map:
 *  _____ _____ _____
 * |     |     |     |
 * | rm0 | rm1 | rm2 |
 * |_____|_____|_____|
 * |     |     |     |
 * | rm3 |     | rm4 |
 * |_____|_____|_____|
 * |     |     |     |
 * | rm5 | rm6 | rm7 |
 * |_____|_____|_____|
 *
 * The expected path is: move(rm0, rm3) move(rm3, rm5) move(rm5, rm6)
*/

#include "planning_problem/domain.hpp"
#include "planning_problem/problem.hpp"
#include "solver.hpp"

#include <iostream>
#include <string>
#include <vector>

#ifndef SYMBOL
#define SYMBOL
	typedef std::string symbol
#endif

int main(void)
{
	/** GROUNDING THE DOMAIN **/

	domain dom("reach-the-room");

	dom.add_predicate("isIn", 1);
	dom.add_predicate("connected", 2);

	dom.add_action("move");
	dom.add_action_param("move", "x");
	dom.add_action_param("move", "y");
	dom.add_action_precond("move", "isIn", false, {"x"});
	dom.add_action_precond("move", "connected", false, {"x", "y"});
	dom.add_action_effect("move", "isIn", false, {"y"});
	dom.add_action_effect("move", "isIn", true, {"x"});

	/** GROUNDING THE PROBLEM **/
	problem prob(&dom);

	prob.add_object("rm0");
	prob.add_object("rm1");
	prob.add_object("rm2");
	prob.add_object("rm3");
	prob.add_object("rm4");
	prob.add_object("rm5");
	prob.add_object("rm6");
	prob.add_object("rm7");

	prob.ground_init("isIn", {"rm0"});

	prob.ground_init("connected", {"rm0", "rm1"});
	prob.ground_init("connected", {"rm0", "rm3"});
	prob.ground_init("connected", {"rm1", "rm0"});
	prob.ground_init("connected", {"rm1", "rm2"});
	prob.ground_init("connected", {"rm2", "rm1"});
	prob.ground_init("connected", {"rm2", "rm4"});
	prob.ground_init("connected", {"rm3", "rm0"});
	prob.ground_init("connected", {"rm3", "rm5"});
	prob.ground_init("connected", {"rm4", "rm2"});
	prob.ground_init("connected", {"rm4", "rm7"});
	prob.ground_init("connected", {"rm5", "rm3"});
	prob.ground_init("connected", {"rm5", "rm6"});
	prob.ground_init("connected", {"rm6", "rm5"});
	prob.ground_init("connected", {"rm6", "rm7"});
	prob.ground_init("connected", {"rm7", "rm4"});
	prob.ground_init("connected", {"rm7", "rm6"});

	prob.ground_final("isIn", {"rm6"});

	prob.ground_final("connected", {"rm0", "rm1"});
	prob.ground_final("connected", {"rm0", "rm3"});
	prob.ground_final("connected", {"rm1", "rm0"});
	prob.ground_final("connected", {"rm1", "rm2"});
	prob.ground_final("connected", {"rm2", "rm1"});
	prob.ground_final("connected", {"rm2", "rm4"});
	prob.ground_final("connected", {"rm3", "rm0"});
	prob.ground_final("connected", {"rm3", "rm5"});
	prob.ground_final("connected", {"rm4", "rm2"});
	prob.ground_final("connected", {"rm4", "rm7"});
	prob.ground_final("connected", {"rm5", "rm3"});
	prob.ground_final("connected", {"rm5", "rm6"});
	prob.ground_final("connected", {"rm6", "rm5"});
	prob.ground_final("connected", {"rm6", "rm7"});
	prob.ground_final("connected", {"rm7", "rm4"});
	prob.ground_final("connected", {"rm7", "rm6"});

	/** SOLVING THE PROBLEM WITH DIJKSTRA **/

	path p = astar(prob, zero_heuristic);

	for (std::vector<symbol> actions : p.second)
	{
		for (std::vector<symbol>::iterator it = actions.begin(); it < actions.end(); ++it)
		{
			if (it == ++(actions.begin()))
				printf("(");
			std::cout << *it;
			if (it != actions.begin() && it != --(actions.end()))
				printf(", ");
		}
		printf(") ");
	}
	printf("\n");

	return 0;
}
