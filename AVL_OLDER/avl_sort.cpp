#include <bits/stdc++.h>
#include "avl.hpp"

using namespace std;

int main() {
	tree<int, int> t; // Arvore AVL utilizada na ordenacao
	ifstream in; //arquivo de entrada do vetor desordenado
	ofstream out; //arquivo de saida do vetor ordenado

	/* Abertura dos arquivos */
	in.open("in.txt", ios::in);
	out.open("out.txt", ios::out);

	/* Enquanto nao chegou no final do arquivo, le o inteiro e adiciona na estrutura avl */
	while(!in.eof()) {
		int a;
		in >> a;
		t.insert(a, a);
	}
	in.close();

	/* Enquanto a arvore nao esta vazia, retira o menor valor e escreve no arquivo de saida */
	while(t.is_empty() == false) {
		int a = t.min_key();
		t.remove(a);
		out << a << " ";
	}
	out << endl;
	out.close();

}