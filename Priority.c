
#include "linkedList.c"

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#ifdef _WIN32
#include <Windows.h>
#define  Second Sleep(1000);
#define  PER_SECOND CLOCKS_PER_SEC;
#else
#include <unistd.h>
#define  Second sleep(1);
#define  PER_SECOND (clock_t) 100;
#endif


//Print created proccesss
void PrintProccess ( int proccess,int Priority[],int Arrival_Time[],int CPUBurst[],  int Bursts[][11]); 

//get random numbers within range  mod
int RandomNum(int mod){
    return (rand() % mod) ;
}

int main(){
    srand(time(0));

    int proccess = 5 ;
   // while( (proccess = RandomNum(500)) < 400  );
    int Priority[proccess];
    int Arrival_Time[proccess];
    int CPUBurst[proccess];
    int Bursts[proccess][11];
    int ArrTime  = 0 ;
    for(int i = 0;i < proccess;i++){

        Priority[i] = RandomNum(10);

        while( Priority[i] == 0 ) {
            Priority[i] = RandomNum(10);
        }


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
    firstReady ->node = NULL;

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
    //firstBlocked ->node = NULL;

    //Proccess ID
    int pos = 0; // proccess position
    int readyPos = 0; // ready queue position

    // counter for completed proccess
    int proccessCompleted = 0;

    while(proccessCompleted < proccess  ){
        time_spent = (double)(clock() - begin) / PER_SECOND;

        //LTS -------------
        //Add proccess to ready list
        if(pos <  proccess && time_spent  >=  Arrival_Time[pos]  ) {
              node *second = (node *) malloc(sizeof(node) );
                second -> pid =  pos;
                second ->priority = Priority[pos];
                second ->burstPos = 1;
                second -> burst = Bursts[pos][second ->burstPos];
                second ->node = NULL;

                if( firstReady ->node == NULL  ){                    
                    firstReady ->node = second;
                }
                else{                    
                    Append( firstReady ,second);
                }
                pos++;
        }
        

        node *n = firstReady ->node;
        while(n != NULL){
            int pid = n -> pid;
            printf( "Ready Proccess  = P%d  %d\n",pid+1,n ->priority);
            n = n -> node;
        }

       //LTS ends ------------

        //STS ------
        //---------------------------------------
        printf ("-------------------------------------------------------------------------------\n");
        if (firstReady ->node != NULL && runningNode ->node == NULL ){
            running = highPriorityNode(firstReady,head);

            int pid = running. pid;
            node *runningN = (node *) malloc(sizeof(node) );

            runningN  = &running;
            runningN ->node = NULL;

            runningNode ->node = runningN;




            /* Mark beginning time of running proccess */
            startRunning = clock();




        }
        
        if( runningNode ->node != NULL  ){
            int pid = runningNode ->node ->pid;
            printf(" Running Process is P%d   %d \n", pid+1, runningNode ->node ->priority );
        }
        //---------------------------------------
        //STS end

        //MTS 
        //---------------------------------------------
        /* See how much time process is running  */
        time_running = (double)(clock() - startRunning) / PER_SECOND;

        if( runningNode ->node != NULL &&   time_running >= runningNode ->node ->burst ){
            int pid = runningNode ->node ->pid;
            printf("Proccess %d has completed CPU cycle \n",pid +1 );

            
            

            //Changing burst time in node
            int oldBurst =  runningNode ->node ->burstPos;
            runningNode ->node ->burstPos = ++oldBurst;  
            runningNode ->node -> burst = Bursts[runningNode ->node ->pid][runningNode ->node ->burstPos];

            

            
            //Creating temp node address to add to blocked list
            node *nextBlocked = (node *) malloc(sizeof(node) );

            nextBlocked ->pid = runningNode ->node ->pid;
            nextBlocked ->priority = runningNode ->node ->priority;
            nextBlocked ->burstPos = runningNode ->node ->burstPos;
            nextBlocked ->burst = runningNode ->node ->burst;
            nextBlocked ->blockedTime = 0; 
            

            //Adding temp node for blocked list
            if(firstBlocked ->node != NULL  ){
                Append(firstBlocked,nextBlocked);
            }else
            {
                nextBlocked ->node = NULL;
                firstBlocked ->node = nextBlocked;
            }
            
            //Deleteing running Node
            deleteNode(runningNode,runningNode ->node ->pid,runningNode ->node);
            
        }
        
        printf ("-------------------------------------------------------------------------------\n");
        if(firstBlocked ->node != NULL){
            node *n = firstBlocked ->node;
            while(n != NULL){
                int pid = n -> pid;
                n ->blockedTime =  ++(n -> blockedTime);
                printf( "Blocked Proccess  = P%d  %d\n",pid+1,n ->blockedTime);
                n = n -> node;
            }
        }

        if(firstBlocked ->node != NULL){
            node *n = firstBlocked ->node;
            while(n != NULL){
                
                int pid = n -> pid;
                
                if(n ->blockedTime >= n->burst && n ->burstPos >= (  CPUBurst[pid] -1  ) ){
                    printf( "proccess execution  has been completed  = P%d  %d\n",pid+1,n ->blockedTime);
                    //Deleting node from blocked linked list
                    deleteNode(firstBlocked,pid,headBlocked);
                    n = n -> node;
                    //increment counter for completed process
                    proccessCompleted++;
                    continue;
                }

                //If burst is complete add it to ready linked list with next burst
                if( n ->blockedTime >= n->burst  ){
                    ///------------------
                    node *second = (node *) malloc(sizeof(node) );
                    second -> pid =  pid;
                    second ->burstPos = (n ->burstPos)+1;
                    second ->priority = n ->priority;
                    second -> burst = Bursts[pid][second ->burstPos];
                    second ->node = NULL;
                    

                    printf( "proccess complete blocked cycle  = P%d  %d\n",pid+1,n ->blockedTime);



                    
                    

                    if( firstReady ->node == NULL  ){
           
                        firstReady ->node = second;
                    }
                    else{
           
                        Append( firstReady ,second);

                    }


                    
                    
                


                    
                    //Deleting node from blocked linked list
                    deleteNode(firstBlocked,pid,headBlocked);
                    ///------------------


                    

                    
                }

                
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