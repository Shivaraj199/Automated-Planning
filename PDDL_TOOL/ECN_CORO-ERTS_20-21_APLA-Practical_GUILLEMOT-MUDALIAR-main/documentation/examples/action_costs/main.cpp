/**
 * The aim of this problem is to move a robot from a room to another.
 * Two rooms must be connected so that the robot can move from one to another. The robot can also
 * break the wall which connects two adjacent rooms in order to connect them but with a higher cost.
 *
 * The rooms are distributed following this map (two connected rooms have a non-plain segment
 * between each other):
 *  _____ _____
 * |     |     |
 * | rm0   rm1 |
 * |_____|__ __|
 * |     |     |
 * | rm2   rm3 |
 * |_____|_____|
 *
 * The expected path if the cost of move is 1 is: move(rm0, rm1) move(rm1, rm3) move(rm3, rm2)
 * The expected path if the cost of move is 2 is: open(rm0, rm2) move(rm0, rm2)
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

	domain dom("reach-the-room2");

	dom.add_predicate("isIn", 1);
	dom.add_predicate("connected", 2);
	dom.add_predicate("adjacent", 2);

	dom.add_action("move");
	dom.set_action_cost("move", 2);
	dom.add_action_param("move", "x");
	dom.add_action_param("move", "y");
	dom.add_action_precond("move", "isIn", false, {"x"});
	dom.add_action_precond("move", "connected", false, {"x", "y"});
	dom.add_action_effect("move", "isIn", false, {"y"});
	dom.add_action_effect("move", "isIn", true, {"x"});

	dom.add_action("open");
	dom.set_action_cost("open", 3);
	dom.add_action_param("open", "x");
	dom.add_action_param("open", "y");
	dom.add_action_precond("open", "isIn", false, {"x"});
	dom.add_action_precond("open", "adjacent", false, {"x", "y"});
	dom.add_action_precond("open", "connected", true, {"x", "y"});
	dom.add_action_effect("open", "connected", false, {"x", "y"});
	dom.add_action_effect("open", "connected", false, {"y", "x"});

	/** GROUNDING THE PROBLEM **/
	problem prob(&dom);

	prob.add_object("rm0");
	prob.add_object("rm1");
	prob.add_object("rm2");
	prob.add_object("rm3");

	prob.ground_init("isIn", {"rm0"});

	prob.ground_init("connected", {"rm0", "rm1"});
	prob.ground_init("connected", {"rm1", "rm0"});
	prob.ground_init("connected", {"rm1", "rm3"});
	prob.ground_init("connected", {"rm2", "rm3"});
	prob.ground_init("connected", {"rm3", "rm1"});
	prob.ground_init("connected", {"rm3", "rm2"});

	prob.ground_init("adjacent", {"rm0", "rm1"});
	prob.ground_init("adjacent", {"rm0", "rm2"});
	prob.ground_init("adjacent", {"rm1", "rm0"});
	prob.ground_init("adjacent", {"rm1", "rm3"});
	prob.ground_init("adjacent", {"rm2", "rm0"});
	prob.ground_init("adjacent", {"rm2", "rm3"});
	prob.ground_init("adjacent", {"rm3", "rm1"});
	prob.ground_init("adjacent", {"rm3", "rm2"});

	prob.ground_final("isIn", {"rm2"});

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
