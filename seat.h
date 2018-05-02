#ifndef SEAT_H
#define SEAT_H
#include "customer.h"

class seat{
    enum{AVAIL,SOLD,PROCESS}_state;
    int _seatid;
    customer *_cust;
    
public:
    seat();
    seat(int seatid, customer *cust);
    bool isAvail() const;
    bool isSold() const;
    bool isProcessing() const;
};

#endif
