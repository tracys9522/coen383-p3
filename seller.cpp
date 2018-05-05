#include "seller.h"

using namespace std;

seller::seller()
{
    sellerid = -1;
    price = -1;
}
seller::seller(int s_id, type t)
{
    sellerid = s_id;
    _type = t;
}

int seller::get_sale_time()
{
    int t;
    if (_type == H) { t = rand()%2 +1;}
    else if (_type == M) {t = rand()%3 + 2;} //rand()%(max-min+1)+min, 2~4
    else if (_type == L) {t = rand()%4 + 4;} //4~7
    else {t = -1;} //error
    return t;
}

ostream &operator << (ostream &ostr, const seller s)
{
    return ostr;
}
