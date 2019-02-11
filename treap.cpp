#include <bits/stdc++.h>

#define pb push_back
#define fi first
#define se second
#define mp make_pair

const int N = 100007, inf = 0x3f3f3f3f;

using namespace std;
typedef long long ll;

struct node {
	int key, prior, cnt;
	node * l, * r;

	node(){}
	node(int key) {
		l = r = 0;
		prior = rand();
		this->key = key;
		cnt = 0;
	}

	void update_cnt() {
		cnt = (l ? l->cnt : 0) + (r ? r->cnt : 0) + 1; 
	}

	// Separa a treap t em treaps l e r, l contem todas os items menores ou iguais a key, e r os estritamente maiores
	static void split(node * t, int key, node * & l, node * & r) {
		if(t == 0) return void(l = r = 0);
		if(key < t->key)
			split(t->l, key, l, t->l), r = t;
		else 
			split(t->r, key, t->r, r), l = t;
		if(t) t->update_cnt();
	}

	// Une as arvores l e r em t, com a premissa de que todos os items de l sao menores que qualquer item de r
	static void merge(node * & t, node * l, node * r) {
		if(!l or !r) t = l? l : r;
		else if(l->prior > r->prior)
			merge(l->r, l->r, r), t = l;
		else 
			merge(r->l, l, r->l), t = r;
		if(t) t->update_cnt();
	}

	// Adiciona um no em t, cumprindo a restricao da prioridade
	static void add(node * & t, node * n) {
		if(t == 0) t = n;
		else if(t->prior < n->prior)
			split(t, n->key, n->l, n->r), t = n;
		else if(n->key < t->key) add(t->l, n);
		else add(t->r, n);
		if(t) t->update_cnt();
	}

	// Remove algum item com chave key de t
	static void remove(node * & t, int key) {
		if(t == 0) return; // nao achou a chave
		if(t->key == key)
			merge(t, t->l, t->r);
		else if(key < t->key) 
			remove(t->l, key);
		else 
			remove(t->r, key);
		if(t) t->update_cnt();
	}

	// Acha o k-esimo cara da treap t (k-esimo cara eh o cara que tem k items menores que ele)
	static int find_kth(node * t, int k, int add = 0) {
		if(t == 0) throw runtime_error("Invalid treap position");
		int less = add + (t->l? t->l->cnt : 0);
		if(less == k) return t->key;
		else if(less > k) return find_kth(t->l, k, add);
		else return find_kth(t->r, k, less + 1);
	}

	// Calcula quantos items da treap t sao menores que key
	static int key_order(node * t, int key) {
		if(t == 0) return 0;
		if(t->key < key) return (t->l? t->l->cnt : 0) + 1 + key_order(t->r, key);
		else return key_order(t->l, key);
	}

	~node() {
		if(l) delete l;
		if(r) delete r;
	}
};

struct treap {
	node * root;

	treap(){root = 0;}
	void add(int key) {
		return node::add(root, new node(key));
	}

	void remove(int key) {
		return node::remove(root, key);
	}

	int get_min() {
		if(!root) throw runtime_error("Treap is empty");
		node * cur = root;
		while(cur->l) cur = cur->l;
		return cur->key;
	}

	int count(int key) {
		return node::key_order(root, key + 1) - node::key_order(root, key);
	}

	int find_kth(int k) {
		if(k < 0 or k >= size()) throw runtime_error("Invalidd treap position");
		return node::find_kth(root, k);
	}

	int key_order(int key) {
		return node::key_order(root, key);
	}

	int size() {
		return root? root->cnt : 0;
	}

	~treap() {delete root;}
};



int main() {
	int n;
	scanf("%d", &n);

	vector<int> v;
	for(int i = 0; i < n; ++i) {
		int a; scanf("%d", &a);
		v.pb(a);
	}

	// ordena por treap-sort
	treap t;
	for(int i = 0; i < v.size(); ++i) {
		t.add(v[i]);
	}

	for(int i = 0; i < v.size(); ++i) {
		v[i] = t.find_kth(i);
	}

	for(int i = 0; i < v.size(); ++i) {
		printf("%d\n", v[i]);
	}

}

