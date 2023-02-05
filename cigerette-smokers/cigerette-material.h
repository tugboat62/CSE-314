#ifndef __CIG_MATERIAL_H__
#define __CIG_MATERIAL_H__

#include <pthread.h>
#define NUMBER_OF_MATERIALS 3

enum MATERIAL {
    TOBACCO, PAPER, MATCH
};

static const char* material_names[NUMBER_OF_MATERIALS] = {
    [TOBACCO] = "Tobacco",
    [PAPER] = "Paper",
    [MATCH] = "Match"
};

// #define USE_ZEMAPHORE

#ifdef USE_ZEMAPHORE
#include "../zemaphore/zemaphore.h"

extern zem_t smoker_zems[NUMBER_OF_MATERIALS];
extern zem_t table_zem;

void initialize_zemaphores();

#else

// Waiting for other two materials
extern pthread_cond_t smoker_cvs[NUMBER_OF_MATERIALS];
extern int on_table_supply_for[NUMBER_OF_MATERIALS];

// Table needs to be clear for agent to provide
extern int table_clear;
extern pthread_mutex_t table_lock;
extern pthread_cond_t table_cv;

#endif

#endif