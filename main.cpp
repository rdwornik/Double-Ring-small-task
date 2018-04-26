#include <iostream>
#include <string>
#include "BiRing.h"
using namespace std;

int main()
{
    BiRing<int, string> br1;


    br1.push_front(1,"one");
    br1.push_front(2,"two");
    br1.push_front(3,"three");
    br1.push_front(4,"four");
    br1.push_front(5,"five");
    br1.push_front(6,"six");

    BiRing<int, string>::iterator it = br1.begin();
    BiRing<int, string>::iterator it2 = br1.begin();

    it+=2;
    br1.add(it,it2);

    cout<<br1<<endl;

    return 0;
}

//Expected console output for this rudimentary test is:

//[]
//[(1,one)]
//[(1,one)(2,two)]
//[(2,two)]
//[]
//[(1,uno)(2,due)(3,tre)(4,quattro)(5,cinque)]
//[]
//[(1,uno)(2,due)(5,cinque)]
//[(1,uno)(2,due)(5,cinque)]
//