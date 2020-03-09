 #include<stdlib.h>
 typedef struct  Node
{
    int pid ;
    int priority;
    int blockedTime;
    int burstPos ;
    int burst ;
    struct  Node *node;
} node;



//Find smallest node in linked list
node smallestNode(node *first,node *last);

//Find node / process with highest priority1
node highPriorityNode(node *first,node *last);

//Delete node in a linked list with given pid of process
int deleteNode(node *first,int pid,node *last);

//To append node in linked list
void Append(node *start,node *insert);


//Find node with smallest burst size
node smallestNode(node *first,node *last){

    node *min = first ->node;
    node *n = min ->node;
    
    while( n != NULL  ){
        if( n ->burst < min ->burst  )
            min = n;
        n = n ->node;
    }
    node smallest = *min;
    deleteNode(first,min -> pid,last);
    return smallest;

}


//Find node / process with highest priority
node highPriorityNode(node *first,node *last){
    node *big = first ->node;
    node *n = big ->node;
    
    while( n != NULL  ){
        if( n ->priority > big ->priority  )
            big = n;
        n = n ->node;
    }
    node biggestPriorityNode = *big;
    deleteNode(first,big -> pid,last);
    return biggestPriorityNode;
}


//Delete process node in linked list
int deleteNode(node *first,int pid,node *last){

    node *next = first ->node;
    if(  next -> pid == pid ){
        first ->node = next ->node;
        //free(next);
        return 0;
    }

    node *lastNode = next; //save previous node

    next = next ->node; // next node

    while(next != NULL ){
        if(  next -> pid == pid ){
            lastNode ->node = next ->node;
            //free(next);
            return 0;
        }
        lastNode = next;
        next = next ->node;

    }
}

//Append to linked list
void Append(node *start,node *insert)
{
    node  *new_node = (node *)malloc(sizeof( node));
    *new_node = *insert;
    new_node ->node = NULL;

    node *ptr = start->node;

    while(ptr -> node != NULL){ 
        //printf("inside append cuu node =   %d \n", ptr ->pid);
    ptr = ptr -> node;
}
    ptr -> node = new_node;

}


