#ifndef PROBLEM_HPP
#define PROBLEM_HPP

#include "../data_structures/tuple.hpp"
#include "domain.hpp"
#include "state.hpp"

#include <cassert>
#include <string>
#include <vector>

#ifndef SYMBOL
#define SYMBOL
	typedef std::string symbol
#endif

class problem
{
	private:
		/** ATTRIBUTES **/

		// The domain associated with the problem
		domain* m_domain;

		// The objects
		std::vector<symbol> m_objects;

		// The initial state
		state m_init_state;

		// The final state
		state m_final_state;

	public:

		/** METHODS **/

		// Constructors
		problem(void);
		problem(domain* dom);
		problem(const problem &prob);

		// Getters
		domain get_domain(void) const;
		const std::vector<symbol> get_objects(void) const;
		const state init_state(void) const;
		const state final_state(void) const;

		// Setter
		void set_initial(const state &other);
		void set_final(const state &other);

		// Modifiers
		void add_object(const symbol &obj);
		void ground_init (const symbol &pred, tuple<symbol> objs);
		void ground_final (const symbol &pred, tuple<symbol> objs);

		// Other
		void delete_relax(const problem &prob);
		void delete_domain(void);

};

#endif // PROBLEM_HPP
