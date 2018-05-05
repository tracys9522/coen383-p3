#ifndef SELLER_H
#define SELLER_H
#include "customer.h"
#include <ostream>

class seller{
    int sellerid;
    int price;
    enum type{H,M,L}_type;
    customer *queue;
public:
    seller();
    seller(int s_id, type t);
    int get_sale_time();
};
std::ostream &operator <<(std::ostream &ostr, const seller s);

#endif
