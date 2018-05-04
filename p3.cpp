#include <pthread.h>
#include "critical.h"
#include "customer.h"
#include "seat.h"
using namespace std;


customer *queue[10];
seat *theater;

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

/*
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// seller thread to serve one time slice (1 minute)
void * sell(char *seller_type)
{
    While (having more work todo) {
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond, &mutex);
        pthread_mutex_unlock(&mutex);
        // Serve any buyer available in this seller queue that is ready
        // now to buy ticket till done with all relevant buyers in their queue ..................
    }
    return NULL; // thread exits
}
void wakeup_all_seller_threads() {
    pthread_mutex_lock(&mutex);
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
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
    /*
    int i;
    pthread_t tids[10];
    char Seller_type;
    // Create necessary data structures for the simulator.
    // Create buyers list for each seller ticket queue based on the
    // N value within an hour and have them in the seller queue.
    // Create 10 threads representing the 10 sellers. seller-type = “H”;
    pthread_create(&tids[0], NULL, sell, &seller-type);
    seller-type = “M”;
    for (i = 1; i < 4; i++)
        pthread_create(&tids[i], NULL, sell, &seller-type);
    seller-type = “L”;
    for (i = 4; i < 10; i++)
        pthread_create(&tids[i], NULL, sell, &seller-type); // wakeup all seller threads
    wakeup_all_seller_threads();
    // wait for all seller threads to exit
    for (i = 0 ; i < 10 ; i++) Pthread_join(&tids[i], NULL);
    // Printout simulation results
   // ............
    exit(0);
    */
}
