#include "action.hpp"

action::action(const symbol &symb): m_name(symb), m_nbparams(0), m_cost(1) {}

symbol action::name(void) { return m_name; }

unsigned int action::nbparams(void) { return m_nbparams; }

unsigned int action::cost(void) { return m_cost; }

void action::set_cost(unsigned int cost) { m_cost = cost; }

void action::add_param(const symbol &param_name)
{
	assert(("Parameter name already used.", m_params.find(param_name) == m_params.end()));
	m_params.insert(std::pair<symbol, int>(param_name, m_nbparams++));
}

void action::add_precond(int precond_index, bool neg_precond, std::vector<symbol> param_names)
{
	std::vector<int> param_indexes;

	for (symbol s : param_names)
		param_indexes.push_back(param_index(s));

	m_preconds.push_back(triplet<int, bool, std::vector<int>>
		(precond_index, neg_precond, param_indexes));
}

void action::add_effect(int effect_index, bool neg_effect, std::vector<symbol> param_names)
{
	std::vector<int> param_indexes;

	for (symbol s : param_names)
		param_indexes.push_back(param_index(s));

	m_effects.push_back(triplet<int, bool, std::vector<int>>
		(effect_index, neg_effect, param_indexes));
}

void action::add_conditional_effect(const std::vector<triplet<int, bool, std::vector<symbol>>> &cond_eff_preconds,
				    const std::vector<triplet<int, bool, std::vector<symbol>>> &cond_eff_eff)
{
	std::vector<triplet<int, bool, std::vector<int>>> gd_preconds, gd_effects;
	std::vector<int> param_indexes;

	for (triplet<int, bool, std::vector<symbol>> elt : cond_eff_preconds)
	{
		for (symbol s : std::get<2>(elt))
			param_indexes.push_back(param_index(s));

		gd_preconds.push_back(std::make_tuple(std::get<0>(elt), std::get<1>(elt),
						      param_indexes));

		param_indexes.clear();
	}

	for (triplet<int, bool, std::vector<symbol>> elt : cond_eff_eff)
	{
		for (symbol s : std::get<2>(elt))
			param_indexes.push_back(param_index(s));

		gd_effects.push_back(std::make_tuple(std::get<0>(elt), std::get<1>(elt),
						     param_indexes));

		param_indexes.clear();
	}

	m_cond_effects.push_back(std::make_pair(gd_preconds, gd_effects));
}

state action::apply(const state &target, std::vector<symbol> act_params)
{
	assert(("Incorrect number of parameters.", act_params.size() == m_nbparams));

	bool fullfil_preconds = true;
	state obtained;
	std::vector<symbol> predic_params;

	for (triplet<int, bool, std::vector<int>> precond : m_preconds)
	{
		for (int i : std::get<2>(precond))
			predic_params.push_back(act_params[i]);

		if (std::get<1>(precond))
			fullfil_preconds &= !target.contains(std::get<0>(precond), predic_params);
		else
			fullfil_preconds &= target.contains(std::get<0>(precond), predic_params);

		predic_params.clear();
	}

	if (fullfil_preconds)
	{
		obtained = target;

		for (triplet<int, bool, std::vector<int>> effect : m_effects)
		{
			for (int i : std::get<2>(effect))
				predic_params.push_back(act_params[i]);

			if (std::get<1>(effect))
				obtained.erase(std::get<0>(effect), predic_params);
			else
				obtained.add(std::get<0>(effect), predic_params);

			predic_params.clear();
		}

		for (std::pair<std::vector<triplet<int, bool, std::vector<int>>>,
			       std::vector<triplet<int, bool, std::vector<int>>>> cond_eff : m_cond_effects)
		{
			fullfil_preconds = true;

			for (triplet<int, bool, std::vector<int>> precond : cond_eff.first)
			{
				for (int i : std::get<2>(precond))
					predic_params.push_back(act_params[i]);

				if (std::get<1>(precond))
					fullfil_preconds &= !target.contains(std::get<0>(precond), predic_params);
				else
					fullfil_preconds &= target.contains(std::get<0>(precond), predic_params);

				predic_params.clear();

				if (fullfil_preconds)
				{
					for (triplet<int, bool, std::vector<int>> effect : cond_eff.second)
					{
						for (int i : std::get<2>(effect))
							predic_params.push_back(act_params[i]);

						if (std::get<1>(effect))
							obtained.erase(std::get<0>(effect), predic_params);
						else
							obtained.add(std::get<0>(effect), predic_params);

						predic_params.clear();
					}
				}
			}
		}
	}

	return obtained;
}

action action::delete_relax(void)
{
	action relaxed(m_name);
	std::vector<triplet<int, bool, std::vector<int>>> positive_eff;

	relaxed.m_nbparams = m_nbparams;
	relaxed.m_cost = m_cost;
	relaxed.m_params = m_params;
	relaxed.m_preconds = m_preconds;

	// Adding only positive effects
	for (triplet<int, bool, std::vector<int>> eff : m_effects)
	{
		if (!std::get<1>(eff))
			relaxed.m_effects.push_back(eff);
	}

	// Screening all the conditional effects and copying them without the negative effects
	for (std::pair<std::vector<triplet<int, bool, std::vector<int>>>,
		       std::vector<triplet<int, bool, std::vector<int>>>> cond_eff : m_cond_effects)
	{
		// Screening the effects and copying only the positive effects
		for (triplet<int, bool, std::vector<int>> eff : cond_eff.second)
		{
			if (!std::get<1>(eff))
				positive_eff.push_back(eff);
		}
		relaxed.m_cond_effects.push_back(std::make_pair(cond_eff.first, positive_eff));
		positive_eff.clear();
	}

	return relaxed;
}

int action::param_index(const symbol &param_name)
{
	std::map<symbol, int>::iterator param = m_params.find(param_name);
	assert(("No such parameter exists.", param != m_params.end()));

	return param->second;
}
