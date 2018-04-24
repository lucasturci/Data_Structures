#ifndef AVL_H
#define AVL_H

#include <unistd.h>
#include <bits/stdc++.h>

/* Um no da arvore AVL, contendo uma chave (de tipo T1) e um valor (de tipo T2) */
template<typename T1, typename T2>
struct node {
	node<T1, T2> * left, * right;
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

/* Estrutura da arvore AVL, em formato template. Pode guardar qualquer tipo: T1 = chave, T2 = valor. Na biblioteca STL, seria um multimap */
template<typename T1, typename T2>
struct tree {
	node<T1, T2> * root;

	tree();

	node<T1, T2> * zig_zig(node<T1, T2> * a, node<T1, T2> * b, int left);
	node<T1, T2> * zig_zag(node<T1, T2> * a, node<T1, T2> * b, node<T1, T2> * c, int left);
	node<T1, T2> * update(node<T1, T2> * cur);
	node<T1, T2> * insert(node<T1, T2> * cur, node<T1, T2> * n);
	void insert(T1 key, T2 val);
	T2 find(node<T1, T2> * cur, T1 key);
	T2 find(T1 key);
	node<T1, T2> * remove_predecessor(node<T1, T2> * cur);
	node<T1, T2> * remove(node<T1, T2> * cur, T1 key);
	T2 min_key();
	T2 max_key();
	void remove(T1 key);
	bool is_empty();
	void in_order(node<T1, T2> * cur);
	void debug(node<T1, T2> * cur);
	void dealloc_node(node<T1, T2> * n);

	~tree();

};

/* Construtor da arvore AVL */
template <typename T1, typename T2>
tree<T1, T2>::tree() {
	root = NULL;
}

/* Rotacao zig-zig (left = 1 se a rotacao eh para a direita, 0 caso contrario). Retorna o novo no raiz dessa subarvore */
template <typename T1, typename T2>
node<T1, T2> * tree<T1, T2>::zig_zig(node<T1, T2> * a, node<T1, T2> * b, int left) {
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

/* Rotacao zig-zag (left = 1 se a rotacao eh para a direita, 0 caso contrario) */
template <typename T1, typename T2>
node<T1, T2> * tree<T1, T2>::zig_zag(node<T1, T2> * a, node<T1, T2> * b, node<T1, T2> * c, int left) {
	zig_zig(b, c, !left);
	if(left) a->left = c;
	else a->right = c;
	return zig_zig(a, c, left);
}

/* Atualiza os parametros avl e alt (altura) de um no da arvore. 
Caso o fator avl seja invalido, uma rotacao eh realizada. Retorna o novo no raiz dessa subarvore */
template <typename T1, typename T2>
node<T1, T2> * tree<T1, T2>::update(node<T1, T2> * cur) {
	node<T1, T2> * a, * b, * c;
	if(cur == NULL) return NULL;
	cur->alt = std::max(cur->left? cur->left->alt : 0, cur->right? cur->right->alt : 0) + 1;
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

/* Insere um no (n) na subarvore de raiz cur. Retorna a nova raiz dessa subarvore, caso sejam necessarias rotacoes */
template <typename T1, typename T2>
node<T1, T2> * tree<T1, T2>::insert(node<T1, T2> * cur, node<T1, T2> * n) {
	if(cur == NULL) return n;
	if(n->key < cur->key) cur->left = insert(cur->left, n);
	else cur->right = insert(cur->right, n);
	return update(cur); //eh preciso atualizar os fatores avl e altura desse no, e fazer as rotacoes necessarias
}

/* Insere uma nova chave na arvore */
template <typename T1, typename T2>
void tree<T1, T2>::insert(T1 key, T2 val) {
	node<T1, T2> * n = new node<T1, T2>(key, val);
	root = insert(root, n);
}

/* Encontra o valor correspondente a chave key na subarvore de raiz cur */
template <typename T1, typename T2>
T2 tree<T1, T2>::find(node<T1, T2> * cur, T1 key) {
	if(cur == NULL) return -1; //not found
	if(cur->key == key) return cur->val;
	else if(key > cur->key) return find(cur->right, key);
	else return find(cur->left, key);
}

/* Encontra o valor correspondente a chave key. Retorna -1 caso ela nao seja achada. */
template <typename T1, typename T2>
T2 tree<T1, T2>::find(T1 key) {
	return find(root, key);
}

/* Remove o predecessor de cur.*/
template <typename T1, typename T2>
node<T1, T2> * tree<T1, T2>::remove_predecessor(node<T1, T2> * x) {
	node<T1, T2> * n, * prev = x;
	std::stack<node<T1, T2> *> st; // Serve para guardar os antecessores, na arvore, do predecessor
	for(n = x->left; n->right; n = n->right) {
		st.push(n);
		prev = n;
	}

	/* Uma excecao, em que nao ha filho direito, mas ha filho esquerdo. Nesse caso, por ser uma arvore AVL, ele pode 
	ter no maximo um filho esquerdo. Queremos remover um no folha, assim, uma rotacao para a direita eh suficiente 
	para corrigir */
	if(n->left) { 
		prev = zig_zig(n, n->left, 1);
		st.push(prev);
	}

	/* Trocamos os valores guardados ali pelos valores do no x. Assim, preservamos esse no e removemos o no x */
	x->key = n->key;
	x->val = n->val;
	delete n;
	n = NULL;

	if(prev != x) prev->right = NULL;
	else prev->left = NULL;

	// Atualiza os nos antecessores, para tomar conta das possiveis rotacoes necessarias
	while(st.size()) {
		n = st.top(); st.pop();
		
		n->right = update(n->right);
	}
	x->left = update(n);
	return update(x);
}

/* Remove a chave key na subarvore de raiz cur. Retorna a nova raiz dessa subarvore, caso sejam necessarias rotacoes */
template <typename T1, typename T2>
node<T1, T2> * tree<T1, T2>::remove(node<T1, T2> * cur, T1 key) {
	if(cur == NULL) return NULL; //nao achou a chave
	if(key == cur->key) { //Achou
		if(cur->left == NULL and cur->right == NULL) { //Eh um no folha, podemos remover
			delete cur;
			cur = NULL;
		}
		else if(cur->left) { // Nao eh um no folha, trocamos ele com o predecessor, que eh um no folha
			node<T1, T2> * x = remove_predecessor(cur);
			return x;
		}
		else { //Se o no nao tiver filho esquerdo (nao podemos tirar o predecessor) ele ter no maximo 1 no aa direita
			//Trocamos com ele entao, que eh o succesor
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

/* Remove a chave key da arvore */
template <typename T1, typename T2>
void tree<T1, T2>::remove(T1 key) {
	root = remove(root, key);
}

/* Encontra o valor da minima chave */
template<typename T1, typename T2>
T2 tree<T1, T2>::min_key() {
	node<T1, T2> * n = root;
	while(n->left) n = n->left;
	return n->val;
}

/* Encontra o valor da maxima chave */
template<typename T1, typename T2>
T2 tree<T1, T2>::max_key() {
	node<T1, T2> * n = root;
	while(n->right) n = n->right;
	return n->val;
}

/* Retorna se a arvore esta vazia */
template <typename T1, typename T2>
bool tree<T1, T2>::is_empty() {
	return root == NULL;
}

/* Desaloca uma subarvore de raiz n */
template <typename T1, typename T2>
void tree<T1, T2>::dealloc_node(node<T1, T2> * n) {
	if(n == NULL) return;
	dealloc_node(n->left);
	dealloc_node(n->right);
	delete n;
}

template <typename T1, typename T2>
void tree<T1, T2>::debug(node<T1, T2> * cur) {
	if(cur) {
		debug(cur->left);
		std::cout << "Val = " << cur->val << " Alt = " << cur->alt << " AVL = " << cur->avl << std::endl;
		debug(cur->right); 
	}
}

/* Deconstrutor. Desaloca a arvore*/
template <typename T1, typename T2>
tree<T1, T2>::~tree() {
	dealloc_node(root);
}

#endif