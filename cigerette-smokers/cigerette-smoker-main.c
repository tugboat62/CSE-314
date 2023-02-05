#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "cigerette-agent.h"
#include "cigerette-smoker.h"

int main(int argc, char const *argv[])
{   
    if (argc != 2) {
        printf("Usage: %s number-of-agent-iteration\n", argv[0]);
        exit(1);
    }

    int agent_iteration = atoi(argv[1]);
    if (agent_iteration <= 0) {
        printf("Usage: %s number-of-agent-iteration\n", argv[0]);
        printf("\tnumber-of-agent-iteration : must be positive\n");
        exit(2);
    }

    srand(time(NULL));

    #ifdef USE_ZEMAPHORE
    initialize_zemaphores();
    #endif

    pthread_t agent, smoker_tobacco, smoker_paper, smoker_match;
    start_agent(&agent, "Supplier", agent_iteration);
    start_smoker(&smoker_tobacco, TOBACCO);
    start_smoker(&smoker_paper, PAPER);
    start_smoker(&smoker_match, MATCH);
    pthread_join(agent, NULL);

    return 0;
}
