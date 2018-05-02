#ifndef CUSTOMER_H
#define CUSTOMER_H
#include <ostream>
#include <iostream>
using namespace std;

class customer{
    int _arrival;
    int _custid;
public:
    customer();
    customer(int custid, int arrival);
    int arrival() const;
    int custid() const;
};
ostream &operator <<(ostream &ostr, const customer cust);
#endif
