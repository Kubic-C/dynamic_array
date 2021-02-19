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
 * if you want to define to change 
 * the namespace name, do:
 * #define DYNM_ARR_NAMESPACE_NAME [enter the namespace name here]
 * 
 * by defualt it is "kubic"
 * 
 * you should use the std::vector<> class
 * for dynamic arrays(it may be faster), 
 * unless you want more control over 
 * the arrays themselves.
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

	template<typename type>
	class dynamic_array
	{
	public: // constructors 
		// default
		dynamic_array();

	public: // methods
		_table<type> get_table() const;

		type& get(size_t index);

		// put an element on the back of the array
		void push_back(const type& element);

		// erase the last element
		void pop_back();

		// allocate more space for to use additional elements
		void reserve(size_t amount);

		// resize to fit so it will remove any unused space(table.size == table.capacity)
		void resize_to_fit();

		// move all elements into a new table; move table(member) into table(argrument) 
		void _move_elements_into(_table<type>& table);

		// move all elements from a new table; move table(argrument) into table(member)
		void _move_elements_from(_table<type>& table);

	public: // operators
		type& operator[](size_t index);

	public: // iterators
		typedef type* iterator;
		typedef const type* const_iterator;

		iterator begin();
		iterator end();

		const_iterator begin() const;
		const_iterator end() const;

	private: // members
		_table<type> table;

	};

	// // constructors \\ \\

	template<typename type>
	dynamic_array<type>::dynamic_array()
		: table()
	{
		// allocate zero elements
		table.elements = new type[0];
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
		if (index_a > table.size)
			throw error("invalid index; index_a is more then this->table.size");

		if (index_a < 0)
			index_a = table.size - (-index_a);

		return table.elements[index_a];
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
		*end() = element_a;
		table.size++;
	}

	template<typename type>
	void dynamic_array<type>::pop_back()
	{
		// insure that that we don't cause a invalid delete operation
		if (table.size == 0)
			throw error("invalid pop_back; table.size = 0, therefore there is nothing to pop back");

		delete end();
		table.size--;
	}

	template<typename type>
	void dynamic_array<type>::reserve(size_t amount_a)
	{
		// amount + size == "amount to be allocated" + "amount already allocated" 
		table.capacity = amount_a + size;

		// if we don't do this a memory leak will occur, we will not be able to access 
		// the already allocated elements.
		_table<type> table_copy = table;

		// allocate a new array of elements
		table.elements = new type[table.capacity];

		// move all old elements into the new array
		_move_elements_from(table_copy);
		
	}

	template<typename type>
	void dynamic_array<type>::resize_to_fit()
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
		for (size_t i = 0; i != table.size; i++)
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
		return get(index);
	}

	// // iterators \\ \\

	template<typename type>
	dynamic_array<type>::iterator dynamic_array<type>::begin()
	{
		return (iterator)&table.elements[0];
	}

	template<typename type>
	dynamic_array<type>::iterator dynamic_array<type>::end()
	{
		return (iterator)&table.elements[table.size-1];
	}

	template<typename type>
	dynamic_array<type>::const_iterator dynamic_array<type>::begin() const
	{
		return (const_iterator)&table.elements[0];
	}

	template<typename type>
	dynamic_array<type>::const_iterator dynamic_array<type>::end() const
	{
		return (const_iterator)&table.elements[table.size - 1];
	}
}

#endif DYNAMIC_ARRAY_H