#include "customer.h"

customer::customer()
{
    _custid = -1;
    _arrival = -1;
    _seatid = -1;
}
customer::customer(int custid, int arrival)
{
    _custid = custid;
    _arrival = arrival;
    _seatid = -1;
}

void customer::set_id(int id) {
  _custid = id;
}

void customer::set_seat(int id) {
  _seatid = id;
}

int customer::custid() const
{
    return _custid;
}

int customer::arrival() const
{
    return _arrival;
}

int customer::seatid() const {
  return _seatid;
}

customer& customer::operator=(const customer& c){
    _custid = c._custid;
    _arrival = c._arrival;
    return *this;
}

ostream &operator << (ostream &ostr, const customer cust)
{
    ostr<<"cust id: "<<cust.custid() << " arrival: "<< cust.arrival();
    return ostr;
}
