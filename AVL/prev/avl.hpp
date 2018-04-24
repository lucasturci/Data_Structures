#ifndef AVL_H
#define AVL_H

#include <utility>
#include <stack>

template<typename T1, typename T2>
struct node {
	node * left, * right;
	int avl, alt;
	T1 key;
	T2 val;
	node(){}
	node(T1 key, T2 val) {
		this->key = key;
		this->val = val;
		left = right = NULL;
		avl = 0;
		alt = 1;
	}

};

template<typename T1, typename T2>
struct tree {
	node<T1, T2> * root;

	tree();

	node<T1, T2> * zig_zig(node<T1, T2> * a, node<T1, T2> * b, int left);
	node<T1, T2> * zig_zag(node<T1, T2> * a, node<T1, T2> * b, node<T1, T2> * c, int left);
	node<T1, T2> * update(node<T1, T2> * cur);
	node<T1, T2> * insert(node<T1, T2> * cur, node<T1, T2> * n);
	void insert(T1 key, T2 val);
	int find(node<T1, T2> * cur, T1 key);
	T2 find(T1 key);
	node<T1, T2> * remove_predecessor(node<T1, T2> * cur);
	node<T1, T2> * remove(node<T1, T2> * cur, T1 key);
	void remove(T1 key);
	bool is_empty();
	void in_order(node<T1, T2> * cur);
	void dealloc_node(node<T1, T2> * n);

	~tree();

};

#endif