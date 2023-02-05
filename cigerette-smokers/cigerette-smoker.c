#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "cigerette-smoker.h"

void* smoker_thread(void* data_p) {
    cigerette_smoker* smoker_data = data_p;
    usleep(((rand()%3000)+1000UL)*1000); // Start after 1 to 4 seconds

    printf("Starting Smoker(%s)\n", smoker_data->smoker_type);
    
    while (1) {
        #ifndef USE_ZEMAPHORE
            pthread_mutex_lock(&table_lock);
        #endif

        #ifndef USE_ZEMAPHORE

            // Wait until table is cleared and spply for the smoker
            while (table_clear || !on_table_supply_for[smoker_data->unlimited_supply_of])
                pthread_cond_wait(&smoker_cvs[smoker_data->unlimited_supply_of], &table_lock);
            
            // Use the materials
            on_table_supply_for[smoker_data->unlimited_supply_of] = 0;
            // Clear the table
            table_clear = 1;
            
        #else
            // Wait until supply is on the table
            zem_down(&smoker_zems[smoker_data->unlimited_supply_of]);
        #endif

        printf("Smoker(%s) is smoking...\n", smoker_data->smoker_type);

        // Notify agent table is clear
        #ifndef USE_ZEMAPHORE
            pthread_cond_signal(&table_cv);
        #else
            zem_up(&table_zem);
        #endif

        #ifndef USE_ZEMAPHORE
            pthread_mutex_unlock(&table_lock);
        #endif
    }
    
    free(smoker_data);
}

void start_smoker(pthread_t* thread_p, enum MATERIAL with_supply_of) {
    cigerette_smoker* smoker_data = malloc(sizeof(cigerette_smoker));
    smoker_data->smoker_type = material_names[with_supply_of];
    smoker_data->unlimited_supply_of = with_supply_of;

    pthread_create(thread_p, NULL, smoker_thread, smoker_data);
}