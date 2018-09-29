#include <iostream>
#include <string.h>
#include <math.h>
#include <bits/stdc++.h>
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

int n, m, k;

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
		case 1: res = n*k;
		case 2: res = n*k + k*(k-1)*n;
		case 3: res = n*k + k*(k-1)*n + m*k;
		default : res = 0;
	}
	if (mode == 0)
		res += (a-1)*k + b;
	if (mode == 1)
	{
		res += (a-1)*k*(k-1) + (b-1)*(k)+c;
		res -= b < c ? b : b-1;
	}
	if (mode == 2)
		res += (edges_prefix[a][b]-1)*k + c;
	if (mode == 3)
		res += (a-1)*a/2 + b;
	return res;
}

int main(int argv, char* args[])
{
	cin >> n >> m >> K;
	vector< vector<int> > constraints;
	vector< vector<int> > edges(n+1);
	vector< vector<int> > edges_prefix(n+1);
	int counter_const = 0;

	for (int i = 1; i <= n; ++i)
	{
		edges[i].reshape(n+1);
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
		}
		else
		{
			edges[a][b] = 1;
		}
	}

	for (int i = 1; i <= n; ++i)
	{
		sort(edges[i].begin(), edges[i].end());
	}

	int local_counter = 1;
	for (int i = 1; i <= n; ++i)
	{
		edges_prefix[i].reshape(n+1);
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

	// Clique constraint
	for (int k = 1; k <= K; ++k)
	{
		for (int j = 1; j <= n; ++j)
		{
			for (int i = 1; i <= n; ++i)
			{
				vector<int> local_constraint;
				int a, b, c;
				a = -1*get_index(i,k,0,0);
				b = -1*get_index(j,k,0,0);
				c = get_index(i,j,0,3);
				local_constraint.push_back(a);
				local_constraint.push_back(b);
				local_constraint.push_back(c);
				constraints.push_back(local_constraint);
			}
		}

	}

	// Subgraph constraint
	for (int k1 = 1; k1 <= K; ++k1)
	{
		for (int k2 = k1+1; k2 <= K; ++k2)
		{
			vector<int> local_constraint;
			for (int i = 0; i <= n; ++i)
			{
				local_constraint.push_back(get_index(i, k1, k2, 1));
			}
			constraints.push_back(local_constraint);

			vector<int> local_constraint;
			for (int i = 0; i <= n; ++i)
			{
				local_constraint.push_back(get_index(i, k2, k1, 1));
			}
			constraints.push_back(local_constraint);
		}
	}

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

				vector<int> local_constraint;
				local_constraint.push_back(-1*a);
				local_constraint.push_back(-1*c);
				constraints.push_back(local_constraint);

				vector<int> local_constraint;
				local_constraint.push_back(a);
				local_constraint.push_back(-1*b);
				local_constraint.push_back(c);
				constraints.push_back(local_constraint);
			}
		}
	}

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

				vector<int> local_constraint;
				local_constraint.push_back(-1*a);
				local_constraint.push_back(c);
				constraints.push_back(local_constraint);

				vector<int> local_constraint;
				local_constraint.push_back(a);
				local_constraint.push_back(-1*b);
				local_constraint.push_back(-1*c);
				constraints.push_back(local_constraint);

			}
		}
	}

	// y constraint
	for (int i = 1; i <= n; ++i)
	{
		for (int j = i+1; j <= n; ++j)
		{
			vector<int> local_constraint(1);
			if(edges[i][j] == 0)
			{
				local_constraint[0] = -1*get_index(i,j,0,3);
			}
			else
			{
				local_constraint[0] = get_index(i,j,0,3);
			}
			constraints.push_back(local_constraint);
		}
	}
}
