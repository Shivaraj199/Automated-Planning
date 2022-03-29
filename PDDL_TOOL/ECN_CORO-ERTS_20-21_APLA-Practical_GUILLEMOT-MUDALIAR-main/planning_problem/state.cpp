#include "state.hpp"

state::state(void) : m_size(0), m_gdpredicates(nullptr) {}

state::state(const std::vector<unsigned int> &statedims) : m_size(0),
							   m_dimensions(statedims)
{
	unsigned int i = 0;

	m_gdpredicates = new kdt<tuple, symbol>[m_dimensions.size()];

	for (unsigned int u : m_dimensions)
	{
		m_gdpredicates[i].set_dimension(u);
		i++;
	}
}

state::state (const state &other) : m_size(0), m_dimensions(other.m_dimensions)
{
	unsigned int i = 0;

	m_gdpredicates = new kdt<tuple, symbol>[m_dimensions.size()];

	for (unsigned int u : m_dimensions)
	{
		m_gdpredicates[i].set_dimension(u);
		i++;
	}

	for (i = 0; i < m_dimensions.size(); ++i)
	{
		for (tuple<symbol> value : other.m_gdpredicates[i])
			add(i, value);
	}
}

unsigned int state::size(void) const { return m_size; }

std::vector<unsigned int> state::dimensions(void) const { return m_dimensions; }

unsigned int state::kdt_size(unsigned int index) const
{
	unsigned int to_return = 0;

	if (index < m_dimensions.size())
		to_return = m_gdpredicates[index].size();

	return to_return;
}

void state::reset(void)
{
	unsigned int i = 0;

	if (m_gdpredicates)
	{
		m_size = 0;
		delete[] m_gdpredicates;
		m_gdpredicates = new kdt<tuple, symbol>[m_dimensions.size()];
		for (unsigned int u : m_dimensions)
		{
			m_gdpredicates[i].set_dimension(u);
			i++;
		}
	}
}

bool state::empty(void) const
{
	bool return_value = true;
	unsigned int i;

	if (m_gdpredicates)
	{
		for (i = 0; i < m_dimensions.size(); ++i)
			return_value &= !m_gdpredicates[i].empty();
	}

	return return_value;
}

bool state::contains(unsigned int index, tuple<symbol> value) const
{
	return m_gdpredicates[index].contains(value);
}

void state::add(unsigned int index, tuple<symbol> value)
{
	if(!m_gdpredicates[index].insert(value))
		m_size++;
}

void state::erase(unsigned int index, tuple<symbol> value)
{
	assert(("Error erasing predicate from state.", m_gdpredicates[index].erase(value) == 0));
	m_size--;
}

bool state::included(const state &other)
{
	bool is_included = true;
	unsigned int i;

	if (m_gdpredicates)
	{
		for (i = 0; i < m_dimensions.size() && is_included; ++i)
		{
			for (tuple<symbol> t : m_gdpredicates[i])
			{
				is_included &= other.contains(i, t);
			}
		}
	}

	return is_included;
}

tuple<symbol> state::operator[](unsigned int index) const
{
	unsigned int curr_index = index, i, total_size(0);
	tuple<symbol> to_return;

	for (i = 0; i < m_dimensions.size(); ++i)
	{
		total_size += m_gdpredicates[i].size();
		if (index < total_size)
		{
			to_return = m_gdpredicates[i][curr_index];
			break;
		}
		else
			curr_index -= m_gdpredicates[i].size();
	}

	return to_return;
}

state state::operator=(const state &other)
{
	unsigned int i = 0;

	m_dimensions.clear();
	if (m_gdpredicates)
		delete[] m_gdpredicates;

	m_size = 0;
	m_dimensions = other.m_dimensions;
	m_gdpredicates = new kdt<tuple, symbol>[m_dimensions.size()];

	for (unsigned int u : m_dimensions)
	{
		m_gdpredicates[i].set_dimension(u);
		i++;
	}

	for (i = 0; i < m_dimensions.size(); ++i)
	{
		for (tuple<symbol> value : other.m_gdpredicates[i])
			add(i, value);
	}

	return *this;
}

bool state::operator==(const state& other)
{
	bool equal = true;
	unsigned int i;

	if (m_gdpredicates)
	{
		for (i = 0; i < m_dimensions.size() && equal; ++i)
		{
			for (tuple<symbol> t : m_gdpredicates[i])
				equal &= other.contains(i, t);
		}
	}

	if (other.m_gdpredicates)
	{
		for (i = 0; i < other.m_dimensions.size() && equal; ++i)
		{
			for (tuple<symbol> t : other.m_gdpredicates[i])
				equal &= contains(i, t);
		}
	}

	return equal;
}

std::ostream& operator<<(std::ostream &os, const state &s)
{
	unsigned int curr_size(0), curr_kdt, curr_index = 0;
	tuple<symbol>::iterator it;

	for (curr_kdt = 0; curr_kdt < s.dimensions().size(); ++curr_kdt)
	{
		curr_size += s.kdt_size(curr_kdt);
		os << curr_kdt << ": ";

		for (; curr_index < curr_size; ++curr_index)
		{
			for (it = s[curr_index].begin(); it < s[curr_index].end(); ++it)
			{
				if (it == s[curr_index].begin())
					os << "(";
				else
					os << ", ";
				os << *it;
				if (it == s[curr_index].end()-1)
					os << ") ";
			}
		}
		os << "\n";
	}

	return os;
}
