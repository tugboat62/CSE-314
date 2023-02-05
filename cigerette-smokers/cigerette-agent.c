#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "cigerette-material.h"
#include "cigerette-agent.h"

void* agent_thread(void* data_p) {
    cigerette_agent* agent_data = data_p;
    usleep(((rand()%3000)+1000UL)*1000); // Start after 1 to 4 seconds

    printf("Starting Agent(%s)\n", agent_data->agent_name);

    for (int i=0; i<agent_data->number_of_iteration; i++) {
        int selected_material_one = rand() % NUMBER_OF_MATERIALS;
        
        // Choose random variable without the first selected variable
        int selected_material_two = rand() % (NUMBER_OF_MATERIALS-1);
        if (selected_material_two >= selected_material_one)
            selected_material_two++;
        
        // Expected Smoker to pick-up the materials
        int signal_to = (TOBACCO + PAPER + MATCH) - (selected_material_one + selected_material_two);
        
        #ifndef USE_ZEMAPHORE
            pthread_mutex_lock(&table_lock);
        #endif 
            
        // Show console message about the materials that has been put to table
        printf(
            "%s is puting %s and %s on the table\n", 
            agent_data->agent_name, 
            material_names[selected_material_one], 
            material_names[selected_material_two]
        );
        
        #ifndef USE_ZEMAPHORE
        
            // Table has material for this smoker
            on_table_supply_for[signal_to] = 1;
            // Table is not empty
            table_clear = 0;

            // Signal Smoker to pick up the materials
            pthread_cond_signal(&smoker_cvs[signal_to]);

            // Wait for table to be cleared
            while (!table_clear)
                pthread_cond_wait(&table_cv, &table_lock);
        
        #else

            zem_up(smoker_zems+signal_to);
            zem_down(&table_zem);

        #endif

        #ifndef USE_ZEMAPHORE
            pthread_mutex_unlock(&table_lock);
        #endif
    }
    
    free(agent_data);
}

void start_agent(pthread_t* thread_p, const char* agent_name, int num_of_iterations){
    cigerette_agent* agent_data = malloc(sizeof(cigerette_agent));
    agent_data->agent_name = agent_name;
    agent_data->number_of_iteration = num_of_iterations;
    
    pthread_create(thread_p, NULL, agent_thread, agent_data);
}