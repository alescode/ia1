#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <string.h>
using namespace std;

int main(void) {
    vector<string>* A = new vector<string>;
    string* h = new string("hola");
    string* f = new string("chao");

    A->push_back(*h);
    A->push_back(*f);    
    A->push_back(*h);
    A->push_back(*f);    

    for (int i = 0; i < (*A).size(); i++)
        cout << (*A)[i] << " ";
    cout << endl;
}

