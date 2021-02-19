/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *																	*
 *				Copyright 2021 Sawyer Porter						*
 *																    *
 * Licensed under the Apache License, Version 2.0 (the "License");  *
 * you may not use this file except in compliance with the License. *
 * You may obtain a copy of the License at							*
 * http://www.apache.org/licenses/LICENSE-2.0						*
 *																	*
 * Unless required by applicable law or agreed to in writing,       *
 * software distributed under the License is distributed on an      *
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,     *
 * either express or implied.See the License for the specific       *
 * language governing permissionsand limitations under the License. *
 *																	*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * 
 * // vvv READ THIS vvv //
 * 
 * THE CHALLENGE:
 * use no includes, even those 
 * from the standerd library
 * 
 * if you want to define to change 
 * the namespace name, do:
 * #define DYNM_ARR_NAMESPACE_NAME [enter the namespace name here]
 * 
 * by defualt it is "kubic"
 * 
 * you should use the std::vector<> class
 * for dynamic arrays(as it may be faster),
 * and can be constructed with arrays, for example: std::vector<int> foo = { 0, 1, 2 }.
 * 
 * this impl should be completly
 * compatible the std::vector<> class
 * 
 * all function/method definitions will be 
 * commented :D
 * 
 * // ----------------- //
 * 
*/

#pragma once
#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#ifndef DYNM_ARR_NAMESPACE_NAME
#define DYNM_ARR_NAMESPACE_NAME kubic
#endif

namespace DYNM_ARR_NAMESPACE_NAME
{
	class error
	{
	public:
		error(const char* error)
		{
			what = error;
		}

		const char* what = "";
	};

	template<typename type>
	struct _table
	{
		// amount of elements
		size_t size = 0;
		
		// amount of space allocated in <type> not in bytes
		size_t capacity = 0;

		// array
		type* elements = nullptr;
	};

	// class impl of a dynamic array
	template<typename type>
	class dynamic_array
	{
	public: // constructors 
		dynamic_array();

		dynamic_array(const type* begin, const type* end);

		~dynamic_array();

	public: // iterators
		typedef type* iterator;
		typedef const type* const_iterator;

		iterator begin() { return (iterator)&table.elements[0];  }
		iterator end() { return (iterator)&table.elements[table.size]; }

		const_iterator begin() const { return (iterator)&table.elements[0]; }
		const_iterator end() const { return (const_iterator)&table.elements[table.size]; }

	public: // methods
		_table<type> get_table() const;

		type& get(size_t index);

		// gets the size
		size_t size();

		// gets the capacity
		size_t capacity();

		// gets the mem address of first element
		type* data();
		
		// clears everything inside of the array
		void clear();

		// returns true if the array is empty
		bool empty();

		// put an element on the back of the array
		void push_back(const type& element);

		// erase the last element
		void pop_back();

		// allocate more space for to use additional elements
		void reserve(size_t amount);

		// resize to fit so it will remove any unused space(table.size == table.capacity)
		void shrink_to_fit();

		// erase a part of the array
		void erase(size_t begin_index, size_t end_index);

		// erase a part of the array
		void erase(const_iterator begin, const_iterator end);

		// insert an array into the dynamic array
		void insert(size_t where_, iterator begin, iterator end);

		// move all elements into a new table; move table(member) into table(argrument) 
		void _move_elements_into(_table<type>& table);

		// move all elements from a new table; move table(argrument) into table(member)
		void _move_elements_from(_table<type>& table);

	public: // operators
		type& operator[](size_t index);


	private: // members
		_table<type> table;

	};

	// // constructors \\ \\

	template<typename type>
	dynamic_array<type>::dynamic_array()
	{
		clear();
	}

	template<typename type>
	dynamic_array<type>::dynamic_array(const type* begin, const type* end)
	{
		clear();
		for (type* i = begin; i != end; i++)
			push_back(*i);
	}

	template<typename type>
	dynamic_array<type>::~dynamic_array()
	{
		clear();
	}

	// // methods \\ \\

	template<typename type>
	_table<type> dynamic_array<type>::get_table() const
	{
		return table;
	}

	template<typename type>
	type& dynamic_array<type>::get(size_t index_a)
	{
		if (index_a >= table.size)
			throw error("invalid index; index_a is more then or equal to this->table.size");

		return table.elements[index_a];
	}

	template<typename type>
	size_t dynamic_array<type>::size()
	{
		return table.size;
	}

	template<typename type>
	size_t dynamic_array<type>::capacity()
	{
		return table.capacity;
	}

	template<typename type>
	type* dynamic_array<type>::data()
	{
		return &table.elements[0];
	}

	template<typename type>
	void dynamic_array<type>::clear()
	{
		delete[] table.elements;

		table.size = 0;
		table.capacity = 0;
		table.elements = nullptr;
	}

	template<typename type>
	bool dynamic_array<type>::empty()
	{
		return (bool)table.size;
	}


	template<typename type>
	void dynamic_array<type>::push_back(const type& element_a)
	{
		// allocate 1 more element, if needed
		if(table.size+1 > table.capacity)
			reserve(1U);

		// then set the last element
		//
		// note: the last element is not at the 
		// end of the array, but rather the last element
		// is the last element that has a valid element(set by user)
		// behind it and a invalid element(random bytes) in front of it
		table.size++;
		get(table.size-1) = element_a;
	}

	template<typename type>
	void dynamic_array<type>::pop_back()
	{
		// insure that that we don't cause a invalid delete operation
		if (table.size == 0)
			throw error("invalid pop_back; table.size = 0, therefore there is nothing to pop back");

		// because deleting a part of an array(that is allocated on the heap)
		// will corrupt the heap manager, we need to instead ignore the last
		// element when doing a _move_elements_from() call by subtracting 1
		// from table.size and table_copy.size and then calling _move_elements_from()

		_table<type> table_copy = table;
		table_copy.size--;

		table.elements = new type[table.capacity];
		table.size--;
		
		_move_elements_from(table_copy);
	}

	template<typename type>
	void dynamic_array<type>::reserve(size_t amount_a)
	{
		// if we don't do this a memory leak will occur, we will not be able to access 
		// the already allocated elements.
		_table<type> table_copy = table;

		// amount + size == "amount to be allocated" + "amount already allocated" 
		table.capacity = amount_a + table.size;

		// allocate a new array of elements
		table.elements = new type[table.capacity];

		// move all old elements into the new array
		_move_elements_from(table_copy);
	}

	template<typename type>
	void dynamic_array<type>::shrink_to_fit()
	{
		// if we don't do this a memory leak will occur, we will not be able to access 
		// the already allocated elements.
		_table<type> table_copy = table;

		// allocate only what is needed
		table.elements = new type[table.size];

		// set the capacity to the amount allocated
		table.capacity = table.size;

		// then move all elements into the newly allocated array
		_move_elements_from(table_copy);
	}
	
	template<typename type>
	void dynamic_array<type>::erase(size_t begin_index_a, size_t end_index_a)
	{
		erase(&table.elements[begin_index_a], &table.elements[end_index_a]);
	}

	template<typename type>
	void dynamic_array<type>::erase(const_iterator begin_a, const_iterator end_a)
	{
		// insure that the beginning index is not more 
		// then the ending index to stop weird behaviour
		if (begin_a > end_a)
			throw error("invalid indexes; beginning index is more then ending index");

		// make a copy of the table, for keeping the old elements
		_table<type> table_copy = table;

		// allocate a new array of elements
		table.elements = new type[table.capacity];
		table.size = 0;

		// now only add the elements to the new array that are
		// not in the range of begin_index and end_index
		for (iterator i = &table_copy.elements[0]; i != &table_copy.elements[table_copy.size]; i++)
			if (!(begin_a <= i && end_a >= i))
				push_back(*i);

		delete[] table_copy.elements;
	}

	template<typename type>
	void dynamic_array<type>::insert(size_t where_a, iterator begin_a, iterator end_a)
	{
		if (where_a > table.size)
			throw error("invalid index; where_a is more then table.size");

		// make a copy of the table, for keeping the old elements
		_table<type> table_copy = table;

		// calculates how many elements are between end_a and begin_a
		size_t amount_of_elements = ((end_a - begin_a) + 1);
		
		// sets the capacity based on that
		//				|amount of elements|		|unused space|          | old elements |
		table.capacity = amount_of_elements + (table.capacity - table.size) + table.size;
		
		// sets the size based off of the amount of new elements and old
		//			|old elements|amount of elements|
		table.size = table.size + amount_of_elements;

		// allocate a new array of elements
		table.elements = new type[table.capacity];
		
		// there is probably a better way
		// to implement this

		// set the new elements first
		for (size_t i = 0; i < amount_of_elements; i++)
			table.elements[i + where_a] = *(i + begin_a);

		// then set the old elements
		size_t j = 0;
		for (size_t i = 0; i != table.size && j != table_copy.size; i++)
		{
			if (!(where_a <= i && (where_a+amount_of_elements-1) >= i))
			{
#pragma warning(push) // users on other platforms may have to change this
#pragma warning(disable : 6386)
				table.elements[i] = table_copy.elements[j];
#pragma warning(pop)
				j++;
			}
		}
	}

	template<typename type>
	void dynamic_array<type>::_move_elements_into(_table<type>& table_a)
	{
		// insure that the table handle all the elements. if( amount allocated [less then] amount of elements )
		if (table_a.capacity < table.size)
			throw error("cannot move elements; table_a does not have enough space allocated");

		// then just move all elements to the new table
		for (size_t i = 0; i != table.size; i++)
			table_a.elements[i] = table.elements[i];

		table_a.size = table.size;

		// then just remove all the old elements
		delete[]table.elements;
		table.capacity = 0;
		table.size = 0;
	}

	template<typename type>
	void dynamic_array<type>::_move_elements_from(_table<type>& table_a)
	{
		// insure that the table handle all the elements. if( amount allocated [less then] amount of elements )
		if (table.capacity < table_a.size)
			throw error("cannot move elements; this->table does not have enough space allocated");

		// then just move all elements from table_a into this->table
		for (size_t i = 0; i != table_a.size; i++)
			table.elements[i] = table_a.elements[i];

		table.size = table_a.size;

		// then just remove all the old elements
		delete[] table_a.elements;
		table_a.capacity = 0;
		table_a.size = 0;
	}

	// // operators \\ \\

	template<typename type>
	type& dynamic_array<type>::operator[](size_t index_a)
	{
		return get(index_a);
	}
}

#endif DYNAMIC_ARRAY_H