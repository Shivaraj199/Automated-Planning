#include "planning_problem/domain.hpp"
#include "planning_problem/problem.hpp"
#include "parser.hpp"
#include "solver.hpp"

#include <cassert>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

#define DOMAIN_FILE "../PDDL_FILES/blocksworld/blocksworld.pddl"
#define PROBLEM_FILE "../PDDL_FILES/blocksworld/pb1.pddl"

int main(void)
{
	/** DOMAIN PARSING **/

	std::ifstream input_file(DOMAIN_FILE);
	assert(("Domain file could not be opened correctly.\n", input_file.is_open()));

	Parser domain_parser(input_file);

	domain dom(domain_parser.domainName);

	for (std::pair<std::string, int> pred : domain_parser.predicateList)
	{
		dom.add_predicate(pred.first, pred.second);
	}

	for (std::string action_name : domain_parser.actionList)
	{

		dom.add_action(action_name);
	}
	for (auto constants : problem_parser.constList)
	{
  		dom.add_constant(constants);
	}
	for (std::pair<std::string, int> action_cost : domain_parser.actionCostList)
	{

		dom.set_action_cost(action_cost.first, action_cost.second);
	}
	for (std::pair<std::string, std::string> action_param : domain_parser.parameterList)
	{

		dom.add_action_param(action_param.first, action_param.second);
	}

	for (std::tuple<std::string, std::string, bool, std::vector<std::string>> precond :
		domain_parser.precondList)
	{
		dom.add_action_precond(std::get<0>(precond), std::get<1>(precond),
		                      	std::get<2>(precond), std::get<3>(precond));
	}

	for (std::tuple<std::string, std::string, bool, std::vector<std::string>> effect :
		domain_parser.effectList)
	{

		dom.add_action_effect(std::get<0>(effect), std::get<1>(effect), std::get<2>(effect),
			                    std::get<3>(effect));
	}

	  for (std::tuple <std::string,
 		const std::vector<tuple<std::string, bool, std::vector<std::string>>> ,
 		const std::vector<tuple<std::string, bool, std::vector<std::string>>>    > conditional_effect :domain_parser.condiList){

			dom.add_action_cond_effect(std::get<0>(conditional_effect), std::get<1>(conditional_effect), std::get<2>(conditional_effect));
		}







	input_file.close();









	input_file.open(PROBLEM_FILE);
	assert(("Problem file could not be opened correctly.\n", input_file.is_open()));
	Parser problem_parser(input_file);
	problem prob(&dom);

	for (auto object : problem_parser.objectList)
	{
  		prob.add_object(object);
	}

	for (std::tuple<std::string, std::vector<std::string>> init : problem_parser.initList)
	{
		prob.ground_init(std::get<0>(init),{std::get<1>(init)});
	}

	for (std::tuple<std::string, std::vector<std::string>> goal : problem_parser.goalList)
	{
	     prob.ground_final(std::get<0>(goal),{std::get<1>(goal)});
	}

	input_file.close();

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
