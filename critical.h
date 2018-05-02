#ifndef CRITICAL_H
#define CRITICAL_H

#include "pthread.h"
#include <ctime>
#include <cstdlib>
#include <ctime>
#include <sys/time.h>

//100 seats total
seat theater[100];

//seats info
seat *h_seat;
seat *m_seat;
seat *l_seat;
int avail_seats;

int total_time = 60;

//seller info
int h_seller = 1;
int m_seller = 3;
int l_seller = 6;

bool go;

int input_customer;

int h_sold = 0;
int m_sold = 0;
int l_sold = 0;


#endif

