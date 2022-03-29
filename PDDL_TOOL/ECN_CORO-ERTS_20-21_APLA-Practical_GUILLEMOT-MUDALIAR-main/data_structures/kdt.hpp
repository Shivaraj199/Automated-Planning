#ifndef KDT_HPP
#define KDT_HPP

#include <initializer_list>
#include <iterator>

template<template<typename> typename T, typename U> class kdt
{
	public:
		kdt(void): m_dimension(0), m_size(0), m_tree(nullptr) {}
		kdt(unsigned int dimension): m_dimension(dimension), m_size(0), m_tree(nullptr) {}

		unsigned int size(void) { return m_size; }

		void set_dimension(unsigned int dimension)
		{
			m_dimension = dimension;
			if (m_tree)
				delete m_tree;
		}

		bool empty(void) { return !m_tree; }

		int insert(const T<U> &value) { return insert(nullptr, m_tree, 0, value); }
		int insert(std::initializer_list<U> value) { return insert(T<U>(value)); }

		bool contains(T<U> value) { return contains(m_tree, 0, value); }
		bool contains(std::initializer_list<U> value) { return contains(T<U>(value)); }

		const T<U>* query(T<U> value) { return query(m_tree, value); }

		int erase(T<U> value) { return erase(m_tree, 0, value); }
		int erase(std::initializer_list<U> value) { return erase(T<U>(value)); }

		class kdtnode
		{
			public:
				// Attributes
				T<U> m_value;
				kdtnode *m_parent;
				kdtnode *m_left;
				kdtnode *m_right;

				// Methods
				kdtnode(kdtnode* parent, const T<U> &value):
					m_value(value), m_parent(parent),
					m_left(nullptr), m_right(nullptr) {}
		};

		class iterator: public std::iterator<std::input_iterator_tag, T<U>>
		{
			private:
				kdtnode* m_ptr;

			public:
				using difference_type = typename std::iterator<std::input_iterator_tag, T<U>>::difference_type;

				iterator(void) : m_ptr(nullptr) {}
				iterator(kdtnode* node) : m_ptr(node) {}
				inline kdtnode& operator=(const kdtnode& node) { return node; }
				inline bool operator==(const iterator& rhs) const { return m_ptr==rhs.m_ptr; }
				inline bool operator!=(const iterator& rhs) const { return m_ptr!=rhs.m_ptr; }
				inline T<U>& operator*(void) const { return m_ptr->m_value; }
				inline T<U>* operator->(void) const { return *(m_ptr->m_value); }

				iterator& operator++(void)
				{
					if (m_ptr->m_right != 0)
					{
						m_ptr = m_ptr->m_right;
						while (m_ptr->m_left != 0)
							m_ptr = m_ptr->m_left;
					}
					else
					{
						kdtnode* parent = m_ptr->m_parent;
						while (parent != 0 && m_ptr == parent->m_right)
						{
							m_ptr = parent;
							parent = parent->m_parent;
						}
						if (m_ptr != parent)
							m_ptr = parent;
					}
					return *this;
				}

				iterator operator++(int) { iterator tmp(*this); ++m_ptr; return tmp; }
		};

		iterator begin(void) { return iterator(min(m_tree)); }

		iterator end(void) { return iterator(nullptr); }

		T<U> operator[](unsigned int index)
		{
			unsigned int i;
			iterator it = begin();

			for (i = 0; i < index; ++i)
				++it;

			return *it;
		}

	private:
		unsigned int m_dimension;
		unsigned int m_size;
		kdtnode *m_tree;

		kdtnode* min(kdtnode* tree)
		{
			kdtnode* return_value = tree;

			if (return_value)
			{
				while(return_value->m_left != nullptr)
					return_value = return_value->m_left;
			}

			return return_value;
		}

		kdtnode* max(kdtnode* tree)
		{
			kdtnode* return_value = tree;

			if (return_value)
			{
				while(return_value->m_right != nullptr)
					return_value = return_value->m_right;
			}

			return return_value;
		}

		int insert(kdtnode* prev_node, kdtnode* &tree, unsigned int dimension, const T<U> &value)
		{
			int return_value = 1;

			if (tree == nullptr)
			{
				tree = new kdtnode(prev_node, value);
				if (tree)
				{
					m_size++;
					return_value = 0;
				}
			}
			else if (value[dimension] == tree->m_value[dimension]
				 && value != tree->m_value)
				return_value = insert(tree, tree->m_left, ++dimension%m_dimension, value);
			else if (value[dimension] < tree->m_value[dimension])
				return_value = insert(tree, tree->m_left, ++dimension%m_dimension, value);
			else if (value[dimension] > tree->m_value[dimension])
				return_value = insert(tree, tree->m_right, ++dimension%m_dimension, value);

			return return_value;
		}

		bool contains(const kdtnode* tree, unsigned int dimension, const T<U> &value)
		{
			bool return_value = false;

			if (tree)
			{
				if (value == tree->m_value)
					return_value = true;
				else if (value[dimension] <= tree->m_value[dimension])
					return_value = contains(tree->m_left,
								++dimension%m_dimension,
								value);
				else
					return_value = contains(tree->m_right,
								++dimension%m_dimension,
								value);
			}

			return return_value;
		}

		int erase(kdtnode* &tree, unsigned int dimension, const T<U> &value)
		{
			kdtnode* to_remove;
			int return_value = 1;

			if (tree != nullptr)
			{
				if (value[dimension] == tree->m_value[dimension]
				    && value != tree->m_value)
					return_value = erase(tree->m_left,
							     ++dimension%m_dimension,
							     value);
				else if (value[dimension] < tree->m_value[dimension])
					return_value = erase(tree->m_left,
							     ++dimension%m_dimension,
							     value);
				else if (value[dimension] > tree->m_value[dimension])
					return_value = erase(tree->m_right,
							     ++dimension%m_dimension,
							     value);
				else
				{
					return_value = 0;
					to_remove = tree;
					m_size--;
					if (tree->m_left == nullptr)
					{
						tree = tree->m_right;
						if (tree)
							tree->m_parent = to_remove->m_parent;
					}
					else if (tree->m_right == nullptr)
					{
						tree = tree->m_left;
						if (tree)
							tree->m_parent = to_remove->m_parent;
					}
					else
					{
						tree = max(tree->m_left);
						tree->m_parent->m_right = tree->m_left;
						if (tree->m_left)
							tree->m_left->m_parent = tree->m_parent;

						tree->m_parent = to_remove->m_parent;
						tree->m_left = to_remove->m_left;
						tree->m_left->m_parent = tree;
						tree->m_right = to_remove->m_right;
						tree->m_right->m_parent = tree;
					}
					delete to_remove;
				}
			}

			return return_value;
		}
};

#endif // KDT_HPP
