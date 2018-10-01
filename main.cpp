#include <iostream>
#include <vector>
#include <fstream>
#include <string.h>
#include <math.h>
// #include <bits/stdc++.h>
#include <unordered_map>

#define vit vector<int>::iterator
#define vec vector<int>
#define itr ::iterator
#define pb push_back
#define mp make_pair
#define mod(a,b) (a%b+b)%b

/*
vector	void push_back	pop_back
		insert(itr)	erase(itr)
		ITERATOR:
		CONST ITR vec.begin(), vec.end()
		*itr returns the value stored

map 	ret itr .find(Key)
<Key,T> map[key] = T


stack	void push pop
		ret top

queue	void push pop
		ret front

sort(start itr, end itr)

*/
using namespace std;

int n, m, K, nVar, nClause;
vector< vector<int> > edges_prefix;

int pseudohash(int a, int b)
{
	int ret;
	if(a > b)
	{
		ret = b*n + a;
	}
	else
	{
		ret = a*n + b;
	}
	return ret;
}

int get_index(int a, int b, int c, int mode)
{
	int res;
	switch (mode)
	{
		case 1: res = n*K; break;
		case 2: res = n*K + K*(K-1)*n; break;
		case 3: res = n*K + K*(K-1)*n + m*K; break;
		default : res = 0; break;
	}
	if (mode == 0)
		res += (b-1)*n + a;
	if (mode == 1)
	{
		res += (a-1)*K*(K-1) + (b-1)*(K)+c;
		res -= b < c ? b : b-1;
	}
	if (mode == 2)
		res += (edges_prefix[a][b]-1)*K + c;
	if (mode == 3)
	{
		// std:://cerr << "a, b - " << a << " " << b << '\n';
		// std:://cerr << "res before - " << res << '\n';
		res += (a-1)*n - (a-1)*a/2 + (b-a);
		// std:://cerr << "res after - " << res << '\n';
	}
	return res;
}

int main(int argv, char* args[])
{

	// string filename = argv[1];
	// std::ifstream in(filename+".graph");
  // std::streambuf *cinbuf = std::cin.rdbuf(); //save old buf
  // std::cin.rdbuf(in.rdbuf()); //redirect std::cin to in.txt!
	//
  // std::ofstream out(filename+".satinput");
  // std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
  // std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!


	cin >> n >> m >> K;
	nVar = n*K + K*(K-1)*n + m*K;
	 // + n*(n-1)/2;
	vector< vector<int> > constraints;
	vector< vector<int> > edges(n+1);
	edges_prefix.resize(n+1);
	int counter_const = 0;
	int node_const, clique_const, subgraph_const, edge_const, z_const, u_const;

	for (int i = 1; i <= n; ++i)
	{
		edges[i].resize(n+1);
		for (int j = 1; j <= n; ++j)
		{
			edges[i][j] = 0;
		}
	}

	for (int i = 0; i < m; ++i)
	{
		int a, b;
		cin >> a >> b;
		if(a > b)
		{
			edges[b][a] = 1;
			//cerr << "Edge between " << b << " " << a << "\n";
		}
		else
		{
			edges[a][b] = 1;
			//cerr << "Edge between " << a << " " << b << "\n";
		}
	}

	int local_counter = 1;
	for (int i = 1; i <= n; ++i)
	{
		edges_prefix[i].resize(n+1);
		for (int j = 1; j <= n; ++j)
		{
			if(edges[i][j] == 1)
			{
				edges_prefix[i][j] = local_counter;
				local_counter++;
			}
			else
			{
				edges_prefix[i][j] = 0;
			}
		}
	}

	// Each node in at least one k constraint
	for (int i = 1; i <= n; ++i)
	{
		vector<int> local_constraint;
		for (int k = 1; k <= K; ++k)
		{
			local_constraint.push_back(get_index(i,k,0,0));
		}
		constraints.push_back(local_constraint);
	}
	node_const = constraints.size();

	// Clique constraint
	for (int k = 1; k <= K; ++k)
	{
		for (int i = 1; i <= n; ++i)
		{
			for (int j = i+1; j <= n; ++j)
			{
				if(edges[i][j] == 1)
					continue;
				vector<int> local_constraint;
				int a, b, c;
				a = -1*get_index(i,k,0,0);
				b = -1*get_index(j,k,0,0);
				// c = get_index(i,j,0,3);
				local_constraint.push_back(a);
				local_constraint.push_back(b);
				// local_constraint.push_back(c);
				constraints.push_back(local_constraint);
			}
		}
	}
	clique_const = constraints.size() -node_const;

	// Subgraph constraint
	for (int k1 = 1; k1 <= K; ++k1)
	{
		for (int k2 = k1+1; k2 <= K; ++k2)
		{
			vector<int> local_constraint;
			for (int i = 1; i <= n; ++i)
			{
				local_constraint.push_back(get_index(i, k1, k2, 1));
			}
			constraints.push_back(local_constraint);

			local_constraint.resize(0);
			for (int i = 1; i <= n; ++i)
			{
				local_constraint.push_back(get_index(i, k2, k1, 1));
			}
			constraints.push_back(local_constraint);
		}
	}
	std::cerr << constraints.size() << '\n';
	subgraph_const = constraints.size() - clique_const;

	// Edge constraint
	for (int i = 1; i <= n; ++i)
	{
		for (int j = i+1; j <= n; ++j)
		{
			if(edges[i][j] == 0)
				continue;
			vector<int> local_constraint;
			for (int k = 1; k <= K; ++k)
			{
				local_constraint.push_back(get_index(i,j,k,2));
			}
			constraints.push_back(local_constraint);
		}
	}
	edge_const = constraints.size() - subgraph_const;


	// z constraint
	for (int i = 1; i <= n; ++i)
	{
		for (int k1 = 1; k1 <= K; ++k1)
		{
			for (int k2 = 1; k2 <= K; ++k2)
			{
				if(k1 == k2)
					continue;
				int a, b, c;
				a = get_index(i,k1,k2,1);
				b = get_index(i,k1,0,0);
				c = get_index(i,k2,0,0);


				vector<int> local_constraint;
				local_constraint.push_back(-1*a);
				local_constraint.push_back(b);
				constraints.push_back(local_constraint);

				local_constraint.resize(0);
				local_constraint.push_back(-1*a);
				local_constraint.push_back(-1*c);
				constraints.push_back(local_constraint);

				local_constraint.resize(0);
				local_constraint.push_back(a);
				local_constraint.push_back(-1*b);
				local_constraint.push_back(c);
				constraints.push_back(local_constraint);
			}
		}
	}
	z_const = constraints.size() - edge_const;

	// u constraint
	for (int i = 1; i <= n; ++i)
	{
		for (int j = i+1; j <= n; ++j)
		{
			if(edges[i][j] == 0)
				continue;
			for (int k = 1; k <= K; ++k)
			{
				int a, b, c;
				a = get_index(i,j,k,2);
				b = get_index(i,k,0,0);
				c = get_index(j,k,0,0);


				vector<int> local_constraint;
				local_constraint.push_back(-1*a);
				local_constraint.push_back(b);
				constraints.push_back(local_constraint);

				local_constraint.resize(0);
				local_constraint.push_back(-1*a);
				local_constraint.push_back(c);
				constraints.push_back(local_constraint);

				local_constraint.resize(0);
				local_constraint.push_back(a);
				local_constraint.push_back(-1*b);
				local_constraint.push_back(-1*c);
				constraints.push_back(local_constraint);

			}
		}
	}
	u_const = constraints.size() - z_const;

	// y constraint
	// for (int i = 1; i <= n; ++i)
	// {
	// 	for (int j = i+1; j <= n; ++j)
	// 	{
	// 		vector<int> local_constraint(1);
	// 		if(edges[i][j] == 0)
	// 		{
	// 			local_constraint[0] = -1*get_index(i,j,0,3);
	// 			//cerr << "No edge between " << i << " and " << j << " " << get_index(i,j,0,3) << "\n";
	// 		}
	// 		else
	// 		{
	// 			local_constraint[0] = get_index(i,j,0,3);
	// 			//cerr << "Adding edge between " << i << " and " << j <<  " " << get_index(i,j,0,3) << "\n";
	// 		}
	// 		constraints.push_back(local_constraint);
	// 	}
	// }
	nClause = constraints.size();

	cerr << "node_const " << node_const << "\n";
	cerr << "clique_const " << clique_const << "\n";
	cerr << "subgraph_const " << subgraph_const << "\n";
	cerr << "edge_const " << edge_const << "\n";
	cerr << "z_const " << z_const << "\n";
	cerr << "u_const " << u_const << "\n";

	// Print out constraints
	cout << "p cnf " << to_string(nVar) << " " << to_string(nClause) << " \n";
	for(vector< vector<int> >::iterator it = constraints.begin(); it != constraints.end(); ++it)
	{
	    vector<int> constraint = *it;
			for(vector<int>::iterator it_vec = constraint.begin(); it_vec != constraint.end(); ++it_vec)
			{
				cout << to_string(*it_vec) << ' ';
			}
			cout << "0\n";
	}

	// cin.rdbuf(cinbuf);   //reset to standard input again
  // cout.rdbuf(coutbuf); //reset to standard output again
}
