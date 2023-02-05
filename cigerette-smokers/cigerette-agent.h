#ifndef __CIG_AGENT_H__
#define __CIG_AGENT_H__

#include <pthread.h>

typedef struct
{
    const char* agent_name;
    int number_of_iteration;
} cigerette_agent;

void start_agent(pthread_t* thread, const char* agent_name, int num_of_iterations);

#endif