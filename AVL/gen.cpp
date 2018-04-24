#include <bits/stdc++.h>

using namespace std;

/* Gerador de vetores desordenados */
int main() {
	int n = 100;
	ofstream out("in.txt", ios::out);
	vector<int> v;

	for(int i = 1; i <= n; ++i) v.push_back(i);

	random_shuffle(v.begin(), v.end());
	for(int i = 0; i < n; ++i) out << v[i] << " ";
	out << endl;
}