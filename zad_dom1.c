#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


#define threds_num 3

//Node
typedef struct List{
    int value;
    struct List *next;

}List;

struct List *lista;


pthread_mutex_t mutex;
sem_t sem;


// wyswietlanie listy
void display(struct List *element){
    printf("[");
    while(element){
        
        if(element->value){
            printf("%d, ", element->value);
        }
        element = element->next;
    }
    printf("] \n");
}

//wstawienie elementu na koniec listy
void push_f(int value){
    struct List *newElement;
    struct List *cursor = lista;
    newElement = (struct List*)malloc(sizeof(struct List));
    newElement->value = value;
    newElement->next = NULL;
    
    if(lista == NULL){
        lista = newElement;
    }else{

        // przejscie na koniec listy
        while(cursor->next){
            
            cursor = cursor->next;
        }

        cursor->next = newElement;
    }
}
//sciagniecie elementu z konca listy
// int pop_f_old(struct List *head){

    
//         struct List *cursor = head;
//         struct List *prev;

//         //jezeli jest cos na liscie
//         if(cursor != NULL){
//         // przejscie na koniec listy
//             while(cursor->next){
//                 prev = cursor;
//                 cursor = cursor->next;
            
//             }

//             if(cursor != NULL){
//                 int return_value = cursor->value;
            
                    
//                         free(cursor);
//                         cursor = NULL;
//                         prev->next = NULL; // SUPER WAZNE 
//                     return return_value;
                
//             }
//         }
//         return -1;
// }

int pop_f(){

    if (lista != NULL){
        struct List *cursor = lista->next;
        free(lista);
        lista = cursor;
    }

}
// wraper sciagania z listy na potrzeby tworzenia watku
void* popThread(void  *arg){
    
    int i=0;
    struct List *k = (struct List*) arg;
    
    while(1){
        sem_wait(&sem);
        pthread_mutex_lock(&mutex);
            // printf("watek wlasnie zdjal: %d\n", pop_f(k));
            pop_f();
            display(lista);
        pthread_mutex_unlock(&mutex);
        sleep(3);
    }
}
// wraper sciagania z listy na potrzeby tworzenia watku
void* pushThread(void *arg){
    
    
    int val ;
    
    while(1){
        val = rand()%100;
        pthread_mutex_lock(&mutex);
            // printf("watek wlasnie wlozyl: %d\n", val );
            push_f( val );
            display(lista);
        pthread_mutex_unlock(&mutex);
        
        sem_post(&sem);
        sleep(1);
    }
}






int main(){


    
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


    // raczej bedzie wkladal niz wyjmowl przez sleepy w funkcajch wrpaerach

    for(i=0; i<threds_num; i++ ){

        if(i==0){
           pthread_create( &tid[0], NULL, pushThread, (void *)lista); 
        }else{
           pthread_create( &tid[i], NULL, popThread, (void *) lista); 
        }

    }


  

    //czekanie na watki
    for (i=0; i<threds_num; i++){
        pthread_join(tid[i], NULL);
    }

    sem_destroy(&sem);
    
    free(lista);
}