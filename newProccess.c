#include<stdio.h>
#include<stdlib.h>
#ifdef _WIN32
#include <Windows.h>
#define  Second Sleep(1000);
#else
#include <unistd.h>
#define  Second sleep(1);
#endif
#include<time.h>

// Linked list
typedef struct  Node
{
    int pid ;
    int burstPos ;
    int burst ;
    struct  Node *node;
} node;

// very very important 
typedef struct Hash {
   int pid;   
   int burst;
} hashmap;



//Print created proccesss
void PrintProccess ( int proccess,int Priority[],int Arrival_Time[],int CPUBurst[],  int Bursts[][11]); 

//get random numbers within range  mod
int RandomNum(int mod){
    return (rand() % mod) ;
}

//Find smallest node in linked list
node smallestNode(node *first,node *last);

//Delete node in a linked list with given pid of process
void deleteNode(node *first,int pid,node *last);

//To append node in linked list
void appendNode(node *start,node *last,node *data);

int main(){
    srand(time(0));

    int proccess;
    while( (proccess = RandomNum(10)) < 8  );
    int Priority[proccess];
    int Arrival_Time[proccess];
    int CPUBurst[proccess];
    int Bursts[proccess][11];
    int ArrTime  = 0 ;
    for(int i = 0;i < proccess;i++){
        Priority[i] = RandomNum(10);
        Arrival_Time[i] = ArrTime;
        ArrTime  += RandomNum(10);
        CPUBurst[i] = RandomNum(10);
        while( (CPUBurst[i]) < 3 ){
            CPUBurst[i] = RandomNum(10);
        }
        if( CPUBurst[i] % 2 == 0) CPUBurst[i] +=1; 


        for (int j = 0 ; j < CPUBurst[i];j++){
                while( (Bursts[i][j] = RandomNum(10)) ==  0  );

        }
    }

   PrintProccess (proccess,Priority,Arrival_Time,CPUBurst, Bursts );


    //Timing
    clock_t begin;
    double time_spent;

    /* Mark beginning time */
    begin = clock();



    //Ready Linked Lists
    node *head = (node* )  malloc( sizeof(node)  );
    node *firstReady = head;

    //Running Linked list
    node *runningNode = (node* )  malloc( sizeof(node)  );
    node *firstRunning = runningNode;
    runningNode ->node = NULL;
    node running;

    // To measure process running time
    clock_t startRunning;
    double time_running;



    //Blocked Linked list
    node *headBlocked = (node* )  malloc( sizeof(node)  );
    node *firstBlocked = headBlocked;



    //Proccess ID
    int pos = 0; // proccess position
    int readyPos = 0; // ready queue position

    

    while(1){
        time_spent = (double)(clock() - begin) / CLOCKS_PER_SEC;

        //LTS -------------
        //Add proccess to ready list
        if(time_spent  >=  Arrival_Time[pos]) {
              node *second = (node *) malloc(sizeof(node) );
                second -> pid =  pos;
                second ->burstPos = 1;
                second -> burst = Bursts[pos][second ->burstPos];
                

                if(firstReady ->node == NULL ){
                    head = firstReady;
                }

                head ->node = second;
                head = second;
                head -> node = NULL;
                pos++;
        }
        //head -> node = NULL; // Make last node value NULL

        node *n = firstReady ->node;
        while(n != NULL){
            int pid = n -> pid;
            printf( "Ready Proccess  = %d  %d\n",++pid,n ->burstPos);
            n = n -> node;
        }

       //LTS ends ------------

        //STS ------
        //---------------------------------------
        printf ("-------------------------------------------------------------------------------\n");
        if (firstReady ->node != NULL && runningNode ->node == NULL ){
            running = smallestNode(firstReady,head);
            int pid = running. pid;
            node *runningN = (node *) malloc(sizeof(node) );

            runningN  = &running;
            runningN ->node = NULL;
            runningNode ->node = runningN;

           // printf( "Running Proccess  = %d  %d  %d\n", ++pid,running. burst,running.node);
           /* if(runningNode ->node -> node != NULL){
                 printf( "next Running Proccess  is null %d\n  ", runningNode ->node -> pid );
            }*/


            /* Mark beginning time */
            startRunning = clock();




        }
        
        if( runningNode ->node != NULL  ){
            printf(" Running Process is %d  \n",runningNode ->node ->pid);
        }
        //---------------------------------------
        //STS end

        //MTS 
        //---------------------------------------------
        /* See how much time process is running  */
        time_running = (double)(clock() - startRunning) / CLOCKS_PER_SEC;

        if( runningNode ->node != NULL &&   time_running >= runningNode ->node ->burst ){
            printf("Proccess %d has completed cup cycle \n",runningNode ->node ->pid );

            
            //printf("Proccess ended busrt %d  \n",runningNode ->node ->burst);

            //Changing burst time in node
            int oldBurst=  runningNode ->node ->burstPos;
            runningNode ->node ->burstPos = ++oldBurst;  
            runningNode ->node -> burst = Bursts[runningNode ->node ->pid][oldBurst];

            //printf("Proccess new busrt %d  ",runningNode ->node ->burst);


            //appendNode(firstReady,head,runningNode ->node);

            //Adding node for blocked list
            node *nextBlocked = (node *) malloc(sizeof(node) );

            nextBlocked ->pid = runningNode ->node ->pid;
            nextBlocked ->burstPos = runningNode ->node ->burstPos;
            nextBlocked ->burst = runningNode ->node ->burst;
            nextBlocked ->node = NULL;                     

            headBlocked ->node = nextBlocked;
            headBlocked = nextBlocked;
            headBlocked ->node = NULL;

            //Deleteing running Node
            deleteNode(runningNode,runningNode ->node ->pid,runningNode ->node);
            runningNode ->node = NULL;
        }
        //headBlocked ->node = NULL;
        printf ("-------------------------------------------------------------------------------\n");
        if(firstBlocked ->node != NULL){
            node *n = firstBlocked ->node;
            while(n != NULL){
                int pid = n -> pid;
                printf( "Blocked Proccess  = %d  %d\n",pid,n ->burstPos);
                n = n -> node;
            }
        }



        //----------------------------------------------
        //MTS end


        printf ("#########################################################################################\n\n");

        Second ;

    }



    

































    return 0 ;
}

//Print Proccess
void PrintProccess ( int proccess,int Priority[],int Arrival_Time[],int CPUBurst[],  int Bursts[][11]){
    printf("PID\tPriority\tArrival Time\tCPU Burst \tBursts\n");
    for (int  i = 0 ; i < proccess;i++){
         printf("P%d\t%d\t\t%d\t\t%d\t\t",i+1,Priority[i], Arrival_Time[i],CPUBurst[i] );
        for(int j = 0;j < CPUBurst[i];j++){
            printf("%d\t",  Bursts[i][j]  ); 
        }
        printf("\n");
    }
}

//Find node with smallest burst size
node smallestNode(node *first,node *last){

    node *min = first ->node;
    node *n = min ->node;;
    
    while( n != NULL  ){
        if( n ->burst < min ->burst  )
            min = n;
        n = n ->node;
    }
    node smallest = *min;
    deleteNode(first,min -> pid,last);
    return smallest;

}
/*node* smallestNode(node *first){

    node *min = first ->node;
    node *n = min ->node;;
    
    while( n != NULL  ){
        if( n ->burst < min ->burst  )
            min = n;
        n = n ->node;
    }

    return min;

}*/

//Delete process node in linked list
void deleteNode(node *first,int pid,node *last){

    node *next = first ->node;
    if(  next -> pid == pid ){
        first ->node = next ->node;
        free(next);
        return;
    }

    node *lastNode = next; //save previous node

    next = next ->node; // next node

    while(next != NULL ){
        if(  next -> pid == pid ){
            lastNode ->node = next ->node;
            free(next);
            return;
        }
        lastNode = next;
        next = next ->node;

    }
}


//Append to linked list
void appendNode(node *start,node *last,node *data){
    node *new = (node *) malloc(sizeof(node) );
    *new = *data;
    //printf("\ndata of last %d \n",last ->pid );
    last -> node = new;
    //printf("\ndata of last %d \n",last ->pid );
    //deleteNode(node *first,data,last);
    free(data);
    printf("\n  --------  proccesses in ready list --------------\n" );


    node *n = start ->node;
        while(n != NULL){
            int pid = n -> pid;
            printf( "Ready Proccess  = %d  %d\n",++pid,n ->burstPos);
            n = n -> node;
        }
}