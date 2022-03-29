#ifndef ACTION_HPP
#define ACTION_HPP

#include "state.hpp"

#include <cassert>
#include <map>
#include <string>
#include <tuple>
#include <vector>

#ifndef SYMBOL
#define SYMBOL
	typedef std::string symbol;
#endif

template<typename T1, typename T2, typename T3> using triplet =
	std::tuple<T1, T2, T3>;

class action
{
	private:
		/** ATTRIBUTES **/
		symbol m_name;
		unsigned int m_nbparams;
		unsigned int m_cost;
		std::map<symbol, int> m_params;
		std::vector<triplet<int, bool, std::vector<int>>> m_preconds;
		std::vector<triplet<int, bool, std::vector<int>>> m_effects;
		std::vector<std::pair<std::vector<triplet<int, bool, std::vector<int>>>,
				      std::vector<triplet<int, bool, std::vector<int>>>>> m_cond_effects;

		/** METHOD **/
		int param_index(const symbol &param_name);

	public:
		/** METHODS **/

		// Constructor
		action(const symbol &symb);

		// Getters
		symbol name(void);
		unsigned int nbparams(void);
		unsigned int cost(void);

		// Setter
		void set_cost(unsigned int cost);

		// Modifiers
		void add_param(const symbol &param_name);
		void add_precond(int precond_index, bool neg_precond,
			std::vector<symbol> param_names);
		void add_effect(int effect_index, bool neg_effect,
			std::vector<symbol> param_names);
		void add_conditional_effect(const std::vector<triplet<int, bool, std::vector<symbol>>> &cond_eff_preconds,
					    const std::vector<triplet<int, bool, std::vector<symbol>>> &cond_eff_eff);

		// Others
		state apply(const state &target, std::vector<symbol> act_params);
		action delete_relax(void);
};

#endif // ACTION_HPP
