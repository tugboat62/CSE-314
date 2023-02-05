#ifndef __CIG_SMOKER_H__
#define __CIG_SMOKER_H__

#include "cigerette-material.h"

typedef struct
{
    const char* smoker_type;
    enum MATERIAL unlimited_supply_of;
} cigerette_smoker;

void start_smoker(pthread_t* thread, enum MATERIAL with_supply);

#endif