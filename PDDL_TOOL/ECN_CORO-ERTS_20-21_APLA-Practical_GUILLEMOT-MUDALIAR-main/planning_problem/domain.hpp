#ifndef DOMAIN_HPP
#define DOMAIN_HPP

#include "action.hpp"

#include <cassert>
#include <initializer_list>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#ifndef SYMBOL
#define SYMBOL
	typedef std::string symbol;
#endif

class domain
{
	private:
		/** ATTRIBUTES **/

		// Name of the domain
		symbol m_name;

		// Symbols of the domain
		std::set<symbol> m_symbols;

		// Constants of the domain
		std::vector<symbol> m_constants;

		/**
		 * Predicates of the domain
		 * The two integers in the pair correspond to:
		 *	- the number of parameters of the predicate
		 *	- the index where it should be added/deleted in a state's KD-trees array
		*/
		std::map<symbol, std::pair<int, int>> m_predicates;

		// Actions of the domain
		std::map<symbol, action> m_actions;

		/**
		 * Dimensions of the KD-trees in a state
		 * Corresponds to the number of parameters of the predicate with the corresponding
		 * index in the std::map m_predicates.
		 * The first dimension will always be 1 to store the 0-arity predicates
		*/
		std::vector<unsigned int> m_statedims;

	public:
		/** METHODS **/

		// Constructor
		domain(void);
		domain(const symbol &name);

		// Destructor
		~domain(void);

		// Getters
		symbol name(void);
		std::vector<symbol> constants(void);
		std::vector<unsigned int> state_dimensions(void);
		unsigned int pred_index(const symbol &predic_name, unsigned int predic_nbparams);
		bool is_symbol(const symbol &symb);

		// Modifiers
		void add_symbol(const symbol &symb);
		void add_constant(const symbol &constant);
		void add_predicate(const symbol &predic_name, unsigned int nbparams);
		void add_action(const symbol &act_name);
		void set_action_cost(const symbol &act_name, unsigned int cost);
		void add_action_param(const symbol &act_name, const symbol &param_name);

		void add_action_precond(const symbol &act_name, const symbol &precond_name,
				bool neg_precond, const std::vector<symbol> &param_names);

		void add_action_effect(const symbol &act_name, const symbol &effect_name,
				bool neg_effect, const std::vector<symbol> &param_names);

		void add_action_cond_effect(const symbol &act_name,
			const std::vector<triplet<symbol, bool, std::vector<symbol>>> &cond_eff_preconds,
			const std::vector<triplet<symbol, bool, std::vector<symbol>>> &cond_eff_eff);

		// Other
		void delete_relax(const domain &dom);

		/** ITERATOR **/
		class iterator: public std::iterator<std::input_iterator_tag, std::pair<symbol, action>>
		{
			public:
				using difference_type = typename std::iterator<std::input_iterator_tag, std::pair<symbol, action>>::difference_type;

				iterator(const std::map<symbol, action>::iterator &other): m_it(other) {}
				inline iterator operator=(const iterator &other) { return other; }
				inline bool operator==(const iterator &other) const { return m_it == other.m_it; }
				inline bool operator!=(const iterator &other) const { return m_it != other.m_it; }
				inline action& operator*(void) const { return m_it->second; }
				inline action operator->(void) const { return m_it->second; }

				inline iterator operator++(void) { ++m_it; return *this; }
				inline iterator operator++(int) { iterator tmp(*this); ++m_it; return tmp; }

			private:
				std::map<symbol, action>::iterator m_it;
		};

		iterator begin(void) { return m_actions.begin(); }
		iterator end(void) { return m_actions.end(); }
};

#endif // DOMAIN_HPP
