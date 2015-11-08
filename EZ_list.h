#ifndef EZ_LIST_H
#define EZ_LIST_H
#include"EZ_iterator.h"
#include"EZ_simple_alloc.h"
#include"EZ_construct.h"
#include<cstddef>


template<class T>
struct __listNode{
	typedef __listNode<T>* node_pointer;
	node_pointer next;
	node_pointer prev;
	T data;
};

template<class T, class Ref, class Ptr>
struct __list_iterator{

	typedef __list_iterator<T, T&, T*>		iterator;
	typedef __list_iterator<T, Ref, Ptr>	self;

	typedef bidirectional_iterator_tag		iterator_category;
	typedef T								value_type;
	typedef Ptr								pointer;
	typedef	ptrdiff_t						difference_type;
	typedef Ref								reference;
	typedef size_t							size_type;
	typedef __listNode<T>*					link_type;

	link_type node;

	__list_iterator(link_type x) : node(x){}
	__list_iterator(){}
	__list_iterator(const iterator& x) : node(x.node){}

	bool operator== (const self& x)const { return node == x.node; }
	bool operator!= (const self& x)const { return node != x.node; }

	reference operator* ()const { return node->data; }

	pointer operator->() const{ return &(operator*()); }

	self& operator++(){
		node = node->next;
		return *this;
	}

	self operator++(int){
		self tmp = *this;
		++ *this;
		return tmp;
	}

	self& operator--(){
		node = node->prev;
		return *this;
	}

	self& operator--(int){
		self tmp = *this;
		-- *this;
		return tmp;
	}
};


template<class T, class Alloc = alloc>
class list{
protected:
	typedef __listNode<T>	listNode;
	typedef simple_alloc<listNode, Alloc> list_node_allocator;

public:
	typedef __list_iterator<T, T&, T*> iterator;
	typedef __list_iterator<T, const T&, const T*> const_iterator;
	typedef listNode*		link_type;
	typedef T				value_type;
	typedef T*				pointer;
	typedef T&				reference;
	typedef ptrdiff_t		difference_type;
	typedef size_t			size_type;

protected:

	link_type node;

	link_type get_node(){
		return list_node_allocator::allocate();
	}

	void put_node(link_type p){
		list_node_allocator::deallocate(p);
	}

	link_type create_node(const T& x){
		link_type loc = get_node();
		construct(&(loc->data), x);
		return loc;
	}

	void destory_node(link_type p){
		destory(&(p->data));
		put_node(p);
	}

	void empty_initialize()
	{
		node = get_node();
		node->next = node;
		node->prev = node;
	}

public:

	iterator begin(){ return node->next; }
	iterator end(){ return node; }

	bool empty() const{ return node->next == node; }

	size_type size() const{
		size_type result = 0;
		result = distance(begin(), end());
		return result;
	}

	size_type size(){
		size_type result = 0;
		result = distance(begin(), end());
		return result;
	}

	reference front(){ return *begin(); }
	reference back(){ return *(--end()); }

	list() { empty_initialize(); }

	void push_back(const T& x){ insert(end(), x); }

	iterator insert(iterator position, const T& x)
	{
		link_type tmp = create_node(x);
		tmp->next = position.node;
		tmp->prev = position.node->prev;
		(position.node)->prev->next = tmp;
		(position.node)->prev = tmp;
		return tmp;
	}

	void push_front(const T& x)
	{
		insert(begin(), x);
	}

	iterator erase(iterator position)
	{
		link_type next_node = position.node->next;
		next_node->prev = position.node->prev;
		position.node->prev->next = next_node;
		destory_node(position.node);
		return next_node;
	}

	void pop_front(){ erase(begin()); }
	void pop_back(){ erase(--end()); }

	void clear()
	{
		link_type cur = begin().node;
		while (cur != node)
		{
			link_type tmp = cur;
			cur = cur->next;
			destory_node(tmp);
		}

		node->next = node;
		node->prev = node;
	}

	void remove(const T& value)
	{
		iterator first = begin();
		iterator last = end();
		while (first != last)
		{
			if (*first == value)
			{
				first = erase(first);
			}
			else
				++first;
		}
	}

	void unique()
	{
		iterator first = begin();
		iterator last = end();
		if (first == last)
			return;
		iterator next = first;
		while (++next != last)
		{
			if (*next == *first)
			{
				 erase(next);
			}
			else
			{
				first = next;
			}
			next = first;
		}
	}

	void transfer(iterator position, iterator first, iterator last)
	{
		if (position != last)
		{
			last.node->prev->next = position.node;
			first.node->prev->next = last.node;
			position.node->prev->next = first.node;
			link_type tmp = first.node->prev;
			first.node->prev = position.node->prev;
			position.node->prev = last.node->prev;
			last.node->prev = tmp;
		}
	}

};
#endif