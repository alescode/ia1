// inserting into a vector
#include <iostream>
#include <vector>
using namespace std;

int main ()
{
  vector<int> myvector;
  myvector.push_back(4);
  myvector.push_back(5);
  myvector.push_back(8);
  myvector.push_back(2);
  myvector.push_back(1);
  myvector.push_back(9);

  vector<int>::iterator it, end;
  it = myvector.begin() + 3;
    
  myvector.insert(it, 10292);
  
  cout << "myvector contains:";
  for (it=myvector.begin(); it<myvector.end(); it++)
    cout << " " << *it;
  cout << endl;

  return 0;
}