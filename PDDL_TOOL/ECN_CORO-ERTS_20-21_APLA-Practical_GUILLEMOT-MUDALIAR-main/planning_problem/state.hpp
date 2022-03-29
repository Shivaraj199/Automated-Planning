#ifndef STATE_HPP
#define STATE_HPP

#include "../data_structures/kdt.hpp"
#include "../data_structures/tuple.hpp"

#include <cassert>
#include <map>
#include <string>
#include <vector>

#ifndef SYMBOL
#define SYMBOL
	typedef std::string symbol;
#endif

class state
{
	private:
		/** ATTRIBUTES **/

		/**
		 * Size of the state
		 * Number of predicates in all the KD-trees
		*/
		unsigned int m_size;

		/**
		 * The dimensions of the KD-trees to store the grounded predicates.
		 * The first dimension is 1 to store the 0-arity predicates and the
		 * other correspond to the number of parameters of the associated
		 * predicate.
		*/
		std::vector<unsigned int> m_dimensions;

		// Array of KD-trees to store the grounded predicates
		kdt<tuple, symbol>* m_gdpredicates;

	public:
		/** METHODS **/

		// Constructors

		/**
		 * Default constructor creating the empty state.
		 * The empty state is returned when trying to apply an action on a state
		 * which doesnt fullfil the pre-conditions of the action.
		*/
		state(void);

		/**
		 * Builds a state with empty KD-trees initialized to the proper dimensions.
		*/
		state(const std::vector<unsigned int> &statedims);

		/**
		 * Copy constructor
		*/
		state(const state &other);

		// Getter
		unsigned int size(void) const;
		std::vector<unsigned int> dimensions(void) const;
		unsigned int kdt_size(unsigned int index) const;

		// Others
		void reset(void);
		bool empty(void) const;
		bool contains(unsigned int index, tuple<symbol> value) const;
		void add(unsigned int index, tuple<symbol> value);
		void erase(unsigned int index, tuple<symbol> value);
		bool included(const state &other);

		/** OPERATOR **/
		tuple<symbol> operator[](unsigned int index) const;
		state operator=(const state &other);
		bool operator==(const state &other);
};

std::ostream& operator<<(std::ostream &os, const state &s);

#endif // STATE_HPP
