#include <iostream>
#include <string.h>
#include <math.h>
#include <vector>
#include <unordered_map>
#include <fstream>

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

int n, m, K;

int main(int argv, char* args[])
{
	cin >> n >> m >> K;

	ifstream myfile;
	myfile.open ("test.satoutput", ios::in);

	string sat;
	myfile >> sat;
	if(sat == "SAT")
	{
		for (int k = 1; k <= K; ++k)
		{
			vector<int> subgraph;
			int counter = 0;


			int a;
			for (int i = 1; i <= n; ++i)
			{
				myfile >> a;
				if(a > 0)
				{
					subgraph.push_back(i);
					counter++;
				}
			}

			cout << "#" << k << " " << counter << endl;
			if(counter > 0)
			{
				for (int j = 0; j < subgraph.size() - 1; ++j)
				{
					cout << subgraph[j] << " ";
				}
				cout << subgraph[subgraph.size()-1] << endl;
			}
		}
	}
	else
	{
		cout << 0 << endl;
	}
}
