#include "seat.h"

seat::seat()
{
    _seatid = -1;
    _cust = NULL;
    _state = AVAIL;
}

seat::seat(int seatid,customer *cust)
{
    _seatid = seatid;
    _cust = cust;
}
void seat::set_id(int id){
    _seatid = id;
}

void seat::set_proc(customer *cust){
    _cust = cust;
    _state = PROCESS;
}

void seat::set_sold(){
    _state = SOLD;
}

int seat::seatID() const{
    return _seatid;
}

bool seat::isAvail() const
{
    return _state == AVAIL;
}
bool seat::isSold() const
{
    return _state == SOLD;
}
bool seat::isProcessing() const
{
    return _state == PROCESS;
}

ostream &operator << (ostream &ostr, const seat s)
{
    ostr<<s.seatID();
    return ostr;
}
