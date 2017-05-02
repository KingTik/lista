#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>


/**
glowa jest z wartoscia -1
nie mozna usunac glowy


*/

#define threds_num 3

typedef struct List{
    int value;
    struct List *next;

}List;

typedef struct Arguments{
    sem_t sem;
    int value;
    int thread_id;
    struct List *head;
}Arguments;


pthread_mutex_t mutex;
sem_t sem;



void display(struct List *element){
    
    while(element){
        if(element->value){
            printf("wartosc -> %d \n", element->value);
        }
        element = element->next;
    }
    
}

void push_f(struct List *head, int value){
    struct List *newElement;
    struct List *cursor = head;
    newElement = (struct List*)malloc(sizeof(struct List));
    newElement->value = value;
    
    while(cursor->next){
        
        cursor = cursor->next;
    }

    cursor->next = newElement;
    
}

int pop_f(struct List *head){

    
        struct List *cursor = head;
        
        while(cursor->next){
            cursor = cursor->next;
        }

        if(cursor != NULL){
        int return_value = cursor->value;
       
        if(return_value == -1){
            // na liscie jest tylko glowa
            // zgodnie z zalozeniem nie usuwam
            return -1; 
        }else{
            
                free(cursor);
            
            return return_value;
        }
    }
}

void* popThread(void  *arg){
    
    int i=0;
    struct List *k = (struct List*) arg;
    
    sem_wait(&sem);
    pthread_mutex_lock(&mutex);
        printf("watek wlasnie zdjal: %d\n", pop_f(k));
    pthread_mutex_unlock(&mutex);
    
}

void* pushThread(void *arg){
    
    int i=0;
    int val = rand()%100;
    struct List *k = (struct List*) arg;
    
    pthread_mutex_lock(&mutex);
        printf("watek wlasnie wlozyl: %d\n", val );
        push_f(k, val );
    pthread_mutex_unlock(&mutex);
    sem_post(&sem);

}




// TO DO:
// - iles watkow (min 3)
// - semafor/sekcja krytyczna, takie tam

int main(){


    struct List *lista;
    sem_t sem;
    pthread_t tid[threds_num];
    sem_init(&sem, 0, 0);
    int sem_status, i,j,k;
    //glowa listy
    lista = (struct List*)malloc(sizeof(struct List));
    lista->value = -1;
    time_t tt;
    int seed = time (&tt);
    srand(seed);


    sem_status = sem_init(&sem, 0, 0);

    if(sem_status == -1){
        printf("bald tworzenia semafora");
        return -1;
    }


    for(i=0; i<30; i++){
        
        pthread_create( &tid[0], NULL, pushThread, (void *)lista);
        
    }    


    k =1;
    for(i=0; i<30; i++){
                
        
        pthread_create( &tid[k], NULL, popThread, (void *) lista);
        
        k++;
        if(k>threds_num) k = 1;
    }

// printf("-------------------------\n");
// display(lista);

    // pthread_join(tid[0], NULL);
    for (i=0; i<threds_num; i++){
        pthread_join(tid[i], NULL);
    }

    sem_destroy(&sem);
    
    free(lista);
}