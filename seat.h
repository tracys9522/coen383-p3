#ifndef SEAT_H
#define SEAT_H
#include <string>
#include "customer.h"

class seat{
    enum{AVAIL,SOLD,PROCESS}_state;
    int _seatid;
    customer *_cust;
    string _seatname;

public:
    seat();
    seat(int seatid, customer *cust);
    void set_id(int id);
    void set_proc();
    void set_sold(string seatname);
    int seatID() const;
    bool isAvail() const;
    bool isSold() const;
    bool isProcessing() const;
    string seatname() const;
};

ostream &operator <<(ostream &ostr, const seat s);
#endif
