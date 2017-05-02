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

};

typedef struct Arguments{
    sem_t sem;
    int value;
    int thread_id;
    struct List *head;
};

void display(struct List *element){
    
    while(element){
        if(element->value){
            printf("wartosc -> %d \n", element->value);
        }
        element = element->next;
    }
    
}

void push(struct List *head, int value){
    struct List *newElement;
    struct List *cursor = head;
    newElement = (struct List*)malloc(sizeof(struct List));
    newElement->value = value;
    
    while(cursor->next){
        
        cursor = cursor->next;
    }

    cursor->next = newElement;
    
}

void popThread(struct Arguments arg){
    //sem_wait(&(arg.sem));
    int i=0;
    for(i=0; i<8; i++);{
        //printf("watek %d wlasnie zdjal: %d", arg.thread_id, pop(arg.head));
        printf("watek  wlasnie zdjal: ");
    }
}

void pushThread(struct Arguments arg){
    //sem_wait(&(arg.sem));
    //push(arg.head, arg.value);
    //sem_post(&(arg.sem));
    int i=0;
    for(i=0; i<16; i++){
        printf("watek %d wlasnie wlozyl: %d", arg.thread_id, arg.value );
    }
}

int pop(struct List *head){

    
        struct List *cursor = head;

        while(cursor->next){
            cursor = cursor->next;
        }

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


// TO DO:
// - iles watkow (min 3)
// - semafor/sekcja krytyczna, takie tam

int main(){


    struct List *lista;
    struct Arguments *arg;
    sem_t sem;
    pthread_t tid[threds_num];
    pthread_data_t data[threds_num];
    int sem_status, i;


    //glowa listy
    lista = (struct List*)malloc(sizeof(struct List));
    lista->value = -1;

    sem_status = sem_init(&sem, 0, 1);

    if(sem_status == -1){
        printf("bald tworzenia semafora");
        return -1;
    }

    arg = (struct Arguments*)malloc(sizeof(struct Arguments) * threds_num);
    arg->sem = sem;
    arg->head = lista;

    for(i=0; i<threds_num; i++){
        (arg+i)->thread_id = i;
        pthread_create(&tid, NULL, popThread, (arg+i));

    }


    pthread_join(tid, NULL);

    sem_destroy(&sem);
    free(arg);
    free(lista);
}