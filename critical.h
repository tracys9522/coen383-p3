#ifndef CRITICAL_H
#define CRITICAL_H
#include <pthread.h>
//inputs
int input_customer;

//number of sellers
int h_seller = 1;
int m_seller = 3;
int l_seller = 6;

//number of customer for each seller
int h_cust = 0;
int m_cust = 0;
int l_cust = 0;

//total available seats left
int remaining_seats = 0;

//mutex and cond
pthread_mutex_t seat_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

#endif
