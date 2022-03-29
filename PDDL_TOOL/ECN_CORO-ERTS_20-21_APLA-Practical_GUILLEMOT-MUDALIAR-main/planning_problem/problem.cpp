#include "problem.hpp"

problem::problem(void) : m_domain(nullptr) {}

problem::problem(domain* dom) : m_domain(dom),
				m_objects(m_domain->constants()),
				m_init_state(m_domain->state_dimensions()),
				m_final_state(m_domain->state_dimensions()) {}

problem::problem(const problem &prob) : m_domain(prob.m_domain), m_objects(prob.m_objects),
					m_init_state(prob.m_init_state),
					m_final_state(prob.m_final_state) {}

domain problem::get_domain(void) const { return *m_domain; }

const std::vector<symbol> problem::get_objects(void) const { return m_objects; }

const state problem::init_state(void) const { return m_init_state; }

const state problem::final_state(void) const { return m_final_state; }

void problem::set_initial(const state &other) { m_init_state = other; }

void problem::set_final(const state &other) { m_final_state = other; }

void problem::add_object (const symbol &obj)
{
	m_domain->add_symbol(obj);
	m_objects.push_back(obj);
}

void problem::ground_init (const symbol &pred, tuple<symbol> objs)
{
	bool exists;
	unsigned int pred_index = m_domain->pred_index(pred, objs.size());

	for (symbol s : objs)
	{
		exists = false;
		for (std::vector<symbol>::iterator it = m_objects.begin(); it < m_objects.end() && !exists; ++it)
			exists |= (s == *it);
		assert(("No such object exists.", exists));
	}

	m_init_state.add(pred_index, objs);
}

void problem::ground_final (const symbol &pred, tuple<symbol> objs)
{
	bool exists;
	unsigned int pred_index = m_domain->pred_index(pred, objs.size());

	for (symbol s : objs)
	{
		exists = false;
		for (std::vector<symbol>::iterator it = m_objects.begin(); it < m_objects.end() && !exists; ++it)
			exists |= (s == *it);
		assert(("No such object exists.", exists));
	}

	m_final_state.add(pred_index, objs);
}

void problem::delete_relax(const problem &prob)
{
	domain* dom = new domain();
	dom->delete_relax(prob.get_domain());

	m_domain = dom;
	m_objects = prob.m_objects;
	m_init_state = prob.m_init_state;
	m_final_state = prob.m_final_state;
}

void problem::delete_domain(void)
{
	delete m_domain;
}
