#include "domain.hpp"

domain::domain(void) {}

domain::domain(const symbol &name) : m_name(name)
{
	m_symbols.insert(m_name);
	m_statedims.push_back(1);
}

domain::~domain(void) {}

symbol domain::name(void) { return m_name; }

std::vector<symbol> domain::constants(void) { return m_constants; }

std::vector<unsigned int> domain::state_dimensions(void) { return m_statedims; }

unsigned int domain::pred_index(const symbol &predic_name, unsigned int predic_nbparams)
{
	std::map<symbol, std::pair<int, int>>::iterator predic = m_predicates.find(predic_name);
	assert(("No such predicate exists.", predic != m_predicates.end()));
	assert(("Wrong number of parameters for this predicate.",
			predic_nbparams == predic->second.first));
	return predic->second.second;
}

bool domain::is_symbol(const symbol &symb)
{
	return m_symbols.find(symb) != m_symbols.end();
}

void domain::add_symbol(const symbol &symb)
{
	assert(("Symbol already exists.", m_symbols.find(symb) == m_symbols.end()));
	m_symbols.insert(symb);
}

void domain::add_constant(const symbol &constant)
{
	assert(("Symbol already exists.", m_symbols.find(constant) == m_symbols.end()));
	m_constants.push_back(constant);
}

void domain::add_predicate(const symbol &predic_name, unsigned int nbparams)
{
	assert(("Symbol already exists.", m_symbols.find(predic_name) == m_symbols.end()));

	m_symbols.insert(predic_name);

	if (nbparams > 0)
	{
		m_statedims.push_back(nbparams);
		m_predicates.insert(std::pair<symbol, std::pair<int, int>>
			(predic_name, std::pair<int, int>(nbparams, m_statedims.size()-1)));
	}
	else
		m_predicates.insert(std::pair<symbol, std::pair<int, int>>
			(predic_name, std::pair<int, int>(nbparams, 0)));
}

void domain::add_action(const symbol &act_name)
{
	assert(("Symbol already exists.", m_symbols.find(act_name) == m_symbols.end()));
	m_symbols.insert(act_name);
	m_actions.insert(std::pair<symbol, action>(act_name, action(act_name)));
}

void domain::set_action_cost(const symbol &act_name, unsigned int cost)
{
	std::map<symbol, action>::iterator act = m_actions.find(act_name);
	assert(("No such action exists.", act != m_actions.end()));
	act->second.set_cost(cost);
}

void domain::add_action_param(const symbol &act_name, const symbol &param_name)
{
	std::map<symbol, action>::iterator act = m_actions.find(act_name);
	assert(("No such action exists.", act != m_actions.end()));
	act->second.add_param(param_name);
}

void domain::add_action_precond(const symbol &act_name, const symbol &precond_name,
				bool neg_precond, const std::vector<symbol> &param_names)
{
	std::map<symbol, action>::iterator act = m_actions.find(act_name);
	std::map<symbol, std::pair<int, int>>::iterator pred = m_predicates.find(precond_name);
	assert(("No such action exists.", act != m_actions.end()));
	assert(("No such predicate exists.", pred != m_predicates.end()));
	assert(("Wrong number of parameters for this predicate.",
		param_names.size() == pred->second.first));
	act->second.add_precond(pred->second.second, neg_precond, param_names);
}

void domain::add_action_effect(const symbol &act_name, const symbol &effect_name,
			       bool neg_effect, const std::vector<symbol> &param_names)
{
	std::map<symbol, action>::iterator act = m_actions.find(act_name);
	std::map<symbol, std::pair<int, int>>::iterator pred = m_predicates.find(effect_name);
	assert(("No such action exists.", act != m_actions.end()));
	assert(("No such predicate exists.", pred != m_predicates.end()));
	assert(("Wrong number of parameters for this predicate.",
		param_names.size() == pred->second.first));
	act->second.add_effect(pred->second.second, neg_effect, param_names);
}

void domain::add_action_cond_effect(const symbol &act_name,
	const std::vector<triplet<symbol, bool, std::vector<symbol>>> &cond_eff_preconds,
	const std::vector<triplet<symbol, bool, std::vector<symbol>>> &cond_eff_eff)
{
	std::map<symbol, action>::iterator act = m_actions.find(act_name);
	std::map<symbol, std::pair<int, int>>::iterator pred;
	std::vector<triplet<int, bool, std::vector<symbol>>> preconds;
	std::vector<triplet<int, bool, std::vector<symbol>>> effects;

	assert(("No such action exists.", act != m_actions.end()));

	for (triplet<symbol, bool, std::vector<symbol>> elt : cond_eff_preconds)
	{
		pred = m_predicates.find(std::get<0>(elt));
		assert(("No such predicate exists.", pred != m_predicates.end()));
		assert(("Wrong number of parameters for this predicate.",
			std::get<2>(elt).size() == pred->second.first));
		preconds.push_back(std::make_tuple(pred->second.second, std::get<1>(elt),
				   std::get<2>(elt)));
	}

	for (triplet<symbol, bool, std::vector<symbol>> elt : cond_eff_eff)
	{
		pred = m_predicates.find(std::get<0>(elt));
		assert(("No such predicate exists.", pred != m_predicates.end()));
		assert(("Wrong number of parameters for this predicate.",
			std::get<2>(elt).size() == pred->second.first));
		effects.push_back(std::make_tuple(pred->second.second, std::get<1>(elt),
				  std::get<2>(elt)));
	}

	act->second.add_conditional_effect(preconds, effects);
}

void domain::delete_relax(const domain &dom)
{
	m_name = dom.m_name;
	m_symbols = dom.m_symbols;
	m_predicates = dom.m_predicates;

	for (std::pair<symbol, action> p : dom.m_actions)
		m_actions.insert(std::make_pair(p.first, p.second.delete_relax()));

	m_statedims = dom.m_statedims;
}
