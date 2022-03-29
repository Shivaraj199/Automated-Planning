/**
 * The aim of this problem is to move a robot from a start tile to an end tile avoiding the
 * holes.
 * The tiles are described in the following map (the holes behind indicated by a !):
 *  _____ _____ _____ _____ _____
 * |     |     |     |     |     |
 * | A1  | A2  | A3! | A4  | A5  |
 * |_____|_____|_____|_____|_____|
 * |     |     |     |     |     |
 * | B1! | B2  | B3  | B4  | B5! |
 * |_____|_____|_____|_____|_____|
 * |     |     |     |     |     |
 * | C1  | C2  | C3! | C4  | C5  |
 * |_____|_____|_____|_____|_____|
 * |     |     |     |     |     |
 * | D1! | D2  | D3  | D4  | D5! |
 * |_____|_____|_____|_____|_____|
 * |     |     |     |     |     |
 * | E1  | E2  | E3! | E4  | E5  |
 * |_____|_____|_____|_____|_____|
 *
 * The expected paths are either:
 * 	- move(A1, A2) move(A2, B2) move(B2, B3) move(B3, B4) move(B4, C4) move(C4, D4) move(D4, E4)
 *	  move(E4, E5)
 *	- move(A1, A2) move(A2, B2) move(B2, C2) move(C2, D2) move(D2, D3) move(D3, D4) move(D4, E4)
 * 	  move(E4, E5)
*/

#include "planning_problem/domain.hpp"
#include "planning_problem/problem.hpp"
#include "solver.hpp"

#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#ifndef SYMBOL
#define SYMBOL
	typedef std::string symbol
#endif

template<typename T1, typename T2, typename T3> using triplet = std::tuple<T1, T2, T3>;

int main(void)
{
	/** GROUNDING THE DOMAIN **/
	domain dom("avoid-the-holes");

	/**
	 * A conditional effect is composed of:
	 * 	- a list of pre-conditions (std::vector<triplet<symbol, bool, std::vector<symbol>>>)
	 * 	- a list of effects (std::vector<triplet<symbol, bool, std::vector<symbol>>>)
	 *
	 * The definition of pre-conditions and effects is consistant with that of pre-conditions
	 * and effects in an action (the symbol is a predicate name, the boolean is true when
	 * the pre-condition or effect is preceded by "not" in the PDDL file, and the
	 * std::vector<symbol> is the list of parameters (names of action's parameters).
	*/
	std::vector<symbol> params;
	std::vector<triplet<symbol, bool, std::vector<symbol>>> cond_eff_preconds, cond_eff_eff;

	dom.add_predicate("in", 1);
	dom.add_predicate("adjacent", 2);
	dom.add_predicate("hole", 1);

	dom.add_action("move");
	dom.add_action_param("move", "x");
	dom.add_action_param("move", "y");
	dom.add_action_precond("move", "in", false, {"x"});
	dom.add_action_precond("move", "adjacent", false, {"x", "y"});

	// Adding the conditional effect's pre-condition
	params.push_back("y");
	cond_eff_preconds.push_back(std::make_tuple("hole", true, params));
	params.clear();

	// Adding the conditional effect's effects
	params.push_back("y");
	cond_eff_eff.push_back(std::make_tuple("in", false, params));
	params.clear();

	params.push_back("x");
	cond_eff_eff.push_back(std::make_tuple("in", true, params));
	params.clear();

	dom.add_action_cond_effect("move", cond_eff_preconds, cond_eff_eff);

	/** GROUNDING THE PROBLEM **/
	problem prob(&dom);

	prob.add_object("A1");
	prob.add_object("A2");
	prob.add_object("A3");
	prob.add_object("A4");
	prob.add_object("A5");
	prob.add_object("B1");
	prob.add_object("B2");
	prob.add_object("B3");
	prob.add_object("B4");
	prob.add_object("B5");
	prob.add_object("C1");
	prob.add_object("C2");
	prob.add_object("C3");
	prob.add_object("C4");
	prob.add_object("C5");
	prob.add_object("D1");
	prob.add_object("D2");
	prob.add_object("D3");
	prob.add_object("D4");
	prob.add_object("D5");
	prob.add_object("E1");
	prob.add_object("E2");
	prob.add_object("E3");
	prob.add_object("E4");
	prob.add_object("E5");

	prob.ground_init("in", {"A1"});

	prob.ground_init("adjacent", {"A1", "A2"});
	prob.ground_init("adjacent", {"A1", "B1"});
	prob.ground_init("adjacent", {"A2", "A1"});
	prob.ground_init("adjacent", {"A2", "A3"});
	prob.ground_init("adjacent", {"A2", "B2"});
	prob.ground_init("adjacent", {"A3", "A2"});
	prob.ground_init("adjacent", {"A3", "A4"});
	prob.ground_init("adjacent", {"A3", "B3"});
	prob.ground_init("adjacent", {"A4", "A3"});
	prob.ground_init("adjacent", {"A4", "A5"});
	prob.ground_init("adjacent", {"A4", "B4"});
	prob.ground_init("adjacent", {"A5", "A4"});
	prob.ground_init("adjacent", {"A5", "B5"});
	prob.ground_init("adjacent", {"B1", "A1"});
	prob.ground_init("adjacent", {"B1", "B2"});
	prob.ground_init("adjacent", {"B1", "C1"});
	prob.ground_init("adjacent", {"B2", "A2"});
	prob.ground_init("adjacent", {"B2", "B1"});
	prob.ground_init("adjacent", {"B2", "B3"});
	prob.ground_init("adjacent", {"B2", "C2"});
	prob.ground_init("adjacent", {"B3", "A3"});
	prob.ground_init("adjacent", {"B3", "B2"});
	prob.ground_init("adjacent", {"B3", "B4"});
	prob.ground_init("adjacent", {"B3", "C3"});
	prob.ground_init("adjacent", {"B4", "A4"});
	prob.ground_init("adjacent", {"B4", "B3"});
	prob.ground_init("adjacent", {"B4", "B5"});
	prob.ground_init("adjacent", {"B4", "C4"});
	prob.ground_init("adjacent", {"B5", "A5"});
	prob.ground_init("adjacent", {"B5", "B4"});
	prob.ground_init("adjacent", {"B5", "C5"});
	prob.ground_init("adjacent", {"C1", "B1"});
	prob.ground_init("adjacent", {"C1", "C2"});
	prob.ground_init("adjacent", {"C1", "D1"});
	prob.ground_init("adjacent", {"C2", "B2"});
	prob.ground_init("adjacent", {"C2", "C1"});
	prob.ground_init("adjacent", {"C2", "C3"});
	prob.ground_init("adjacent", {"C2", "D2"});
	prob.ground_init("adjacent", {"C3", "B3"});
	prob.ground_init("adjacent", {"C3", "C2"});
	prob.ground_init("adjacent", {"C3", "C4"});
	prob.ground_init("adjacent", {"C3", "D3"});
	prob.ground_init("adjacent", {"C4", "B4"});
	prob.ground_init("adjacent", {"C4", "C3"});
	prob.ground_init("adjacent", {"C4", "C5"});
	prob.ground_init("adjacent", {"C4", "D4"});
	prob.ground_init("adjacent", {"C5", "B5"});
	prob.ground_init("adjacent", {"C5", "C4"});
	prob.ground_init("adjacent", {"C5", "D5"});
	prob.ground_init("adjacent", {"D1", "C1"});
	prob.ground_init("adjacent", {"D1", "D2"});
	prob.ground_init("adjacent", {"D1", "E1"});
	prob.ground_init("adjacent", {"D2", "C2"});
	prob.ground_init("adjacent", {"D2", "D1"});
	prob.ground_init("adjacent", {"D2", "D3"});
	prob.ground_init("adjacent", {"D2", "E2"});
	prob.ground_init("adjacent", {"D3", "C3"});
	prob.ground_init("adjacent", {"D3", "D2"});
	prob.ground_init("adjacent", {"D3", "D4"});
	prob.ground_init("adjacent", {"D3", "E3"});
	prob.ground_init("adjacent", {"D4", "C4"});
	prob.ground_init("adjacent", {"D4", "D3"});
	prob.ground_init("adjacent", {"D4", "D5"});
	prob.ground_init("adjacent", {"D4", "E4"});
	prob.ground_init("adjacent", {"D5", "C5"});
	prob.ground_init("adjacent", {"D5", "D4"});
	prob.ground_init("adjacent", {"D5", "E5"});
	prob.ground_init("adjacent", {"E1", "D1"});
	prob.ground_init("adjacent", {"E1", "E2"});
	prob.ground_init("adjacent", {"E2", "D2"});
	prob.ground_init("adjacent", {"E2", "E1"});
	prob.ground_init("adjacent", {"E2", "E3"});
	prob.ground_init("adjacent", {"E3", "D3"});
	prob.ground_init("adjacent", {"E3", "E2"});
	prob.ground_init("adjacent", {"E3", "E4"});
	prob.ground_init("adjacent", {"E4", "D4"});
	prob.ground_init("adjacent", {"E4", "E3"});
	prob.ground_init("adjacent", {"E4", "E5"});
	prob.ground_init("adjacent", {"E5", "D5"});
	prob.ground_init("adjacent", {"E5", "E4"});

	prob.ground_init("hole", {"A3"});
	prob.ground_init("hole", {"B1"});
	prob.ground_init("hole", {"B5"});
	prob.ground_init("hole", {"C3"});
	prob.ground_init("hole", {"D1"});
	prob.ground_init("hole", {"D5"});
	prob.ground_init("hole", {"E3"});

	prob.ground_final("in", {"E5"});

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
