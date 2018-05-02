#ifnedf SELLER_H
#define SELLER_H

#include <ostream>

class seller{
    int sellerid;
    int price;
    customer *queue;
public:
    seller();
};
std::ostream &operator <<(std::ostream &ostr, const seller *s, const int length);

#endif
