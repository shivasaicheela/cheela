#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>

int nR , nP; // number of resources and process
int *R; // resources
int **A; // alloaction
int **maxR; // maximum required resources 
int **needed; // need of resources
int *safeS; // safe sequence of process
int nProRan = 0; // number of process index runs

pthread_mutex_t LR;
pthread_cond_t cond;

// check safe sequence required function(int check_safe();)

int check_safe()
{
	int i,j,k;
	int f[nP], ans[nP], ind = 0; 
    for (k = 0; k < nP; k++) { 
        f[k] = 0; 
    } 
    int needed[nP][nR]; 
    for (i = 0; i < nP; i++) { 
        for (j = 0; j < nR; j++) 
            needed[i][j] = maxR[i][j] - A[i][j]; 
    } 
    int y = 0; 
    for (k = 0; k < 5; k++) { 
        for (i = 0; i < nP; i++) { 
            if (f[i] == 0) { 
  
                int flag = 0; 
                for (j = 0; j < nR; j++) { 
                    if (needed[i][j] > R[j]) 
                        flag = 1; 
                    break; 
                } 
  
                if (flag == 0) { 
                    ans[ind++] = i; 
                    for (y = 0; y < nR; y++) 
                        R[y] += A[i][y]; 
                    f[i] = 1; 
                } 
            } 
        } 
    } 
  
    printf("Following is the SAFE Sequence\n"); 
    for (i = 0; i < nP - 1; i++) 
        printf(" P%d ->", ans[i]); 
    printf(" P%d", ans[nP - 1]); 
  
    return (1); 

}


// process function
//void* PC(void* );

void* PC(void *arg) 
{
		int i,j;
        int p = *((int *) arg);

	// lock resources
        pthread_mutex_lock(&LR);

        // condition check
        while(p != safeS[nProRan])
        {
        	pthread_cond_wait(&cond, &LR);
		}
                

	// process
        printf("\n--> Process %d", p+1);
        printf("\n\tAllocated : ");
        
        for( i=0; i<nR; i++)
        {
        	 printf("%3d", A[p][i]);
		}
               

        printf("\n\tNeeded    : ");
        
        for( i=0; i<nR; i++)
        {
        	 printf("%3d", needed[p][i]);
		}
            
        printf("\n\tAvailable : ");
        
        for( i=0; i<nR; i++)
        {
        	printf("%3d", R[i]);
		}
                

        printf("\n"); 
		sleep(1);

        printf("\tResource Allocated!");
        printf("\n"); 
		sleep(1);
	
       printf("\tProcess Releasing Resource...");
       printf("\n"); 
	//	sleep(1);
        printf("\tResource Released!");

	for( i=0; i<nR; i++)
	{
		R[i] += A[p][i];
	}
            
        printf("\n\tNow Available : ");
        
        for( i=0; i<nR; i++)
        {
        	printf("%d", R[i]);
		}
                
        printf("\n\n");

        sleep(1);

	// condition broadcast
        nProRan++;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&LR);
		pthread_exit(NULL);
}



int main( ) 
{
	int i,j;
	

        printf("\n Enter number of processes : ");
        scanf("%d", &nP);

        printf("\nEnter Number of resources : ");
        scanf("%d", &nR);

        R = (int *)malloc(nR * sizeof(*R));
        printf("Enter the number of Available resources : ");
        for( i=0; i<nR; i++)
                scanf("%d", &R[i]);

        A = (int **)malloc(nP * sizeof(*A));
        for( i=0; i<nP; i++)
                A[i] = (int *)malloc(nR * sizeof(**A));

        maxR = (int **)malloc(nP * sizeof(*maxR));
        for( i=0; i<nP; i++)
                maxR[i] = (int *)malloc(nR * sizeof(**maxR));

        // allocated
        
        printf("\n");
        for( i=0; i<nP; i++) 
		{
                printf("Enter the resource to process %d : ", i+1);
                for( j=0; j<nR; j++)
                        scanf("%d", &A[i][j]);
        }
        printf("\n");

	// maximum required resources
        for( i=0; i<nP; i++)
		{
                printf("\n Enter the maximum resource required by process %d : ", i+1);
                for( j=0; j<nR; j++)
                        scanf("%d", &maxR[i][j]);
        }
        printf("\n");

	// calculate need matrix
        needed = (int **)malloc(nP * sizeof(*needed));
        for( i=0; i<nP; i++)
        {
			needed[i] = (int *)malloc(nR * sizeof(**needed));
		}

        for( i=0; i<nP; i++)
        {
            for( j=0; j<nR; j++)
            {
				needed[i][j] = maxR[i][j] - A[i][j];
		    }
	    }
	    
	// get safe sequence
	safeS = (int *)malloc(nP * sizeof(*safeS));
       for( i=0; i<nP; i++) safeS[i] = -1;

        if(check_safe()!=1)
		{
                printf("\nUnsafe State system to a unsafe state.\n\n");
                exit(-1);
        }
		
       
        printf("\nExecuting Processes.");
        sleep(1);
	
	// run threads
	pthread_t P[nP];
        pthread_attr_t AT;
        pthread_attr_init(&AT);

	int PN[nP];
	for( i=0; i<nP; i++)
	{
		PN[i] = i;
	}

        for( i=0; i<nP; i++)
        {
        	pthread_create(&P[i], &AT, PC, (void *)(&PN[i]));
		}
                
        for( i=0; i<nP; i++)
        {
        	 pthread_join(P[i], NULL);
		}
               
        printf("\nAll Processes Finished\n");	
	
	// free R
        free(R);
        for( i=0; i<nP; i++) 
		{
            free(A[i]);
            free(maxR[i]);
			free(needed[i]);
        }
        free(A);
        free(maxR);
		free(needed);
        free(safeS);
}





