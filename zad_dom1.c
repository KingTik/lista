#include <stdio.h>
#include <stdlib.h>


typedef struct List{
    int value;
    struct List *next;

}List;

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


int pop(struct List *head){

    struct List *cursor = head;

    while(cursor->next){
        cursor = cursor->next;
    }

    int return_value = cursor->value;
    free(cursor);
    return return_value;
}


// TO DO:
// - iles watkow (min 3)
// - semafor/sekcja krytyczna, takie tam

int main(){


struct List *lista;


lista = (struct List*)malloc(sizeof(struct List));
lista->value = -1;

for (int i=0; i< 15; i++){
    push(lista, i);
}
display(lista);

printf("-->%d", pop(lista));

display(lista);


free(lista);
}