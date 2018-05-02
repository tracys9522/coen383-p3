#include "seat.h"

seat::seat()
{
    _seatid = -1;
    _cust = NULL;
}
seat::seat(int seatid,customer *cust)
{
    _seatid = seatid;
    _cust = cust;
}
bool seat::isAvail() const
{
    return _state == AVAIL;
}
bool seat::isSold() const
{
    return _state == SOLD;
}
bool seat::isProcessing const
{
    return _state == PROCESS;
}

