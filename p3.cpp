#include <pthread.h>
#include <stdlib.h>
#include "critical.h"
#include "customer.h"
#include "seat.h"
using namespace std;

customer *queue[10];
seat *theater = NULL;
seat *h_seat = NULL;
seat *m_seat = NULL;
seat *l_seat = NULL;

//pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//compare arrival time
int arrival_compare(const void *s1, const void *s2)
{
    customer *a = (customer *)s1;
    customer *b = (customer *)s2;
    return a->arrival() - b->arrival();
}

//generate for 10 customer queue
void generateQueue(int input)
{
    int arrive,id;
    for(int i = 0; i < 10; i++)
    {
        queue[i] = new customer[input];

        for (int j = 0; j < input; j++){
            arrive = rand()%60;
            id = i * input + j;
            queue[i][j] = customer(id,arrive);
        }
    }

    //sort by arrival
    for (int i = 0; i < 10; i++) {
        qsort((void *)queue[i], input, sizeof(customer),arrival_compare);
    }

    //print out sorted customer queue
    for(int i = 0; i < 10; i++){
        for (int j = 0; j < input; j++) {
            cout<<queue[i][j]<<endl;
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
    h_seat = &theater[0];
    m_seat = &theater[40];
    l_seat = &theater[90];
    remaining_seats = 100;
}

//increment h seller seat
void increase_h_seat()
{
    seat *tmp = h_seat;
    while (!tmp->isAvail() && remaining_seats > 0) {
        tmp++;
    }
    h_seat = tmp;
}

//increase m seller seats
void increase_m_seat()
{
    seat *tmp = m_seat;
    //R5->R6->R4->R7
    while (!tmp->isAvail() && remaining_seats > 0) {
        int seatid = tmp->seatID();
        //R6->R4
        if(seatid == 59){
            tmp = &theater[30];
        }
        //R4->R7
        else if (seatid == 39){
            tmp = &theater[60];
        }
        //R7->R5
        else if (seatid == 69){
            tmp = &theater[40];
        }
        else tmp ++;
    }
    m_seat = tmp;
}

//increament l seller seat
void increase_l_seat()
{
    seat *tmp = l_seat;
    while (!tmp->isAvail() && remaining_seats > 0) {
        int seatid = tmp->seatID();
        //at the end of the row...move to the beginning of the previous row
        if(seatid > 9 && seatid % 10 == 9)
        {
            tmp = &theater[seatid-19];
        }
        else tmp ++;
    }
    l_seat = tmp;
}

//h seller makes the sell of the seats
seat *h_sell()
{
    seat *soldseat = NULL;

    pthread_mutex_lock(&seat_mutex);

    //if there is still free seats
    if (remaining_seats > 0) {
        if (!h_seat->isAvail()) {
            increase_h_seat();
        }
        soldseat = h_seat;
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
        if (!m_seat->isAvail()) {
            increase_m_seat();
        }
        soldseat = m_seat;
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
        if (!l_seat->isAvail()) {
            increase_l_seat();
        }
        soldseat = l_seat;
        soldseat->set_proc();
        remaining_seats--;
        l_cust++;
    }

    pthread_mutex_unlock(&seat_mutex);
    return soldseat;
}

/*
void wakeup_all_seller_threads() {
    pthread_mutex_lock(&mutex);
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
}


// seller thread to serve one time slice (1 minute)
void * sell(char *seller_type)
{
    While (having more work todo) {
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond, &mutex);
        pthread_mutex_unlock(&mutex);
        
		// Serve any buyer available in this seller queue that is ready
		seat *nextseat = NULL;
		if (seller_type == "H") { 
			seat = h_sell();
		}
		else if (seller_type == "M") {
			seat = m_sell();
		}
		else if (seller_type == "L") {
			seat = l_sell();
		}
        
		// now to buy ticket till done with all relevant buyers in their queue ..................
    }
    return NULL; // thread exits
}

*/

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

    generateQueue(input_customer);

    init_theater();
    seat_map_init();

    /*
    int i;
    pthread_t tids[10];
    char seller_type;
    // Create necessary data structures for the simulator.
    // Create buyers list for each seller ticket queue based on the
    // N value within an hour and have them in the seller queue.
    
	// Create 10 threads representing the 10 sellers. 
	seller-type = “H”;
    pthread_create(&tids[0], NULL, sell, &seller-type);
    seller-type = “M”;
    for (i = 1; i < 4; i++)
        pthread_create(&tids[i], NULL, sell, &seller-type);
    seller-type = “L”;
    for (i = 4; i < 10; i++)
        pthread_create(&tids[i], NULL, sell, &seller-type); 
	
	// wakeup all seller threads
    wakeup_all_seller_threads();
	
    // wait for all seller threads to exit
    for (i = 0 ; i < 10 ; i++) Pthread_join(&tids[i], NULL);
    // Printout simulation results
    // ............
    exit(0);
    */
}
