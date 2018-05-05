#ifndef CRITICAL_H
#define CRITICAL_H

//inputs
int input_customer;

//number of sellers
int h_seller = 1;
int m_seller = 3;
int l_seller = 6;

//total available seats left
int remaining_seats = 0;

//mutex and cond
pthread_mutex_t mutex = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond = PTHREAD_MUTEX_INITIALIZER;

#endif
