#include "avl.hpp"

using namespace std;

tree::tree() {
	root = NULL;
}

node * tree::zig_zig(node * a, node * b, int left) {
	if(left) {
		a->left = b->right;
		b->right = a;
	}
	else {
		a->right = b->left;
		b->left = a;
	}
	return b;
}

node * tree::zig_zag(node * a, node * b, node * c, int left) {
	zig_zig(b, c, !left);
	if(left) a->left = c;
	else a->right = c;
	return zig_zig(a, c, left);
}

node * tree::update(node * cur) {
	node * a, * b, * c;
	if(cur == NULL) return NULL;
	cur->alt = max(cur->left? cur->left->alt : 0, cur->right? cur->right->alt : 0) + 1;
	cur->avl = (cur->right? cur->right->alt : 0) - (cur->left? cur->left->alt : 0);
	if(cur->avl == -2) {
		a = cur; b = cur->left; c = cur->left->right;
		if(b->avl == 1) cur = zig_zag(a, b, c, 1);
		else cur = zig_zig(a, b, 1);
		update(a); update(b); update(c);
	}
	else if(cur->avl == 2) {
		a = cur; b = cur->right; c = cur->right->left;
		if(b->avl == -1) cur = zig_zag(a, b, c, 0);
		else cur = zig_zig(a, b, 0);
		update(a); update(b); update(c);
	}
	return cur;
}

node * tree::insert(node * cur, node * n) {
	if(cur == NULL) return n;
	if(n->key < cur->key) cur->left = insert(cur->left, n);
	else cur->right = insert(cur->right, n);
	return update(cur);
}

void tree::insert(int key, int val) {
	node * n = new node(key, val);
	root = insert(root, n);
}

int tree::find(node * cur, int key) {
	if(cur == NULL) return -1; //not found
	if(cur->key == key) return cur->val;
	else if(key > cur->key) return find(cur->right, key);
	else return find(cur->left, key);
}

int tree::find(int key) {
	return find(root, key);
}

node * tree::remove_predecessor(node * cur) {
	node * n, * prev = cur;
	stack<node *> st;
	for(n = cur->left; n->right; n = n->right) {
		st.push(n);
		prev = n;
	}
	if(n->left) {
		prev = zig_zig(n, n->left, 1);
		st.push(prev);
	}
	cur->key = n->key;
	cur->val = n->val;
	delete n;
	if(prev != cur) prev->right = NULL;
	else prev->left = NULL;

	while(st.size()) {
		n = st.top(); st.pop();
		n->right = update(n->right);
	}
	cur->left = update(cur->left);
	return update(cur);
}

node * tree::remove(node * cur, int key) {
	if(cur == NULL) return NULL; //nao achou a chave
	if(key == cur->key) {
		if(cur->left == NULL and cur->right == NULL) {
			delete cur;
			cur = NULL;
		}
		else if(cur->left) {
			return remove_predecessor(cur);
		}
		else {
			cur->key = cur->right->key;
			cur->val = cur->right->val;
			delete cur->right;
			cur->right = NULL;
		}
	}
	else if(key > cur->key) cur->right = remove(cur->right, key);
	else cur->left = remove(cur->left, key);
	return update(cur);
}

void tree::in_order(node * cur) {
	if(cur) {
		in_order(cur->left);
		printf("(%d, %d) altura = %d avl = %d\n", cur->key, cur->val, cur->alt, cur->avl);
		in_order(cur->right);
	}
}

void tree::remove(int key) {
	root = remove(root, key);
}

bool tree::is_empty() {
	return root == NULL;
}

void tree::dealloc_node(node * n) {
	if(n == NULL) return;
	dealloc_node(n->left);
	dealloc_node(n->right);
	delete n;
}

tree::~tree() {
	dealloc_node(root);
}

