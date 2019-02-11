#include <bits/stdc++.h>
#include "avl.hpp"

using namespace std;
typedef long long ll;

int main() {
	tree<int, int> t;

	int p;
	cin >> p;

	int n = 10;
	for(int i = 1; i <= n; ++i) t.insert(2*i, 2*i);
	for(int i = 0; i < n; ++i) cout << t.at(i) << " ";
	cout << endl;
	cout << t.count_less_than(p) << endl;

}