#include<stdio.h>
#include <stdlib.h> 
typedef struct  Node
{
    int data ;
    struct  Node *node;
} node;

//Delete process node in linked list
void deleteNode(node *first,int data,node *last){

    node *next = first ->node;
    if(  next -> data == data ){
        first ->node = next ->node;
        free(next);
        return;
    }

    node *lastNode = next; //save previous node

    next = next ->node; // next node

    while(next != NULL ){
        if(  next -> data == data ){
            if(last == next){
                printf("\ninside second\n");
                lastNode ->node = next ->node;
                free(next);
                *last = *lastNode;
                printf("\n chaged head data =   %d\n",last ->data);
                return;    
            }

            lastNode ->node = next ->node;
            free(next);
            return;
        }
        lastNode = next;
        next = next ->node;

    }
}


node smallestNode(node *first,node *last){

    node *min = first ->node;
    node *n = min ->node;;
    
    while( n != NULL  ){
        if( n ->data < min ->data  )
            min = n;
        n = n ->node;
    }
    
    node smallest = *min;
   deleteNode(first,min ->data,last);
    return smallest;

}



void appendNode(node *last,node *data){
    node *new = (node *) malloc(sizeof(node) );
    *new = *data;
    last -> node = new;
    //deleteNode(node *first,data,last);
    free(data);
    printf("\ndata of new %d \n",new ->data );

}


int main(){
    node *head = (node* )  malloc( sizeof(node)  );
    node *first = head;

    for (int i = 0;i < 5;i++){
        node *second = (node *) malloc(sizeof(node) );
        printf("Enter data : ");
        scanf("%d",&second -> data);
        head -> node = second;
        head = second;

    }
    head -> node = NULL;


    node *n = first ->node;
    while( n != NULL ){
        //int i = 0;
        printf("%d\t",n -> data);
        n = n ->node;
    }

    

    /*int dat ;
    printf("\nEnter delete number = ");
    scanf("%d",&dat);

    deleteNode(first,dat);*/
    node small = smallestNode(first,head);
    //deleteNode(first,small.data,head);

    //node b =  *small;
    //deleteNode(first,small ->data);
    printf("Smallest = %d \n",small.data);
    


    printf("After delete : \n");
    n = first ->node;
    while( n != NULL ){
        //int i = 0;
        printf("%d\t",n -> data);
        n = n ->node;
    }
   // printf("\nhead data = %d",head -> data);
   node *new = (node* )  malloc( sizeof(node)  );
   new ->data = 100;
   new -> node = NULL;
   
   appendNode(head,new);

printf("After append : \n");
    n = first ->node;
    while( n != NULL ){
        //int i = 0;
        printf("%d\t",n -> data);
        n = n ->node;
    }



    /*node *first = (node* )  malloc( sizeof(node)  );
    node *second = (node* )  malloc( sizeof(node)  );
    node *third = (node* )  malloc( sizeof(node)  );

    first -> data = 45;
    first -> node = second;

    second -> data = 50;
    second -> node = third;

    third -> data = 60;
    
    node *n = first;

    while ( n != NULL){
        printf( "%d\t", n -> data);
        n = n -> node;

    }*/



    



    return 0;
}