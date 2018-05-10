#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <queue>
#include <iomanip>
#include <unistd.h>
#include <string>
#include "critical.h"
#include "customer.h"
#include "seat.h"

using namespace std;

customer *lists[10];
customer *current_cust[10];
queue<customer*> queues[10];
seat *theater = NULL;

int ih_seat = 0;
int im_seat = 40;
int il_seat = 90;
int m_skip = 10;

int wakeup_time[10] = { 0 };

int global_time = 0;

pthread_cond_t  cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t start_mutex = PTHREAD_MUTEX_INITIALIZER;

//compare arrival time
int arrival_compare(const void *s1, const void *s2)
{
    customer *a = (customer *)s1;
    customer *b = (customer *)s2;
    return a->arrival() - b->arrival();
}

//generate for 10 customer queue
void generateAllQueues(int input)
{
    int arrive,id;
    for(int i = 0; i < 10; i++)
    {
        lists[i] = new customer[input];

        for (int j = 0; j < input; j++){
            arrive = rand()%60;
            id = i * input + j;
            lists[i][j] = customer(id,arrive);
        }
    }

    //sort by arrival
    for (int i = 0; i < 10; i++) {
        qsort((void *)lists[i], input, sizeof(customer),arrival_compare);
        for (int j = 0; j < input; j++) {
          lists[i][j].set_id(j); // set IDs [0..n] in order
          queues[i].push(&lists[i][j]);
        }
    }

    //print out sorted customer queue
    for(int i = 0; i < 10; i++){
        for (int j = 0; j < input; j++) {
            cout<<lists[i][j]<<endl;
        }
        cout<<endl;
    }
}

void init_theater(){
    theater = new seat[100];
    for (int i = 0; i < 100; i++) {
        theater[i].set_id(i);
        //cout << theater[i];
    }
}

//set up sellers at correct position in the theater
void seat_map_init()
{
    remaining_seats = 100;
}

int next_h_seat() {
  if (remaining_seats < 1) {
    return -1;
  }
  while (!theater[ih_seat].isAvail()) {
    ++ih_seat;
  }
  if (theater[ih_seat].isAvail()) {
    return ih_seat;
  }
  return -1;
}

int next_m_seat() {
  if (remaining_seats < 1) {
    return -1;
  }
  while (!theater[im_seat].isAvail()) {
    ++im_seat;
    if (im_seat % 10 == 0) {
      im_seat -= 10;
      im_seat += m_skip;
      if (m_skip > 0) {
        m_skip = -1 * (m_skip + 10);
      } else {
        m_skip = -1 * (m_skip - 10);
      }
    }
  }
  if (theater[im_seat].isAvail()) {
    return im_seat;
  }
  return -1;
}

int next_l_seat() {
  if (remaining_seats < 1) {
    return -1;
  }
  while (!theater[il_seat].isAvail()) {
    ++il_seat;
    if (il_seat%10 == 0) {
      il_seat -= 20;
    }
  }
  if (theater[il_seat].isAvail()) {
    return il_seat;
  }
  return -1;
}

//h seller makes the sell of the seats
seat *h_sell()
{
    seat *soldseat = NULL;

    pthread_mutex_lock(&seat_mutex);

    //if there is still free seats
    if (remaining_seats > 0) {

        if (!theater[ih_seat].isAvail()) {
          next_h_seat();
        }
        soldseat = &theater[ih_seat];
        soldseat->set_proc();
        remaining_seats--;
        h_cust++;
    }

    pthread_mutex_unlock(&seat_mutex);
    return soldseat;
}

//m seller sells the seat
seat *m_sell()
{
    seat *soldseat = NULL;

    pthread_mutex_lock(&seat_mutex);

    //if there is still free seats
    if (remaining_seats > 0) {
        if (!theater[im_seat].isAvail()) {
          next_m_seat();
        }
        soldseat = &theater[im_seat];
        soldseat->set_proc();
        remaining_seats--;
        m_cust++;
    }

    pthread_mutex_unlock(&seat_mutex);
    return soldseat;
}

//l seller sells the seat
seat *l_sell()
{
    seat *soldseat = NULL;

    pthread_mutex_lock(&seat_mutex);

    //if there is still free seats
    if (remaining_seats > 0) {
        if (!theater[il_seat].isAvail()) {
          next_l_seat();
        }
        soldseat = &theater[il_seat];
        soldseat->set_proc();
        remaining_seats--;
        l_cust++;
    }

    pthread_mutex_unlock(&seat_mutex);
    return soldseat;
}

void wakeup_all_seller_threads() {
  usleep(1000);
  // wait for the threads to finish being created, otherwise the signal gets sent too fast
  pthread_mutex_lock(&start_mutex);
  pthread_cond_broadcast(&cond);
  pthread_mutex_unlock(&start_mutex);
}

// seller thread to serve one time slice (1 minute)
void * sell(void * param)
{
  int id = *(int*)param;
  char seller_type;

  if (id == 0) {
    seller_type = 'H';
  } else if (id < 4) {
    seller_type = 'M';
  } else {
    seller_type = 'L';
  }

  pthread_mutex_lock(&start_mutex);
  pthread_cond_wait(&cond, &start_mutex);
  pthread_mutex_unlock(&start_mutex);

  if (current_cust[id] != NULL) {
    // NOTE finalize sale
    char seatname[5];
    snprintf(seatname, 5, "%c%d%02d", seller_type, id, current_cust[id]->custid());
    theater[current_cust[id]->seatid()].set_sold(string(seatname));
    printf("[%02d:00] [%c%d] sold %c%03d to customer %d\n", global_time, seller_type, id, seller_type, current_cust[id]->seatid(), current_cust[id]->custid());
    current_cust[id] = NULL;
  }

  if (queues[id].empty()) {
    // nothing to run
    return NULL;
  }
  customer* next_cust = queues[id].front();
  if (wakeup_time[id] > global_time || next_cust->arrival() > global_time) {
    // not ready yet
    return NULL;
  }

  //[EVENT] next_cust arrived, announce it
  printf("[%02d:00] [%c%d] Customer %d arrived to the queue.\n", global_time, seller_type, id, next_cust->custid());

  seat* nextseat = NULL;
  if (seller_type == 'H') {
    wakeup_time[id] += rand() % 2 + 1;
    nextseat = h_sell();
  }
  else if (seller_type == 'M') {
    wakeup_time[id] += rand() % 3 + 2;
    nextseat = m_sell();
  }
  else if (seller_type == 'L') {
    wakeup_time[id] += rand() % 4 + 4;
    nextseat = l_sell();
  }

  if (nextseat == NULL) {
    // NOTE: [EVENT] turn customer away
    printf("[%02d:00] [%c%d] Ran out of seats; turned away customer %d.\n", global_time, seller_type, id, next_cust->custid());
  } else {
    // NOTE: [EVENT] successful assignment
    current_cust[id] = queues[id].front();
    printf("[%02d:00] [%c%d] Assigned Customer %d to Seat %d\n", global_time, seller_type, id, next_cust->custid(), nextseat->seatID());
    next_cust->set_seat(nextseat->seatID());
  }

  queues[id].pop();

  return NULL; // thread exits
}

string layout_string() {
  string layout = " ====== Theater ======";
  for (int i = 0; i < 100; i++) {
    if (i % 10 == 0) {
      layout += "\n";
    }
    layout += theater[i].seatname() + " ";
  }
  return layout;
}

int main(int argc, char *argv[]) {
    //input 5,10,15
    if(argc < 2){
        input_customer = 5;
        cout << "no input number... default input customer as " << input_customer << endl;
    }
    else{
        input_customer = atoi(argv[1]);
        cout << "number of customers: " << input_customer << endl;
    }

    // init clock
    global_time = 0;

    generateAllQueues(input_customer);

    init_theater();
    seat_map_init();

    int arg[10] = {0,1,2,3,4,5,6,7,8,9};

    while (global_time < 60) {
      pthread_t tids[10];

      for (int i = 0; i < 10; i++) {
        pthread_create(&tids[i], NULL, sell, &arg[i]);
      }
      wakeup_all_seller_threads();

      // for (int i = 0; i < 10; i++) {
      for (int i = 9; i > -1; i--) {
        pthread_join(tids[i], NULL);
      }

      ++global_time;
    }

    printf("%s\n", layout_string().c_str());
}
