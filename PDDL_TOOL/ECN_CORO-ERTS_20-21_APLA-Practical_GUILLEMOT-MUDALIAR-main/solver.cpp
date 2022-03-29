#include "solver.hpp"

bool max_count(unsigned int nb_params, unsigned int nb_objects, unsigned int *counter)
{
	return (counter[nb_params] == 1);
}

void increment_count(unsigned int nb_params, unsigned int nb_objects,
		     unsigned int *counter, unsigned int index)
{
	counter[index] = ++counter[index]%nb_objects;

	if (counter[index] == 0 && index <= nb_params)
		increment_count(nb_params, nb_objects, counter, ++index);
}

path astar(const problem &prob, heuristic h, unsigned int power)
{
	bool found = false;
	int i;
	unsigned int current_cost, heur_value, *obj_indexes;

	path p;
	state current_state, next, final_state = prob.final_state();
	std::vector<symbol> objects = prob.get_objects(), params;
	std::vector<std::pair<state, unsigned int>> waiting_list;

	/**
	 * Table of predecessors. The items in a tuple correspond to:
	 * 	- the state which is considered,
	 * 	- its predecessor,
	 *	- the cost to reach this state,
	 * 	- the action that leaded to this state.
	*/
	std::vector<std::tuple<state, state, unsigned int, std::vector<symbol>>> preds;
	std::vector<std::tuple<state, state, unsigned int, std::vector<symbol>>>::iterator preds_it;

	std::stack<state> path_states;
	std::stack<std::vector<symbol>> path_actions;

	// Initialization
	waiting_list.push_back({prob.init_state(), 0});
	preds.push_back({waiting_list.back().first, state(), 0, std::vector<symbol>()});

	// Main loop
	while (!waiting_list.empty())
	{
		current_state = waiting_list.back().first;
		waiting_list.pop_back();

		// FOR TEST PURPOSES ONLY
		if (h == critical_path)
		{
			printf("Current state:\n");
			std::cout << current_state << std::endl;
		}
		// END OF TEST

		// Finding the cost of the current state
		for (preds_it = preds.begin(); preds_it < preds.end(); ++preds_it)
		{
			if (std::get<0>(*preds_it) == current_state)
			{
				current_cost = std::get<2>(*preds_it);
				break;
			}
		}

		// Checking if we reached the final state
		if (final_state.included(current_state))
		{
			found = true;
			break;
		}

		// For each action, try to build valid states
		for (action a : prob.get_domain())
		{
			obj_indexes = new unsigned int[a.nbparams()+1];

			for (i = 0; i < a.nbparams(); ++i)
				obj_indexes[i] = 0;

			// While loop over all the possible combinations of objects
			while (!max_count(a.nbparams(), objects.size(), obj_indexes))
			{
				for (i = a.nbparams()-1; i >= 0; --i)
					params.push_back(objects[obj_indexes[i]]);

				next = a.apply(current_state, params);

				// If we found a valid state
				if (!next.empty())
				{
					heur_value = h(prob, next, power);

					// FOR TEST PURPOSES ONLY
					if (h == critical_path)
					{
						printf("Next state:\n");
						std::cout << next;
						printf("\tHeuristic value = %d\n\n", heur_value);
					}
					// END OF TEST

					params.insert(params.begin(), a.name());

					for (preds_it = preds.begin(); preds_it < preds.end(); ++preds_it)
					{
						if (std::get<0>(*preds_it) == next)
						{
							if (std::get<2>(*preds_it) > current_cost+a.cost())
							{
								*preds_it = {std::get<0>(*preds_it), state(current_state), current_cost+a.cost(), params};
								waiting_list.push_back({next, current_cost+a.cost()+heur_value});
							}
							break;
						}
					}
					if (preds_it == preds.end())
					{
						preds.push_back({next, state(current_state), current_cost+a.cost(), params});
						waiting_list.push_back({next, current_cost+a.cost()+heur_value});
					}
				}

				params.clear();
				increment_count(a.nbparams(), objects.size(), obj_indexes);
			}
		}

		std::sort(waiting_list.begin(), waiting_list.end(), greater);
	}

	// Build the path from the predecessors
	if (found)
	{
		// Finding the final state in the predecessors list
		for (preds_it = preds.begin(); !final_state.included(std::get<0>(*preds_it));
			++preds_it);

		std::get<2>(p) = std::get<2>(*preds_it);

		while (!std::get<1>(*preds_it).empty())
		{
			path_states.push(std::get<0>(*preds_it));
			path_actions.push(std::get<3>(*preds_it));

			current_state = std::get<1>(*preds_it);

			for (preds_it = preds.begin(); preds_it < preds.end(); ++preds_it)
			{
				if (std::get<0>(*preds_it) == current_state)
					break;
			}
		}
		path_states.push(std::get<0>(*preds_it));

		while (!path_states.empty())
		{
			std::get<0>(p).push_back(path_states.top());
			path_states.pop();
		}

		while (!path_actions.empty())
		{
			std::get<1>(p).push_back(path_actions.top());
			path_actions.pop();
		}
	}

	return p;
}

unsigned int zero_heuristic(const problem &prob, const state &init, unsigned int power)
{
	return 0;
}

unsigned int delete_relaxation(const problem &prob, const state &init, unsigned int power)
{
	path p;
	problem relaxed_prob;

	/**
	 * Eliminating all negative effects of the initial problem and the negative effects
	 * in the conditional effects
	*/
	relaxed_prob.delete_relax(prob);

	relaxed_prob.set_initial(init);

	// Solving the relaxed problem using Dijkstra
	p = astar(relaxed_prob, zero_heuristic);

	relaxed_prob.delete_domain();

	// Returning the total cost
	return std::get<2>(p);
}

unsigned int critical_path(const problem &prob, const state &init, unsigned int power)
{
	bool updated;
	int i, j;
	path p;
	problem new_p(prob);
	state s(prob.get_domain().state_dimensions());
	unsigned int total_size, pred_indexes[power], h_max(0);

	assert(("Cannot use critical_path heuristic with the current power value, not enough predicate in the final state to create a subset of predicate of this size.", power <= prob.final_state().size()));

	// Initializing the predicate indexes to the first power-size combination
	for (i = 0; i < power; ++i)
		pred_indexes[i] = i;

	// Initializing the initial state
	new_p.set_initial(init);

	while (pred_indexes[0] < prob.final_state().size()-power
	       || pred_indexes[power-1] <= prob.final_state().size()-1)
	{
		// Initializing the final state
		for (i = 0; i < power; ++i)
		{
			// Finding the index of the KD-tree to add the predicate
			total_size = 0;

			for (j = 0; j < prob.final_state().dimensions().size(); ++j)
			{
				total_size += prob.final_state().kdt_size(j);
				if (pred_indexes[i] < total_size)
				{
					s.add(j, prob.final_state()[pred_indexes[i]]);
					break;
				}
			}
		}
		new_p.set_final(s);

		// Solving the new problem with Dijkstra
		p = astar(new_p, zero_heuristic);

		// Saving the heuristic value if greater than h_max
		if (std::get<2>(p) > h_max)
			h_max = std::get<2>(p);

		// Emptying the path
		std::get<0>(p).clear();
		std::get<1>(p).clear();
		std::get<2>(p) = 0;

		// Emptying the final state
		s.reset();

		// Updating the indexes
		for(i = 0; i < power; ++i)
		{
			if (i < power-1 && pred_indexes[i+1] == prob.final_state().size()-power+i+1)
			{
				pred_indexes[i]++;
				for (j = i+1; j < power; ++j)
					pred_indexes[j] = pred_indexes[j-1]+1;
				break;
			}
			else if (i == power-1)
				pred_indexes[i]++;
		}
	}

	return h_max;
}
