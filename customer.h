#ifndef CUSTOMER_H
#define CUSTOMER_H
#include <ostream>
#include <iostream>
using namespace std;

class customer{
    int _arrival;
    int _custid;
    int _seatid;
public:
    customer();
    customer(int custid, int arrival);
    void set_id(int id);
    void set_seat(int id);
    int arrival() const;
    int custid() const;
    int seatid() const;
    customer& operator=(const customer& c);
};
ostream &operator <<(ostream &ostr, const customer cust);
#endif
