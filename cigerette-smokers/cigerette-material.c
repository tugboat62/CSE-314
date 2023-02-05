#include "cigerette-material.h"

// #define USE_ZEMAPHORE

#ifdef USE_ZEMAPHORE
#include "../zemaphore/zemaphore.h"

zem_t smoker_zems[NUMBER_OF_MATERIALS];
zem_t table_zem;

void initialize_zemaphores() {
    // No ingridient available in the Table
    zem_init(smoker_zems+TOBACCO, 0);
    zem_init(smoker_zems+PAPER, 0);
    zem_init(smoker_zems+MATCH, 0);

     // For table is cleared
    zem_init(&table_zem, 0);
}

#else

// Waiting for other two materials
pthread_cond_t smoker_cvs[NUMBER_OF_MATERIALS] = {
    [TOBACCO] = PTHREAD_COND_INITIALIZER,
    [PAPER] = PTHREAD_COND_INITIALIZER,
    [MATCH] = PTHREAD_COND_INITIALIZER,
};

int on_table_supply_for[NUMBER_OF_MATERIALS] = {
    [TOBACCO] = 0,
    [PAPER] = 0,
    [MATCH] = 0,
};

// Table needs to be clear for agent to provide
int table_clear = 1;
pthread_mutex_t table_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t table_cv = PTHREAD_COND_INITIALIZER;

#endif