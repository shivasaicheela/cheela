# bankers algorthim
.Write a multithreaded program that implements the banker's algorithm. Create n threads that request and release resources from the bank.
The banker will grant the request only if it leaves the system in a safe state. It is important that shared data be safe from concurrent 
access. To ensure safe access to shared data, you can use mutex locks.

solution:
Banker’s algorithm consists of Safety algorithm and Resource request algorithm
1)	Let Work and Finish be vector c. Initialize 
2)	Work =Available & finish[i]=false
3)	Find and index i such that both
4)	Finish[i]=false
5)	Need i <=Work
6)	If no such i exit, go to step 9
7)	Work=Work +Allocation[i]
8)	Finish[i]=true, go to step 3,4,5
9)	If finish[i]==true for all I, then the system is in a safe state.

Resource allocation
1)	If Request i<=need, go to step 2
2)	If Request i<=Available, go to step 3 otherwise P[i] must wait so that the resources are not available.
3)	Have the system pretent to have allocated the request resources to process P[i] by modifying the state as follow 
Available =Available-Request
	Allocation=Allocation[i]+Request[i];
	Need [i]=Need[i]-Request[i]
Give the user input for number of process and number of resources nd aslo alloacte the max available, alloaction,need values 
and uoy will obtain the safe sequence with mutex locks, avoidance to dead lock
