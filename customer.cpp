#include "customer.h"

customer::customer()
{
    _custid = -1;
    _arrival = -1;
}
customer::customer(int custid, int arrival)
{
    _custid = custid;
    _arrival = arrival;
}

int customer::custid() const
{
    return _custid;
}

int customer::arrival() const
{
    return _arrival;
}

ostream &operator << (ostream &ostr, const customer cust)
{
    ostr<<"cust id: "<<cust.custid() << " arrival: "<< cust.arrival();
    return ostr;
}

