#ifndef TUPLE_HPP
#define TUPLE_HPP

#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <vector>

template<typename T> class tuple
{
	public:
		/** METHODS **/

		// Constructors
		tuple(void): m_dimension(0), m_values(nullptr) {}

		tuple(std::initializer_list<T> values): m_dimension(values.size())
		{
			m_values = new T[m_dimension];
			std::copy(values.begin(), values.end(), m_values);
		}

		tuple(std::vector<T> values): m_dimension(values.size())
		{
			m_values = new T[m_dimension];
			std::copy(values.begin(), values.end(), m_values);
		}

		// Getter
		unsigned int size(void) const { return m_dimension; }

		// Operators
		tuple operator=(std::initializer_list<T> values)
		{
			if (m_values)
				delete[] m_values;

			m_dimension = values.size();
			m_values = new T[m_dimension];
			std::copy(values.begin(), values.end(), m_values);

			return *this;
		}

		const T operator[](unsigned int index) const { return m_values[index]; }

		bool operator==(const tuple &t) const
		{
			bool return_value = (t.size()==m_dimension);
			unsigned int i;

			if (return_value)
			{
				for (i = 0; i < m_dimension; ++i)
					return_value &= (m_values[i] == t[i]);
			}

			return return_value;
		}

		bool operator!=(const tuple &t) const { return !(*this == t); }

		/** ITERATOR **/
		class iterator: public std::iterator<
				std::random_access_iterator_tag,
				T>
		{
			public:
				using difference_type = typename std::iterator<std::random_access_iterator_tag, T>::difference_type;

				iterator(void) : m_ptr(nullptr) {}
				iterator(T* rhs) : m_ptr(rhs) {}
				inline iterator& operator+=(difference_type rhs) { m_ptr += rhs; return *this; }
				inline iterator& operator-=(difference_type rhs) { m_ptr -= rhs; return *this; }
				inline T& operator*(void) const { return *m_ptr; }
				inline T* operator->(void) const { return m_ptr; }
				inline T& operator[](difference_type rhs) const { return m_ptr[rhs]; }

				inline iterator& operator++(void) { ++m_ptr; return *this; }
				inline iterator& operator--(void) { --m_ptr; return *this; }
				inline iterator operator++(int) const { iterator tmp(*this); ++m_ptr; return tmp; }
				inline iterator operator--(int) const { iterator tmp(*this); --m_ptr; return tmp; }
				inline difference_type operator-(const iterator& rhs) const { return iterator(m_ptr-rhs.ptr); }
				inline iterator operator+(difference_type rhs) const { return iterator(m_ptr+rhs); }
				inline iterator operator-(difference_type rhs) const { return iterator(m_ptr-rhs); }
				friend inline iterator operator+(difference_type lhs, const iterator& rhs) { return iterator(lhs+rhs.m_ptr); }
				friend inline iterator operator-(difference_type lhs, const iterator& rhs) { return iterator(lhs-rhs.m_ptr); }

				inline bool operator==(const iterator& rhs) const { return m_ptr == rhs.m_ptr; }
				inline bool operator!=(const iterator& rhs) const { return m_ptr != rhs.m_ptr; }
				inline bool operator<(const iterator& rhs) const { return m_ptr < rhs.m_ptr; }
				inline bool operator>(const iterator& rhs) const { return m_ptr > rhs.m_ptr; }
				inline bool operator<=(const iterator& rhs) const { return m_ptr <= rhs.m_ptr; }
				inline bool operator>=(const iterator& rhs) const { return m_ptr >= rhs.m_ptr; }

			private:
				T* m_ptr;
		};

		iterator begin(void) { return iterator(m_values); }
		iterator end(void) { return iterator(m_values+m_dimension); }

	private:
		/** ATTRIBUTES **/
		unsigned int m_dimension;
		T* m_values;
};

#endif // TUPLE_HPP
