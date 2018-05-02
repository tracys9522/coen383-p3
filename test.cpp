#include "customer.h"
#include <iostream>
using namespace std;

int main(){
    customer h = customer(10,22);
    cout<<h<<endl;
    cout<<h.custid()<<endl;
    cout<<h.arrival()<<endl;
}
