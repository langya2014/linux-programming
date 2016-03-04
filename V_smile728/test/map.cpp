#include <iostream>
#include <map>
#include <vector>

using namespace std;

int main(void)
{
	/*map<char *,int> a;
	cout<<a.size()<<endl;
	char s[] = "finder";
	a.insert(make_pair(s,1));
	cout<<a.size()<<endl;
	cout<<(a.begin())->first<<endl;
	map<char*,int>::iterator it = a.begin();
	for(;it != a.end(); ++it)
	{
		cout<<"aaaa"<<endl;
	}*/

	vector<int> vint;
	vint.push_back(12);
	vint.push_back(10);
	cout<<vint.front()<<endl;
	
}